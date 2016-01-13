#pragma once

// cinder
#include "cinder/Display.h"
#include "cinder/Json.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

// hexa
#include "MagicWindow/Context.h"
#include "MagicWindow/Utils.h"
#include "MagicWindow/WindowConfig.h"

namespace magicwindow {
    class MagicWindowApp : public ci::app::App{
    public:
        ///////////////////////////////////////////////////////////////
        // Methods
        ///////////////////////////////////////////////////////////////
        /**
         * Initializes the application, config and windows
         * assetPath - the path to the folder appended to
         * the native Documents directory containing
         * cfg.json, errors.txt and log.txt
         *
         * returns bool indicating that initialization was successful
         */
        bool initialize(std::string assetPath = "");

        /**
         * Create windows based on config variables. This method must be called by a sub-classing application.
         */
        void initializeWindowConfiguration();

        /**
         * The main update call
         */
        void update();

        /**
         * The main draw call
         */
        void draw();

        /**
         * Override key and mouse events and pass them to the global emitter
         */
        void fileDrop(ci::app::FileDropEvent e);
        void keyDown(ci::app::KeyEvent e);
        void keyUp(ci::app::KeyEvent e);
        void mouseDown(ci::app::MouseEvent e);
        void mouseDrag(ci::app::MouseEvent e);
        void mouseMove(ci::app::MouseEvent e);
        void mouseUp(ci::app::MouseEvent e);
        void mouseWheel(ci::app::MouseEvent e);
        void shutdown();

    protected:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        Context ctx;
    };
}
