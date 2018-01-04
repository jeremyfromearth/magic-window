#pragma once

#include "cinder/app/App.h"

namespace magicwindow {
  class Timer {
  public:
    ///////////////////////////////////////////////////////////////
    // Constructor
    ///////////////////////////////////////////////////////////////
    Timer();
    
    ///////////////////////////////////////////////////////////////
    // Getter / Setter
    ///////////////////////////////////////////////////////////////
    /**
     * Get and set the duration of the stopwatch
     */
    float getDuration();
    void setDuration(float d);
    
    /**
     * Set the duration of the stop watch and reset it
     */
    void setDuration(float d, bool autoReset);
    
    /**
     * Returns a float indicating what percent the stopwatch is complete
     */
    float getPercentComplete();
    
    /**
     * Returns a float indicating the elapsed seconds since the stopwatch was last reset
     */
    float getElapsedSeconds();
    
    /**
     * Returns a boolean indicating whether or not the stopwatch is complete
     */
    bool isComplete();
    
    /**
     * Resets the stopwatch
     */
    void reset();
    
  private:
    float start;
    float duration;
  };
}
