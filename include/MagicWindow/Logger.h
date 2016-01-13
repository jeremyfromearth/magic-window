#pragma once

// std
#include <iostream>
#include <fstream>
#include <string>

// boost
#include <boost/date_time/posix_time/posix_time.hpp>

// cinder
#include "cinder/app/App.h"

// hexa
#include "MagicWindow/Signals.h"

namespace magicwindow {
    class Logger {
    public:
        ///////////////////////////////////////////////////////////////
        // Ctr
        ///////////////////////////////////////////////////////////////
        Logger(Signals & signals);

    private:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        int maxLines;

        ///////////////////////////////////////////////////////////////
        // Methods
        ///////////////////////////////////////////////////////////////
        void clearFile(std::string filename);

        void log(std::string filename, std::string message, bool printToConsole);
    };
}
    
