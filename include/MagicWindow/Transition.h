#pragma once

#include "MagicWindow/State.h"
#include "MagicWindow/Timer.h"

namespace magicwindow {
  class Transition {
  public:
    ///////////////////////////////////////////////////////////////
    // Constructor
    ///////////////////////////////////////////////////////////////
    Transition(StateRef fromState, StateRef toState, double duration = 0.0f, bool interupt = false);
    
    ///////////////////////////////////////////////////////////////
    // Getter / Setter
    ///////////////////////////////////////////////////////////////
    float getDuration();
    
    float getElapsedSeconds();
    
    float getPercentComplete();
    
    StateRef getFrom();
    
    StateRef getTo();
    
    bool isComplete();
    
    bool isInteruptable();
    void setIsInteruptable(bool interupt);
    
    ///////////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////////
    void reset();
    
  private:
    StateRef from;
    StateRef to;
    Timer timer;
    bool interuptable;
  };
  typedef std::shared_ptr<Transition> TransitionRef;
}
