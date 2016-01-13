#include "MagicWindow/Controller.h"

using namespace magicwindow;

/**
 * Constructor
 */
Controller::Controller(Context & context) : ctx(context) {}

void Controller::connectDrawSignal() {
    drawConnection = ctx.signals.draw.connect([this] {
         draw();
     });
}

void Controller::connectFrameSignals() {
    connectUpdateSignal();
    connectDrawSignal();
}

void Controller::connectUpdateSignal() {
    updateConnection = ctx.signals.update.connect([this] {
        update();
    });
}

void Controller::disconnectDrawSignal() {
     if (drawConnection.isConnected()){
         drawConnection.disconnect();
     }
 }

void Controller::disconnectFrameSignals() {
    disconnectUpdateSignal();
    disconnectDrawSignal();
}

void Controller::disconnectUpdateSignal() {
    if (updateConnection.isConnected()) {
        updateConnection.disconnect();
    }
}
