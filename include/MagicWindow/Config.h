#pragma once

// std
#include <string>

// cinder
#include "cinder/Color.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"

namespace magicwindow {
    class Config {

    public:
        ///////////////////////////////////////////////////////////////
        // Getters
        ///////////////////////////////////////////////////////////////
        const ci::JsonTree & getApp() { return appNode; }
        float getAppScale() { return appScale; }
        ci::fs::path getAssetPath() { return assetPath; }
        bool doShowCursor() { return showCursor; }
        bool doShowParams() { return showParams; }
        ci::vec2 getParamWindowCoords() { return paramsWindowCoords; }
        bool getDefaultKeyHandlersEnabled() { return defaultKeyHandlersEnabled; }
        ci::JsonTree & getExtras() { return extras; }
        bool getFullScreen() { return fullscreen; }
        ci::JsonTree getWindowConfig() { return windowConfig; }
        std::string getWindowMode() { return windowMode; }

        ///////////////////////////////////////////////////////////////
        // Setters
        ///////////////////////////////////////////////////////////////
        void setCursorVisibility(bool v) { showCursor = v; }
        void setAssetPath(std::string assetDirPath) { assetPath = ci::fs::path(assetDirPath); }
        void initialize(ci::JsonTree cfgData);
    
    private:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        ci::JsonTree appNode;
        float appScale;
        ci::fs::path assetPath;
        bool showCursor;
        bool showParams;
        ci::vec2 paramsWindowCoords;
        bool defaultKeyHandlersEnabled;
        ci::JsonTree extras;
        bool fullscreen;
        ci::JsonTree windowConfig;
        std::string windowMode;
    };
}

