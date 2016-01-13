#pragma once

#include "cinder/app/App.h"

namespace magicwindow {
    class Timer {
    public:
        ///////////////////////////////////////////////////////////////
        // Constructor
        ///////////////////////////////////////////////////////////////
        Timer() {
            duration = 0.0f;
            start = ci::app::getElapsedSeconds();
        }
    
        ///////////////////////////////////////////////////////////////
        // Getter / Setter
        ///////////////////////////////////////////////////////////////
        /**
         * Get and set the duration of the stopwatch
         */
        float getDuration() { return duration; }
        void setDuration(float d) { duration = d; }

        /**
         * Set the duration of the stop watch and reset it
         */
        void setDuration(float d, bool autoReset) {
            duration = d;
            if(autoReset) reset();
        }
    
        /**
         * Returns a float indicating what percent the stopwatch is complete
         */
        float getPercentComplete() {
            float current = (float)ci::app::getElapsedSeconds();
            return (current - (float)start) / (float)duration;
        }
    
        /**
         * Returns a float indicating the elapsed seconds since the stopwatch was last reset
         */
        float getElapsedSeconds() {
            float current = ci::app::getElapsedSeconds();
            return current - start;
        }

        /**
         * Returns a boolean indicating whether or not the stopwatch is complete
         */
        bool isComplete() {
            float current = ci::app::getElapsedSeconds();
            return (current - start) > duration;
        }
    
        /**
         * Resets the stopwatch
         */
        void reset() {
            start = ci::app::getElapsedSeconds();
        }
    
    private:
        float start;
        float duration;
    };
}
