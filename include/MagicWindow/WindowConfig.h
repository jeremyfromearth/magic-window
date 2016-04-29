#pragma once

// cinder
#include "cinder/Rect.h"
#include "cinder/gl/gl.h"

namespace magicwindow {
    class WindowConfig {
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
        WindowConfig(int windowId, ci::Rectf windowBounds, ci::vec2 originTranslation) :
            id(windowId), bounds(windowBounds), translation(originTranslation) {}

        int getId() { return id; }
        ci::Rectf getBounds() { return bounds; }
        ci::vec2 getTranslation() { return translation; }

    private:
        ci::Rectf bounds;
        int id;
        ci::vec2 translation;
    };
}
