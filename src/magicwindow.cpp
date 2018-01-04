#include "magicwindow.h"

using namespace ci;
using namespace ci::app;
using namespace ci::params;
using namespace magicwindow;

const std::string config::DISPLAY_SPAN = "span";
const std::string config::DISPLAY_CUSTOM = "custom";
const std::string config::DISPLAY_GRID = "grid";

void config::initialize(JsonTree cfg) {
  bezels = cfg.hasChild("bezels") ? cfg.getChild("bezels").getValue<bool>() : false;
  cursor = cfg.hasChild("cursor") ? cfg.getChild("cursor").getValue<bool>() : true;
  display = cfg.hasChild("display") ? cfg.getChild("display").getValue<std::string>() : config::DISPLAY_SPAN;
  fullscreen = cfg.hasChild("fullscreen") ? cfg.getChild("fullscreen").getValue<bool>() : true;
  keys = cfg.hasChild("keys") ? cfg.getChild("keys").getValue<bool>() : true;
  params = cfg.hasChild("params") ? cfg.getChild("params").getValue<bool>() : true;
  scale = cfg.hasChild("scale") ? cfg.getChild("scale").getValue<float>() : 1.0;
  windows = cfg.getChild("windows");
}

void magicwindow::app::cleanup() {
  ctx.signals.cleanup.emit();
  App::cleanup();
}

void magicwindow::app::draw() {
  WindowRef window = getWindow();
  gl::setMatricesWindow(getWindowSize());
  float inverse_scale = 1.0 / ctx.cfg.scale;
  
  if (window == params_window) {
    gl::clear();
    ctx.params->draw();
  }
  else {
    Rectf scaled_bounds = Rectf(window->getBounds());
    scaled_bounds.scale(inverse_scale);
    
    vec2 scaled_pos = window->getPos();
    scaled_pos *= inverse_scale;
    
    gl::clear();
    ctx.signals.pre_transform_draw.emit();
    gl::pushMatrices();
    gl::scale(ctx.cfg.scale, ctx.cfg.scale);
    gl::translate(-scaled_pos);
    ctx.signals.draw.emit();
    gl::popMatrices();
    ctx.signals.post_transform_draw.emit();
    if(ctx.cfg.bezels) {
      Rectf wb = window->getBounds();
      Rectf bounds = Rectf(wb.getX1(), wb.getY1() + 1, wb.getX2() - 1, wb.getY2());
      gl::color(1, 0, 0);
      gl::pushMatrices();
      gl::drawStrokedRect(bounds);
      gl::popMatrices();
    }
    else {
      gl::clear();
      ctx.signals.draw.emit();
    }
  }
}

void magicwindow::app::fileDrop(FileDropEvent e) { ctx.signals.file_drop.emit(e); }

bool magicwindow::app::initialize(JsonTree data) {
  try {
    ctx.cfg.initialize(data);
  }
  catch (std::exception exc) {
    CI_LOG_EXCEPTION("Could not initialize.", exc);
    return false;
  }
  
  ctx.cfg.cursor ? showCursor() : hideCursor();
  magic();
  return true;
}

bool magicwindow::app::initialize(std::string config_filename)  {
  DataSourceRef cfg_data;
  try {
    cfg_data = loadAsset(config_filename);
  }
  catch (AssetLoadExc exc) {
    CI_LOG_EXCEPTION("Could not load config file. Goodbye.", exc);
    return false;
  }
  
  try {
    return initialize(JsonTree(cfg_data));
  }
  catch (std::exception exc) {
    CI_LOG_EXCEPTION("Could not parse the config file. Adios.", exc);
    return false;
  }
}

void magicwindow::app::keyDown(KeyEvent e) {
  if (ctx.cfg.keys) {
    switch (e.getCode()) {
      case KeyEvent::KEY_ESCAPE:
        quit();
        break;
      case KeyEvent::KEY_m:
        if (e.isControlDown()) {
          ctx.cfg.cursor = !ctx.cfg.cursor;
          ctx.cfg.cursor ? showCursor() : hideCursor();
        }
        break;
      case KeyEvent::KEY_p:
        if (params_window_is_available) {
          params_window->isHidden() ? params_window->show() : params_window->hide();
        }
        break;
    }
  }
  ctx.signals.key_down.emit(e);
}

void magicwindow::app::keyUp(KeyEvent e) {
  ctx.signals.key_up.emit(e);
}

