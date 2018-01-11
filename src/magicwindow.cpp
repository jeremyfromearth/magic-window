#include "magicwindow.h"

using namespace ci;
using namespace ci::app;
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
  scale = cfg.hasChild("scale") ? cfg.getChild("scale").getValue<float>() : 1.0;
  top = cfg.hasChild("top") ? cfg.getChild("top").getValue<bool>() : false;
  windows = cfg.getChild("windows");
}

void magicwindow::app::cleanup() {
  ctx.signals.cleanup.emit();
  App::cleanup();
}

void magicwindow::app::draw() {
  WindowRef window = getWindow();
  
  window_data * data = window->getUserData<window_data>();
  if(data) {
    gl::clear();
    ctx.signals.pre_transform_draw.emit();
    gl::pushMatrices();
    if(ctx.cfg.fullscreen) {
      gl::translate(-data->x, -data->y);
    } else {
      gl::scale(ctx.cfg.scale, ctx.cfg.scale);
      gl::translate(-vec2(data->x, data->y) * (1.0f / ctx.cfg.scale));
    }
    ctx.signals.draw.emit();
    gl::popMatrices();
    ctx.signals.post_transform_draw.emit();
    
    if(ctx.cfg.bezels) {
      Rectf wb = window->getBounds();
      Rectf bounds = Rectf(wb.getX1(), wb.getY1() + 1, wb.getX2() - 1, wb.getY2());
      gl::ScopedColor color(0, 1, 1);
      gl::ScopedMatrices m2;
      gl::drawStrokedRect(bounds);
    }
  } else {
    ctx.signals.draw.emit();
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
  
  if(!main_window) {
    main_window = getWindow();
    ctx.status.current_frame = 0;
    main_window->getSignalPostDraw().connect([&] {
      ctx.signals.main_update.emit();
      ctx.status.current_frame++;
      ctx.status.average_fps = getAverageFps();
    });
  }
  
  // A window for each display with width and height matching the display
  if (ctx.cfg.display == config::DISPLAY_SPAN) {
    std::vector<DisplayRef> displays = Display::getDisplays();
    for (int i = 0; i < displays.size(); i++) {
      Rectf bounds = displays[i]->getBounds();
      WindowRef window = i == 0 ? main_window : createWindow();
      if(ctx.cfg.top) window->setAlwaysOnTop();
      if(ctx.cfg.fullscreen) {
        window->setPos(bounds.getUpperLeft());
        window->setFullScreen();
      } else {
        window->setSize(bounds.getSize() * ctx.cfg.scale);
        window->setBorderless();
        window->setPos(bounds.getUpperLeft() * ctx.cfg.scale);
      }
    }
  }
  
  // As many windows as defined in the window_config variable
  if (ctx.cfg.display == config::DISPLAY_CUSTOM) {
    for (JsonTree::Iter windowIt = window_cfg.begin(); windowIt != window_cfg.end(); windowIt++) {
      int x = windowIt->getChild("x").getValue<int>();
      int y = windowIt->getChild("y").getValue<int>();
      int w = windowIt->getChild("w").getValue<int>();
      int h = windowIt->getChild("h").getValue<int>();
      
      WindowRef window = windowIt == window_cfg.begin() ? main_window : createWindow();
      if(ctx.cfg.top) window->setAlwaysOnTop();
      if(ctx.cfg.fullscreen) {
        window->setPos(x, y);
        window->setFullScreen();
      } else {
        int xs = x * app_scale;
        int ys = y * app_scale;
        int ws = w * app_scale;
        int hs = h * app_scale;
        window->setSize(ws, hs);
        window->setBorderless();
        window->setPos(xs, ys);
      }
    }
  }
  
  if (ctx.cfg.display == config::DISPLAY_GRID) {
    float rows = window_cfg.hasChild("rows") ? window_cfg.getChild("rows").getValue<float>() : 1;
    float cols = window_cfg.hasChild("columns") ? window_cfg.getChild("columns").getValue<float>() : 1;
    
    if(rows <= 0 || cols <= 0) throw std::runtime_error("Rows and columns must be greater than 1");
    
    float w = window_cfg.hasChild("screen_width") ? window_cfg.getChild("screen_width").getValue<float>() : 960;
    float h = window_cfg.hasChild("screen_height") ? window_cfg.getChild("screen_height").getValue<float>() : 540;
    float ws = w * ctx.cfg.scale;
    float hs = h * ctx.cfg.scale;
    
    int id = 0;
    for(int r = 0; r < rows; r++) {
      for(int c = 0; c < cols; c++) {
        // Calculate the coordinates of each window
        int x = c * w;
        int y = r * h;
        x *= ctx.cfg.scale;
        y *= ctx.cfg.scale;

#if defined CINDER_MAC
        // This is an ugly hack to account for the OSX toolbar
        // Because regardless of whether or not we set the window.y to 0, it will be bumbped down by the toolbar
        if(!ctx.cfg.fullscreen && !ctx.cfg.top) y += 22;
#endif
        // Odering of the following is very important
        WindowRef window = r == 0 && c == 0 ? main_window : createWindow();
        window->setUserData(new window_data(x, y, ws, hs, id));
        if(ctx.cfg.top) window->setAlwaysOnTop();
        if(ctx.cfg.fullscreen) {
          window->setPos(x, y);
          window->setFullScreen();
        } else {
          window->setBorderless();
          window->setSize(ws, hs);
          window->setPos(x, y);
        }
        id++;
      }
    }
  }
}

  // TODO Wrap these mouse events in a magicwindow mouse event that translates the event coordinates to the appropriate window
void magicwindow::app::mouseDown(MouseEvent e) { ctx.signals.mouse_down.emit(e); }
void magicwindow::app::mouseDrag(MouseEvent e) { ctx.signals.mouse_drag.emit(e); }
void magicwindow::app::mouseMove(MouseEvent e) { ctx.signals.mouse_move.emit(e); }
void magicwindow::app::mouseUp(MouseEvent e) { ctx.signals.mouse_up.emit(e); }
void magicwindow::app::mouseWheel(MouseEvent e) { ctx.signals.mouse_wheel.emit(e); }

void magicwindow::app::update() {
  ctx.signals.update.emit();
}
