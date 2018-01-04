#pragma once

// std
#include <map>
#include <set>

// cinder
#include "cinder/Function.h"

// hexa
#include "MagicWindow/State.h"
#include "MagicWindow/Timer.h"
#include "MagicWindow/Transition.h"

namespace magicwindow {
  class StateMachine {
  public:
    ///////////////////////////////////////////////////////////////
    // Constructor
    ///////////////////////////////////////////////////////////////
    StateMachine();
    
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    
    /**
     * Returns the id of the current state
     */
    int getCurrent();
    
    /**
     * Returns the string representation of the current state
     */
    std::string getCurrentName();
    
    /**
     * Returns the id of the previous state
     */
    int getPrevious();
    
    /**
     * Returns the string representation of the previous state
     */
    std::string getPreviousName();
    
    /**
     * Called when a transition begins
     */
    ci::signals::Signal<void(TransitionRef & t)> onTransitionStart;
    
    /**
     * Called as a transition progresses
     */
    ci::signals::Signal<void(TransitionRef & t)> onTransitionProgress;
    
    /**
     * Called when a transition is complete
     */
    ci::signals::Signal<void(TransitionRef & t)> onTransitionComplete;
    
    ///////////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////////
    
    /**
     * Add a state to this state machine
     */
    void addState(int id, std::string name);
    
    /**
     * Add a transition between two states
     * Optionally supply a duration and a flag indicating whether or not the transition can be interupted
     */
    void addTransition(int from, int to, float duration = 0.0f, bool interuptable = false);
    
    /**
     * Prints all states and the states that can be transitioned to
     */
    void printTransitions();
    
    /**
     * Sets the current state
     */
    bool setState(int nextState);
    
    /**
     * Updates the state machine
     */
    void update();
    
  protected:
    ///////////////////////////////////////////////////////////////
    // Properties
    ///////////////////////////////////////////////////////////////
    int current;
    int previous;
    TransitionRef transition;
    std::map<int, StateRef> states;
    std::map<int, std::map<int, TransitionRef>> transitions;
  };
}
