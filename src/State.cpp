#include "MagicWindow/State.h"

using namespace std;
using namespace magicwindow;

State::State(int stateId, std::string stateName) : name(stateName), id(stateId) {}

int State::getId(){ return id; }

string State::getName()  { return name; }
