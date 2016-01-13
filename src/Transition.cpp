#include "MagicWindow/Transition.h"

using namespace magicwindow;

Transition::Transition(StateRef fromState, StateRef toState, double duration, bool interupt) :
from(fromState), to(toState), interuptable(interupt) {
    timer.setDuration(duration);
}

float Transition::getDuration() { return timer.getDuration(); }

float Transition::getElapsedSeconds() { return timer.getElapsedSeconds(); }

float Transition::getPercentComplete() { return timer.getPercentComplete(); }

StateRef Transition::getFrom() { return from; }

StateRef Transition::getTo() { return to; }

bool Transition::isComplete() { return timer.isComplete(); }

bool Transition::isInteruptable() { return interuptable; }

void Transition::setIsInteruptable(bool interupt) {
    interuptable = interupt;
}

void Transition::reset() {
    timer.reset();
}
