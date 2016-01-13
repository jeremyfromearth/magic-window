#include "MagicWindow/MagicWindowApp.h"

using namespace ci;
using namespace ci::app;
using namespace ci::params;
using namespace magicwindow;

//
// Public Methods
//
bool MagicWindowApp::initialize(std::string assetPath)  {
    if (assetPath != "") {
        addAssetDirectory(getDocumentsDirectory() /= assetPath);
    }

    DataSourceRef cfgData;
    try {
        cfgData = loadAsset("cfg.json");
    }
    catch (AssetLoadExc exception) {
        ctx.signals.logError.emit("Could not load cfg.json.\n" + toString(exception.what()), true);
        quit();
        return false;
    }

    JsonTree cfg;
    try {
        cfg = JsonTree(cfgData);
    } catch (JsonTree::ExcJsonParserError exception) {
        ctx.signals.logError.emit("Could not parse cfg.json.\n" + toString(exception.what()), true);
        quit();
        return false;
    }
    catch (JsonTree::ExcChildNotFound exception) {
        ctx.signals.logError.emit("Could not parse cfg.json.\n" + toString(exception.what()), true);
        quit();
        return false;
    }
    
    try {
        ctx.config.initialize(cfg);
    }
    catch (std::exception exc) {
        console() << exc.what() << std::endl;
    }
    
    ctx.config.setAssetPath(assetPath);
    ctx.config.doShowCursor() ? showCursor() : hideCursor();
    initializeWindowConfiguration();
    return true;
}

void MagicWindowApp::initializeWindowConfiguration() {
    WindowRef window;
    JsonTree windowConfig = ctx.config.getWindowConfig();

    // A window for each display with width and height matching the display
    if (ctx.config.getWindowMode() == WindowConfig::DISPLAY_SPAN) {
        
        std::vector<DisplayRef> displays = Display::getDisplays();
        
        for (int i = 0; i < displays.size(); i++) {
            if (i == 0) {
                window = getWindow();
            }
            else {
                window = createWindow();
            }

            Rectf bounds = displays[i]->getBounds();
            window->setUserData(new WindowConfig(i, bounds, vec2()));
            window->setPos(bounds.getUpperLeft());
            window->setSize(bounds.getSize());
            window->setBorderless();
            if (ctx.config.getFullScreen()) window->setFullScreen();
        }
    }

    // As many windows as defined in the window_config variable
    if (ctx.config.getWindowMode() == WindowConfig::DISPLAY_CUSTOM) {
        WindowRef window;
        for (JsonTree::Iter windowIt = windowConfig.begin(); windowIt != windowConfig.end(); windowIt++) {
            float x, y, w, h;
            //JsonTree coords = windowIt->getChild("coords");
            int index = std::distance(windowConfig.begin(), windowIt);
            
            x = windowIt->getChild("x").getValue<int>() * ctx.config.getAppScale();
            y = windowIt->getChild("y").getValue<int>() * ctx.config.getAppScale();
            w = windowIt->getChild("w").getValue<int>() * ctx.config.getAppScale();
            h = windowIt->getChild("h").getValue<int>() * ctx.config.getAppScale();

            if (windowIt == windowConfig.begin()) {
                window = getWindow();
            }
            else{
                window = createWindow();
            }

            window->setUserData(new WindowConfig(index, Rectf(x, y, x + w, y + h), vec2(), false));
            window->setBorderless();
            window->setPos(x, y);
            window->setSize(w, h);
            if (ctx.config.getFullScreen()) window->setFullScreen();
        }
    }
    
    if (ctx.config.getWindowMode() == WindowConfig::DISPLAY_GRID) {
        WindowRef window;
        int rows = json::get(windowConfig, "rows", 1);
        int cols = json::get(windowConfig, "columns", 1);
        int w = json::get(windowConfig, "screen_width", 960) * ctx.config.getAppScale();
        int h = json::get(windowConfig, "screen_height", 540) * ctx.config.getAppScale();
        
        int index = 0;
        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                // User the default window if this is the first iteration
                if(r == 0 && c == 0) {
                    window = getWindow();
                } else {
                    window = createWindow();
                }
                
                // Calculate the coordinates of each window
                int x = c * w;
                int y = r * h;
                
                #if defined CINDER_MAC
                // This is an ugly hack to account for the OSX toolbar
                if(!ctx.config.getFullScreen()) if(r != 0) y += 23;
                #endif
                
                window->setBorderless();
                window->setSize(w, h);
                window->setPos(x, y);
                window->setAlwaysOnTop();
                window->setUserData(new WindowConfig(index, Rectf(x, y, w, h), vec2(), false));
                if(ctx.config.getFullScreen()) window->setFullScreen();
                index++;
            }
        }
    }

    if (ctx.config.doShowParams()) {
        WindowRef window = createWindow();
        window->setUserData(new WindowConfig(-1, Rectf(), vec2(), true));
        window->setSize(300, 500);
        window->setAlwaysOnTop(true);
        window->setPos(ctx.config.getParamWindowCoords());
        ctx.params = InterfaceGl::create(window, "Debug Params", vec2(265, 465));
        ctx.params->addText("FPS", "label='FPS should display here'");
        ctx.params->addSeparator();
    }
}

void MagicWindowApp::draw() {
    WindowRef window = getWindow();
    WindowConfig * data = window->getUserData<WindowConfig>();
    gl::setMatricesWindow(getWindowSize());

    if (data) {
        if (data->isDebugWindow()) {
            gl::clear(Color::black());
            ctx.params->draw();
        } else {
            gl::clear();
            ctx.signals.preDrawTransform.emit();
            gl::pushMatrices();
            gl::translate(-data->getTranslation());
            gl::scale(vec3(ctx.config.getAppScale(), ctx.config.getAppScale(), ctx.config.getAppScale()));
            ctx.signals.draw.emit();
            gl::popMatrices();
            ctx.signals.postDrawTransform.emit();
        }
    }
}

void MagicWindowApp::fileDrop(FileDropEvent e) { ctx.signals.fileDrop.emit(e); }

void MagicWindowApp::update() {
    if (ctx.config.doShowParams() && ctx.params) {
        ctx.params->setOptions("FPS", "label='FPS: " + toString(getAverageFps()) + "'");
    }
    
    ctx.state.update();
    ctx.signals.update.emit();
}

void MagicWindowApp::keyDown(KeyEvent e) {
    if (ctx.config.getDefaultKeyHandlersEnabled()) {
        switch (e.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        case KeyEvent::KEY_m:
            if (e.isControlDown()) {
                ctx.config.setCursorVisibility(!ctx.config.doShowCursor());
                ctx.config.doShowCursor() ? showCursor() : hideCursor();
            }
            break;
        }
    }
    ctx.signals.keyDown.emit(e);
}

void MagicWindowApp::keyUp(KeyEvent e) { ctx.signals.keyUp.emit(e); }
void MagicWindowApp::mouseDown(MouseEvent e) { ctx.signals.mouseDown.emit(e); }
void MagicWindowApp::mouseDrag(MouseEvent e) { ctx.signals.mouseDrag.emit(e); }
void MagicWindowApp::mouseMove(MouseEvent e) { ctx.signals.mouseMove.emit(e); }
void MagicWindowApp::mouseUp(MouseEvent e) { ctx.signals.mouseUp.emit(e); }
void MagicWindowApp::mouseWheel(MouseEvent e) { ctx.signals.mouseWheel.emit(e); }
void MagicWindowApp::shutdown() { ctx.signals.shutdown.emit(); }