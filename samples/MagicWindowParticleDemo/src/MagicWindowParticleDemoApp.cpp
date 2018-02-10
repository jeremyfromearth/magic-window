// cinder
#include "cinder/app/RendererGl.h"

// magicwindow
#include "magicwindow.h"

#include "particles.h"

using namespace ci::app;

class MagicWindowParticleDemoApp : public magicwindow::app {
public:
    void setup();
    Particles * p;
};

void MagicWindowParticleDemoApp::setup() {
    initialize();
    p = new Particles(ctx);
    p->setup(app_bounds);
    std::cout << app_bounds << std::endl;
}

CINDER_APP(MagicWindowParticleDemoApp, RendererGl)
