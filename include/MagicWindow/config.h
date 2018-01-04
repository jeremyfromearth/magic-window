#pragma once

// std
#include <string>

// cinder
#include "cinder/Color.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"

namespace magicwindow {
  class config {
    
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
    
  private:
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
  };
}

