// cinder
#include "cinder/app/RendererGl.h"

// magicwindow
#include "MagicWindow/MagicWindowApp.h"

using namespace ci::app;
using namespace magicwindow;

class _TBOX_PREFIX_App : public MagicWindowApp {
public:
    void setup();
};

void _TBOX_PREFIX_App::setup() {
    initialize();
}

CINDER_APP(_TBOX_PREFIX_App, RendererGl)
