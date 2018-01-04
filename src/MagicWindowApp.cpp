#include "MagicWindow/MagicWindowApp.h"

using namespace ci;
using namespace ci::app;
using namespace ci::params;
using namespace magicwindow;

//
// Public Methods
//

bool MagicWindowApp::initialize(JsonTree data) {
    try {
        ctx.config.initialize(data);
    }
    catch (std::exception exc) {
        CI_LOG_EXCEPTION("Could not initialize.", exc);
        return false;
    }

    ctx.config.do_show_cursor() ? showCursor() : hideCursor();
    initializeWindowConfiguration();
    return true;
}

bool MagicWindowApp::initialize(std::string configFilename)  {
    DataSourceRef cfgData;
    try {
        cfgData = loadAsset(configFilename);
    }
    catch (AssetLoadExc exc) {
        CI_LOG_EXCEPTION("Could not load config file. Goodbye.", exc);
        return false;
    }

    try {
        return initialize(JsonTree(cfgData));
    }
    catch (std::exception exc) {
        CI_LOG_EXCEPTION("Could not parse the config file. Adios.", exc);
        return false;
    }
}

void MagicWindowApp::initializeWindowConfiguration() {
    
    float appScale = ctx.config.get_app_scale();
    JsonTree windowConfig = ctx.config.get_window_config();
	
    paramsWindow = getWindow();
    paramsWindow->setUserData(new WindowConfig(-1, Rectf(), vec2()));
    paramsWindow->setSize(500, 300);
    paramsWindow->setPos(ctx.config.get_param_coords());
    ctx.params = InterfaceGl::create(paramsWindow, "Debug Params", vec2(470, 270));
    ctx.params->addText("FPS", "label='FPS should display here'");
    ctx.params->addSeparator();
    paramsWindowIsAvailable = true;
    if (!ctx.config.do_show_params()) paramsWindow->hide();
    paramsWindow->getSignalClose().connect([&] {
        paramsWindowIsAvailable = false;
    });
    
    // A window for each display with width and height matching the display
    if (ctx.config.get_window_mode() == WindowConfig::DISPLAY_SPAN) {
        std::vector<DisplayRef> displays = Display::getDisplays();
        for (int i = 0; i < displays.size(); i++) {
            Rectf bounds = displays[i]->getBounds();
            WindowRef window = createWindow();
            window->setUserData(new WindowConfig(i, bounds, vec2()));
            window->setPos(bounds.getUpperLeft());
            window->setSize(bounds.getSize());
            window->setBorderless();
            if (ctx.config.is_fullscreen()) window->setFullScreen();
        }
    }
    
    
    // As many windows as defined in the window_config variable
    if (ctx.config.get_window_mode() == WindowConfig::DISPLAY_CUSTOM) {
        for (JsonTree::Iter windowIt = windowConfig.begin(); windowIt != windowConfig.end(); windowIt++) {
            int index = std::distance(windowConfig.begin(), windowIt);

			int x = windowIt->getChild("x").getValue<int>();
			int y = windowIt->getChild("y").getValue<int>();
            int w = windowIt->getChild("w").getValue<int>();
            int h = windowIt->getChild("h").getValue<int>();
			int xs = x * appScale;
			int ys = y * appScale;
			int ws = w * appScale;
			int hs = h * appScale;
            
            #if defined CINDER_MAC
            // This is an ugly hack to account for the OSX toolbar
            if(!ctx.config.is_fullscreen()) {
                y += 22;
                ys += 22;
            }
            #endif

            WindowRef window = createWindow();
            window->setUserData(
				new WindowConfig(index,
				Rectf(x, y, x + w, y + h), vec2(-x, -y)));

            window->setBorderless();
            window->setPos(xs, ys);
            window->setSize(ws, hs);
            if (ctx.config.is_fullscreen()) window->setFullScreen();
        }
    }
    
    if (ctx.config.get_window_mode() == WindowConfig::DISPLAY_GRID) {
        int rows = json::get(windowConfig, "rows", 1);
        int cols = json::get(windowConfig, "columns", 1);
		int w = json::get(windowConfig, "screen_width", 960);
		int h = json::get(windowConfig, "screen_height", 540);
		int ws = w * ctx.config.get_app_scale();
		int hs = h * ctx.config.get_app_scale();
        
        int index = 0;
        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                // Calculate the coordinates of each window
                int x = c * w;
                int y = r * h;
				int xs = x * ctx.config.get_app_scale();
				int ys = y * ctx.config.get_app_scale();
                
                #if defined CINDER_MAC
                // This is an ugly hack to account for the OSX toolbar
                if(!ctx.config.is_fullscreen()) {
                    y += 22;
                    ys += 22;
                }
                #endif
                
                WindowRef window = createWindow();
                window->setBorderless();
                window->setSize(ws, hs);
                window->setPos(xs, ys);
                
                window->setUserData(new WindowConfig(index, Rectf(x, y, w, h), vec2(-x, -y)));
                if(ctx.config.is_fullscreen()) window->setFullScreen();
                
                index++;
            }
        }
    }
    
    paramsWindow->setAlwaysOnTop();
}

void MagicWindowApp::draw() {
    WindowRef window = getWindow();
    WindowConfig * data = window->getUserData<WindowConfig>();
    gl::setMatricesWindow(getWindowSize());
    
    if (window == paramsWindow) {
        gl::clear();
        ctx.params->draw();
    }
    else {
        if (data) {
            gl::clear();
            ctx.signals.preDrawTransform.emit();
            gl::pushMatrices();
            gl::scale(ctx.config.get_app_scale(), ctx.config.get_app_scale());
            gl::translate(data->getTranslation());
            ctx.signals.draw.emit();
            gl::popMatrices();
            ctx.signals.postDrawTransform.emit();
            if(ctx.config.bezels_are_visible()) {
                Rectf wb = window->getBounds();
                Rectf bounds = Rectf(wb.getX1(), wb.getY1() + 1, wb.getX2() - 1, wb.getY2());
                gl::color(1, 0, 0);
                gl::pushMatrices();
                gl::drawStrokedRect(bounds);
                gl::popMatrices();
            }
        }
        else {
            gl::clear();
            ctx.signals.draw.emit();
        }
    }
}

void MagicWindowApp::fileDrop(FileDropEvent e) { ctx.signals.fileDrop.emit(e); }

void MagicWindowApp::update() {
    ctx.info.averageFps = getAverageFps();
    if (ctx.config.do_show_params() && ctx.params) {
        ctx.params->setOptions("FPS", "label='FPS: " + toString(getAverageFps()) + "'");
    }
    
    ctx.state.update();
    ctx.signals.update.emit();
}

void MagicWindowApp::keyDown(KeyEvent e) {
    if (ctx.config.default_key_handlers_are_enabled()) {
        switch (e.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        case KeyEvent::KEY_m:
            if (e.isControlDown()) {
                ctx.config.set_cursor_visibility(!ctx.config.do_show_cursor());
                ctx.config.do_show_cursor() ? showCursor() : hideCursor();
            }
            break;
        case KeyEvent::KEY_p:
            if (paramsWindowIsAvailable) {
                paramsWindow->isHidden() ? paramsWindow->show() : paramsWindow->hide();
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
void MagicWindowApp::cleanup() { 
    ctx.signals.cleanup.emit();
    App::cleanup();
}
