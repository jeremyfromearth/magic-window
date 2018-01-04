#include "MagicWindow/magic_window_app.h"

using namespace ci;
using namespace ci::app;
using namespace ci::params;
using namespace magicwindow;

//
// Public Methods
//

bool magic_window_app::initialize(JsonTree data) {
  try {
    ctx.config.initialize(data);
  }
  catch (std::exception exc) {
    CI_LOG_EXCEPTION("Could not initialize.", exc);
    return false;
  }
  
  ctx.config.do_show_cursor() ? showCursor() : hideCursor();
  initialize_window_configuration();
  return true;
}

bool magic_window_app::initialize(std::string config_filename)  {
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

void magic_window_app::initialize_window_configuration() {
  
  float app_scale = ctx.config.get_app_scale();
  JsonTree window_cfg = ctx.config.get_window_config();
  
  params_window = getWindow();
  params_window->setUserData(new window_config(-1, Rectf(), vec2()));
  params_window->setSize(500, 300);
  params_window->setPos(ctx.config.get_param_coords());
  ctx.params = InterfaceGl::create(params_window, "Debug Params", vec2(470, 270));
  ctx.params->addText("FPS", "label='FPS should display here'");
  ctx.params->addSeparator();
  params_window_is_available = true;
  if (!ctx.config.do_show_params()) params_window->hide();
  params_window->getSignalClose().connect([&] {
    params_window_is_available = false;
  });
  
  // A window for each display with width and height matching the display
  if (ctx.config.get_window_mode() == window_config::DISPLAY_SPAN) {
    std::vector<DisplayRef> displays = Display::getDisplays();
    for (int i = 0; i < displays.size(); i++) {
      Rectf bounds = displays[i]->getBounds();
      WindowRef window = createWindow();
      window->setUserData(new window_config(i, bounds, vec2()));
      window->setPos(bounds.getUpperLeft());
      window->setSize(bounds.getSize());
      window->setBorderless();
      if (ctx.config.is_fullscreen()) window->setFullScreen();
    }
  }
  
  
  // As many windows as defined in the window_config variable
  if (ctx.config.get_window_mode() == window_config::DISPLAY_CUSTOM) {
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
      
#if defined CINDER_MAC
      // This is an ugly hack to account for the OSX toolbar
      if(!ctx.config.is_fullscreen()) {
        y += 22;
        ys += 22;
      }
#endif
      
      WindowRef window = createWindow();
      window->setUserData(
        new window_config(index,
        Rectf(x, y, x + w, y + h), vec2(-x, -y)));
      
      window->setBorderless();
      window->setPos(xs, ys);
      window->setSize(ws, hs);
      if (ctx.config.is_fullscreen()) window->setFullScreen();
    }
  }
  
  if (ctx.config.get_window_mode() == window_config::DISPLAY_GRID) {
    int rows = json::get(window_cfg, "rows", 1);
    int cols = json::get(window_cfg, "columns", 1);
    int w = json::get(window_cfg, "screen_width", 960);
    int h = json::get(window_cfg, "screen_height", 540);
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
        
        window->setUserData(new window_config(index, Rectf(x, y, w, h), vec2(-x, -y)));
        if(ctx.config.is_fullscreen()) window->setFullScreen();
        
        index++;
      }
    }
  }
  
  params_window->setAlwaysOnTop();
}

void magic_window_app::draw() {
  WindowRef window = getWindow();
  window_config * data = window->getUserData<window_config>();
  gl::setMatricesWindow(getWindowSize());
  
  if (window == params_window) {
    gl::clear();
    ctx.params->draw();
  }
  else {
    if (data) {
      gl::clear();
      ctx.signals.pre_transform_draw.emit();
      gl::pushMatrices();
      gl::scale(ctx.config.get_app_scale(), ctx.config.get_app_scale());
      gl::translate(data->get_translation());
      ctx.signals.draw.emit();
      gl::popMatrices();
      ctx.signals.post_transform_draw.emit();
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

void magic_window_app::fileDrop(FileDropEvent e) { ctx.signals.file_drop.emit(e); }

void magic_window_app::update() {
  if (ctx.config.do_show_params() && ctx.params) {
    ctx.params->setOptions("FPS", "label='FPS: " + toString(getAverageFps()) + "'");
  }
  
  ctx.signals.update.emit();
}

void magic_window_app::keyDown(KeyEvent e) {
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
        if (params_window_is_available) {
          params_window->isHidden() ? params_window->show() : params_window->hide();
        }
        break;
    }
  }
  ctx.signals.key_down.emit(e);
}

void magic_window_app::keyUp(KeyEvent e) { ctx.signals.key_up.emit(e); }
void magic_window_app::mouseDown(MouseEvent e) { ctx.signals.mouse_down.emit(e); }
void magic_window_app::mouseDrag(MouseEvent e) { ctx.signals.mouse_drag.emit(e); }
void magic_window_app::mouseMove(MouseEvent e) { ctx.signals.mouse_move.emit(e); }
void magic_window_app::mouseUp(MouseEvent e) { ctx.signals.mouse_up.emit(e); }
void magic_window_app::mouseWheel(MouseEvent e) { ctx.signals.mouse_wheel.emit(e); }
void magic_window_app::cleanup() { 
  ctx.signals.cleanup.emit();
  App::cleanup();
}
