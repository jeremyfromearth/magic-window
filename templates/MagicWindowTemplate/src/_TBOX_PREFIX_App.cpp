// cinder
#include "cinder/app/RendererGl.h"

// magicwindow
#include "magicwindow.h"

using namespace ci::app;

class _TBOX_PREFIX_App : public magicwindow::app {
public:
    void setup();
};

void _TBOX_PREFIX_App::setup() {
    initialize();
}

CINDER_APP(_TBOX_PREFIX_App, RendererGl)
