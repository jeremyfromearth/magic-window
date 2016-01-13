#include "MagicWindow/StateMachine.h"

using namespace magicwindow;

StateMachine::StateMachine() {
    current = -1;
    previous = -1;
    transition = nullptr;
}

int StateMachine::getCurrent() {
    return current;
}

std::string StateMachine::getCurrentName() {
    if(!states.count(current)) return "NONE";
    return states.at(current)->getName();
}

int StateMachine::getPrevious() {
    return previous;
}

std::string StateMachine::getPreviousName() {
    if(!states.count(previous)) return "NONE";
    return states.at(previous)->getName();
}


void StateMachine::addState(int id, std::string name) {
    StateRef state = std::make_shared<State>(id, name);
    if(states.count(id) == 0) states.insert(std::pair<int, StateRef>(id, state));
}

void StateMachine::addTransition(int from, int to, float duration, bool interuptable) {
    if(states.count(from) && states.count(to)) {
        if(!transitions.count(from)) {
            transitions.insert(std::pair<int, std::map<int, TransitionRef>>(from, std::map<int, TransitionRef>()));
        }
        transitions.at(from).insert(
            std::pair<int, TransitionRef>(to, std::make_shared<Transition>(states.at(from), states.at(to), duration, interuptable)));
    } else {
        // To or from state is not valid
    }
}

void StateMachine::printTransitions() {
    for(auto a : transitions) {
        ci::app::console() << "From: " << states.at(a.first)->getName() << std::endl;
        for(auto b : a.second) {
            ci::app::console() << "\tTo: " << states.at(b.first)->getName() << std::endl;
        }
    }
}

bool StateMachine::setState(int nextState) {
    if(transition != nullptr) {
        if(!transition->isInteruptable()) {
            ci::app::console() << "Transition in progress in not interuptable" << std::endl;
            return false;
        }
    }
    
    if(states.count(nextState)) {
        if(nextState != current) {
            if(current == -1) {
                current = nextState;
                ci::app::console() << "Initial state set to: " << states.at(current)->getName() << std::endl;
                return true;
            }
            if(transitions.count(current)) {
                if(transitions.at(current).count(nextState)) {
                    transition = transitions.at(current).at(nextState);
                    transition->reset();
                    onTransitionStart.emit(transition);
                    return true;
                } else {
                    
                    ci::app::console() <<
                    "Transition from: " << states.at(current)->getName() <<
                    " to " << states.at(nextState)->getName() <<
                    " is not allowed." << std::endl;
                    
                }
            } else {
                // No transition from current state
            }
        } else {
            //ci::app::console() << "Transition ignored" << std::endl;
        }
    } else {
        /*
         ci::app::console() <<
         "The attempted transition could not be carried out." \
         "Because the state (" << nextState << ") is not valid" << std::endl;
         */
    }
    return false;
}

void StateMachine::update() {
    if(transition != nullptr) {
        if(transition->isComplete()) {
            previous = current;
            current = transition->getTo()->getId();
            //ci::app::console() << "Transition complete, new state: " << states.at(current)->getName() << std::endl;
            onTransitionComplete.emit(transition);
            transition = nullptr;
        } else {
            // Dispatch transition update
            onTransitionProgress.emit(transition);
        }
    }
}
