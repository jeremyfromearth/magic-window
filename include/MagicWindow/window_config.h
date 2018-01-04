#pragma once

// cinder
#include "cinder/Rect.h"
#include "cinder/gl/gl.h"

namespace magicwindow {
  class window_config {
  public:
    /**
     Use to create a window for each screen
     Example json configuration:
      "window_mode" : "span"
     */
    static const std::string DISPLAY_SPAN;
    
    /**
     Custom configuration using custom x, y width and height for each window
     Example json configuration:
       "window_mode" : "custom",
       "windows" : [{
         "x" : 100,
         "y" : 100,
         "w" : 960,
         "h" : 540
        }, {
         "x" : 1000,
         "y" : 1000,
         "w" : 100,
         "h" : 100
       }]
     */
    static const std::string DISPLAY_CUSTOM;
    
    /**
     Creates an array of windows of uniform size defined by rows, columns and width and height.
     Example json configuration:
       "window_mode" : "grid",
       "windows" : {
         "rows" : 2,
         "columns" : 6,
         "screen_width" : 1024,
         "screen_height" : 512
       }
     */
    static const std::string DISPLAY_GRID;
    
  public:
    window_config(
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
}
