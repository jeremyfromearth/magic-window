// cinder
#include "cinder/app/RendererGl.h"

// magicwindow
#include "magicwindow.h"

using namespace ci::app;
using namespace magicwindow;

class MagicWindowBasicApp : public magicwindow::app {
public:
    void setup();
};

void MagicWindowBasicApp::setup() {
    initialize();
}

CINDER_APP(MagicWindowBasicApp, RendererGl)
