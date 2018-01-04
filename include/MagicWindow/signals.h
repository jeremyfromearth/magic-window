#pragma once

// cinder
#include "cinder/Function.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/FileDropEvent.h"

namespace magicwindow {
  class signals {
  public:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    ci::signals::Signal<void()> update;
    ci::signals::Signal<void()> draw;
    ci::signals::Signal<void()> post_transform_draw;
    ci::signals::Signal<void()> pre_transform_draw;
    ci::signals::Signal<void(ci::app::MouseEvent event)> mouse_down;
    ci::signals::Signal<void(ci::app::MouseEvent event)> mouse_drag;
    ci::signals::Signal<void(ci::app::MouseEvent event)> mouse_move;
    ci::signals::Signal<void(ci::app::MouseEvent event)> mouse_up;
    ci::signals::Signal<void(ci::app::MouseEvent event)> mouse_wheel;
    ci::signals::Signal<void(ci::app::KeyEvent event)> key_down;
    ci::signals::Signal<void(ci::app::KeyEvent event)> key_up;
    ci::signals::Signal<void(ci::app::FileDropEvent e)> file_drop;
    ci::signals::Signal<void()> cleanup;
    ci::signals::Signal<void()> shutdown;
  };
}
