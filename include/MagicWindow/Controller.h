#pragma once

#include "MagicWindow/Context.h"

namespace magicwindow {
    class Controller {
    public:
        
        Controller(Context & context);

    protected:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        Context & ctx;
        ci::signals::Connection drawConnection;
        ci::signals::Connection updateConnection;

        ///////////////////////////////////////////////////////////////
        // Methods
        ///////////////////////////////////////////////////////////////

        /**
         * Connects the draw signal of the applicaiton frame loop
         */
        void connectDrawSignal();

        /**
         * Connects the update and draw signals of the applicaiton frame loop
         */
        void connectFrameSignals();

        /**
         * Connects the update signal of the applicaiton frame loop
         */
        void connectUpdateSignal();

        /**
         * Disconnects the draw signal of the applicaiton frame loop
         */
        void disconnectDrawSignal();

        /**
         * Disconnects both the update and draw signals of the application frame loop
         */
        void disconnectFrameSignals();

        /**
         * Disconnects the update signal of the applicaiton frame loop
         */
        void disconnectUpdateSignal();

        /**
         * Called from the handler for the draw signal
         */
        virtual void draw(){};

        /**
         * Called from the handler for the update signal
         */
        virtual void update(){};
    };
}
