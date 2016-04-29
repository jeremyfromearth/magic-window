#pragma once

// cinder
#include "cinder/Function.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/FileDropEvent.h"

// atlas
#include "MagicWindow/WindowConfig.h"

namespace magicwindow {
    class Signals {
    public:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        ci::signals::Signal<void()> update;
        ci::signals::Signal<void()> draw;
        ci::signals::Signal<void()> postDrawTransform;
        ci::signals::Signal<void()> preDrawTransform;
        ci::signals::Signal<void(ci::app::MouseEvent event)> mouseDown;
        ci::signals::Signal<void(ci::app::MouseEvent event)> mouseDrag;
        ci::signals::Signal<void(ci::app::MouseEvent event)> mouseMove;
        ci::signals::Signal<void(ci::app::MouseEvent event)> mouseUp;
        ci::signals::Signal<void(ci::app::MouseEvent event)> mouseWheel;
        ci::signals::Signal<void(ci::app::KeyEvent event)> keyDown;
        ci::signals::Signal<void(ci::app::KeyEvent event)> keyUp;
        ci::signals::Signal<void(ci::app::FileDropEvent e)> fileDrop;
        ci::signals::Signal<void()> cleanup;
        ci::signals::Signal<void()> shutdown;
    };
}
