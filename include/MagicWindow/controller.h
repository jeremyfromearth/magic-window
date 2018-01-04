#pragma once
#include "MagicWindow/app.h"

namespace magicwindow {
  class controller {
  public:
    
    controller(context & context);
    
  protected:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    context & ctx;
    ci::signals::Connection draw_connection;
    ci::signals::Connection update_connection;
    
    ///////////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////////
    
    /**
     * Connects the draw signal of the applicaiton frame loop
     */
    void connect_draw_signal();
    
    /**
     * Connects the update and draw signals of the applicaiton frame loop
     */
    void connect_frame_signals();
    
    /**
     * Connects the update signal of the applicaiton frame loop
     */
    void connect_update_signal();
    
    /**
     * Disconnects the draw signal of the applicaiton frame loop
     */
    void disconnect_draw_signal();
    
    /**
     * Disconnects both the update and draw signals of the application frame loop
     */
    void disconnect_frame_signals();
    
    /**
     * Disconnects the update signal of the applicaiton frame loop
     */
    void disconnect_update_signal();
    
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
