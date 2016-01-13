// cinder
#include "cinder/app/RendererGl.h"

// magicwindow
#include "MagicWindow/MagicWindowApp.h"

using namespace ci::app;
using namespace magicwindow;

class MagicWindowBasicApp : public MagicWindowApp {
public:
    void setup();
};

void MagicWindowBasicApp::setup() {
    initialize();
}

CINDER_APP(MagicWindowBasicApp, RendererGl)
