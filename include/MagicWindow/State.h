#pragma once

#include <string>
#include <memory>

namespace magicwindow {
    class State {
    public:
        ///////////////////////////////////////////////////////////////
        // Constructor
        ///////////////////////////////////////////////////////////////
        State(int stateId, std::string stateName);
        
        ///////////////////////////////////////////////////////////////
        // Getter / Setter
        ///////////////////////////////////////////////////////////////
        int getId();
        
        std::string getName();
        
    private:
        int id;
        std::string name;
    };
    typedef std::shared_ptr<State> StateRef;
}