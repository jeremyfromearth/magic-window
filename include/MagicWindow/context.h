#pragma once

#include "cinder/params/Params.h"

// hexa
#include "MagicWindow/config.h"
#include "MagicWindow/signals.h"

namespace magicwindow {
  class context {
  public:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    config config;
    signals signals;
    ci::params::InterfaceGlRef params;
  };
}