void magicwindow::app::magic() {
  
  float app_scale = ctx.cfg.scale;
  JsonTree & window_cfg = ctx.cfg.windows;
  
  params_window = getWindow();
  params_window->setSize(500, 300);
  
  ctx.params = InterfaceGl::create(params_window, "Debug Params", vec2(470, 270));
  ctx.params->addText("FPS", "label='FPS should display here'");
  ctx.params->addSeparator();
  params_window_is_available = true;
  if (!ctx.cfg.params) params_window->hide();
  params_window->getSignalClose().connect([&] {
    params_window_is_available = false;
  });
  
  // A window for each display with width and height matching the display
  if (ctx.cfg.display == config::DISPLAY_SPAN) {
    std::vector<DisplayRef> displays = Display::getDisplays();
    for (int i = 0; i < displays.size(); i++) {
      Rectf bounds = displays[i]->getBounds();
      WindowRef window = createWindow();
      window->setPos(bounds.getUpperLeft());
      window->setSize(bounds.getSize());
      window->setBorderless();
      if (ctx.cfg.fullscreen) window->setFullScreen();
    }
  }
  
  
  // As many windows as defined in the window_config variable
  if (ctx.cfg.display == config::DISPLAY_CUSTOM) {
    for (JsonTree::Iter windowIt = window_cfg.begin(); windowIt != window_cfg.end(); windowIt++) {
      double index = std::distance(window_cfg.begin(), windowIt);
      
      int x = windowIt->getChild("x").getValue<int>();
      int y = windowIt->getChild("y").getValue<int>();
      int w = windowIt->getChild("w").getValue<int>();
      int h = windowIt->getChild("h").getValue<int>();
      int xs = x * app_scale;
      int ys = y * app_scale;
      int ws = w * app_scale;
      int hs = h * app_scale;
      
      WindowRef window = createWindow();
      
      window->setBorderless();
      window->setPos(xs, ys);
      window->setSize(ws, hs);
      if (ctx.cfg.fullscreen) window->setFullScreen();
    }
  }
  
  if (ctx.cfg.display == config::DISPLAY_GRID) {
    float rows = window_cfg.hasChild("rows") ? window_cfg.getChild("rows").getValue<float>() : 1;
    float cols = window_cfg.hasChild("columns") ? window_cfg.getChild("columns").getValue<float>() : 1;
    float w = window_cfg.hasChild("screen_width") ? window_cfg.getChild("screen_width").getValue<float>() : 960;
    float h = window_cfg.hasChild("screen_height") ? window_cfg.getChild("screen_height").getValue<float>() : 540;
    float ws = w * ctx.cfg.scale;
    float hs = h * ctx.cfg.scale;
    
    int index = 0;
    for(int r = 0; r < rows; r++) {
      for(int c = 0; c < cols; c++) {
        // Calculate the coordinates of each window
        int x = c * w;
        int y = r * h;
        int xs = x * ctx.cfg.scale;
        int ys = y * ctx.cfg.scale;


#if defined CINDER_MAC
        // This is an ugly hack to account for the OSX toolbar
        if(!ctx.cfg.fullscreen) {
          y += 22;
          ys += 22;
        }
#endif

        WindowRef window = createWindow();
        window->setBorderless();
        window->setSize(ws, hs);
        window->setPos(xs, ys);
        if(ctx.cfg.fullscreen) window->setFullScreen();
        index++;
      }
    }
  }
  
  params_window->setAlwaysOnTop();
}

  // TODO Wrap these mouse events in a magicwindow mouse event that translates the event coordinates to the appropriate window
void magicwindow::app::mouseDown(MouseEvent e) { ctx.signals.mouse_down.emit(e); }
void magicwindow::app::mouseDrag(MouseEvent e) { ctx.signals.mouse_drag.emit(e); }
void magicwindow::app::mouseMove(MouseEvent e) { ctx.signals.mouse_move.emit(e); }
void magicwindow::app::mouseUp(MouseEvent e) { ctx.signals.mouse_up.emit(e); }
void magicwindow::app::mouseWheel(MouseEvent e) { ctx.signals.mouse_wheel.emit(e); }

void magicwindow::app::update() {
  if (ctx.cfg.params && ctx.params) {
    ctx.params->setOptions("FPS", "label='FPS: " + toString(getAverageFps()) + "'");
  }
  
  ctx.signals.update.emit();
}
