#pragma once

#include "cinder/params/Params.h"

// hexa
#include "MagicWindow/Config.h"
#include "MagicWindow/Logger.h"
#include "MagicWindow/RuntimeInfo.h"
#include "MagicWindow/StateMachine.h"
#include "MagicWindow/Signals.h"

namespace magicwindow {
    class Context {
    public:
        ///////////////////////////////////////////////////////////////
        // Ctr
        ///////////////////////////////////////////////////////////////
        Context();

        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        Config config;
        Signals signals;
        RuntimeInfo info;
        StateMachine state;
        ci::params::InterfaceGlRef params;
        
    private:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        Logger logger;
    };
}
