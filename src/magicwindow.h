#pragma once

// cinder
#include "cinder/app/App.h"
#include "cinder/app/FileDropEvent.h"
#include "cinder/Color.h"
#include "cinder/Display.h"
#include "cinder/Function.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Log.h"
#include "cinder/Rect.h"

#define MACOS_TOOLBAR_HEIGHT 22

namespace magicwindow {
  ///////////////////////////////////////////////////////////////
  //
  // config
  //
  ///////////////////////////////////////////////////////////////
  class config {
    
  public:
    ///////////////////////////////////////////////////////////////
    // statics
    ///////////////////////////////////////////////////////////////
    static const std::string DISPLAY_CUSTOM;
    static const std::string DISPLAY_GRID;
    static const std::string DISPLAY_SPAN;
    
    ///////////////////////////////////////////////////////////////
    // methods
    ///////////////////////////////////////////////////////////////
    void initialize(ci::JsonTree cfgData);
    
    ///////////////////////////////////////////////////////////////
    // properties
    ///////////////////////////////////////////////////////////////
    ci::JsonTree magic;
    ci::JsonTree windows;
    
    bool bezels;
    bool cursor;
    std::string display;
    bool fullscreen;
    bool keys;
    float scale;
    bool top;
    ci::JsonTree json;
  };
  ///////////////////////////////////////////////////////////////
  //
  // mouse event
  //
  ///////////////////////////////////////////////////////////////
  struct mouse_event {
    ci::app::MouseEvent original;
    ci::vec2 pos;
    
    mouse_event(ci::vec2 pos, ci::app::MouseEvent e) : pos(pos), original(e) { };
  };
  
  ///////////////////////////////////////////////////////////////
  //
  // signals
  //
  ///////////////////////////////////////////////////////////////
  class signals {
  public:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    ci::signals::Signal<void()> update;
    ci::signals::Signal<void()> draw;
    ci::signals::Signal<void()> post_transform_draw;
    ci::signals::Signal<void()> pre_transform_draw;
    ci::signals::Signal<void()> main_update;
    ci::signals::Signal<void(mouse_event e)> mouse_down;
    ci::signals::Signal<void(mouse_event e)> mouse_drag;
    ci::signals::Signal<void(mouse_event e)> mouse_move;
    ci::signals::Signal<void(mouse_event e)> mouse_up;
    ci::signals::Signal<void(ci::app::TouchEvent e)> touches_began;
    ci::signals::Signal<void(ci::app::TouchEvent e)> touches_moved;
    ci::signals::Signal<void(ci::app::TouchEvent e)> touches_ended;
    ci::signals::Signal<void(ci::app::MouseEvent event)> mouse_wheel;
    ci::signals::Signal<void(ci::app::KeyEvent event)> key_down;
    ci::signals::Signal<void(ci::app::KeyEvent event)> key_up;
    ci::signals::Signal<void(ci::app::FileDropEvent e)> file_drop;
    ci::signals::Signal<void()> cleanup;
    ci::signals::Signal<void()> shutdown;
  };
  
  ///////////////////////////////////////////////////////////////
  //
  // info
  //
  ///////////////////////////////////////////////////////////////
  struct appstatus {
    int current_frame;
    float average_fps;
  };
  
  ///////////////////////////////////////////////////////////////
  //
  // context
  //
  ///////////////////////////////////////////////////////////////
  class context {
  public:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    config cfg;
    signals signals;
    appstatus status;
  };
  
  ///////////////////////////////////////////////////////////////
  //
  // window data
  //
  ///////////////////////////////////////////////////////////////
  struct window_data {
    int x;
    int y;
    int h;
    int w;
    int id;
    
    window_data(int x, int y, int w, int h, int id)
      : x(x), y(y), w(w), h(h), id(id){}
  };
  
  ///////////////////////////////////////////////////////////////
  //
  // app
  //
  ///////////////////////////////////////////////////////////////
  class app : public ci::app::App{
  public:
    ///////////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////////
    /**
     * Initializes the application, config and windows
     * configFilename - config file name
     *
     * returns bool indicating that initialization was successful
     */
    bool initialize(std::string config_filename = "cfg.json");
    
    /**
     * Initializes the application, config and windows
     * data - config data as json
     */
    bool initialize(ci::JsonTree data);
    
    /**
     * The main update call
     */
    void update() override;
    
    /**
     * The main draw call
     */
    void draw() override;
    
    /**
     * Override key and mouse events and pass them to the global emitter
     */
    void fileDrop(ci::app::FileDropEvent e) override;
    void keyDown(ci::app::KeyEvent e) override;
    void keyUp(ci::app::KeyEvent e) override;
    void mouseDown(ci::app::MouseEvent e) override;
    void mouseDrag(ci::app::MouseEvent e) override;
    void mouseMove(ci::app::MouseEvent e) override;
    void mouseUp(ci::app::MouseEvent e) override;
    void mouseWheel(ci::app::MouseEvent e) override;
    void touchesBegan(ci::app::TouchEvent e) override;
    void touchesMoved(ci::app::TouchEvent e) override;
    void touchesEnded(ci::app::TouchEvent e) override;
    void cleanup() override;
    
  protected:
    ///////////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////////
    /**
     * Create windows based on config variables. This method must be called by a sub-classing application.
     */
    void magic();
    
    /**
     * Calculate the position of a mouse event globally across all windows
     */
    mouse_event create_mouse_event(ci::app::MouseEvent e);
    
    ci::app::TouchEvent interpolate_touch_event(ci::app::TouchEvent & e);
    
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    context ctx;
    ci::Rectf app_bounds;
    ci::app::WindowRef main_window;
  };
}
