#pragma once

// std
#include <string>

// cinder
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Rect.h"

namespace magicwindow {
  class cfg {
  
  public:
    ///////////////////////////////////////////////////////////////
    // Getters
    ///////////////////////////////////////////////////////////////
    const ci::JsonTree & get_magic_window() { return magic_window; }
    float get_app_scale() { return app_scale; }
    bool bezels_are_visible() { return bezels; }
    bool do_show_cursor() { return cursor; }
    bool do_show_params() { return params; }
    ci::vec2 get_param_coords() { return param_coords; }
    bool default_key_handlers_are_enabled() { return default_keys; }
    ci::JsonTree & get_app() { return app; }
    bool is_fullscreen() { return fullscreen; }
    ci::JsonTree get_window_config() { return windows; }
    std::string get_window_mode() { return window_mode; }
    
    ///////////////////////////////////////////////////////////////
    // Setters
    ///////////////////////////////////////////////////////////////
    void set_cursor_visibility(bool v) { cursor = v; }
    void initialize(ci::JsonTree cfgData);
    
  public:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    ci::JsonTree magic_window;
    float app_scale;
    bool bezels;
    bool cursor;
    bool params;
    ci::vec2 param_coords;
    bool default_keys;
    ci::JsonTree app;
    bool fullscreen;
    ci::JsonTree windows;
    std::string window_mode;
    
    class window {
    public:
      static const std::string DISPLAY_CUSTOM;
      static const std::string DISPLAY_GRID;
      static const std::string DISPLAY_SPAN;
      
    public:
      window(
        int window_id,
        ci::Rectf window_bounds,
        ci::vec2 origin_translation) :
      id(window_id),
      bounds(window_bounds),
      translation(origin_translation) {}
      
      int getId() { return id; }
      ci::Rectf get_bounds() { return bounds; }
      ci::vec2 get_translation() { return translation; }
      
    private:
      ci::Rectf bounds;
      int id;
      ci::vec2 translation;
    };
  };
}

