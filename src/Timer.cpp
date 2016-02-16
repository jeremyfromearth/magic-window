#include "MagicWindow/Timer.h"

using namespace magicwindow;

Timer::Timer() {
	duration = 0.0f;
	start = ci::app::getElapsedSeconds();
}

float Timer::getDuration() { return duration; }
void Timer::setDuration(float d) { duration = d; }

void Timer::setDuration(float d, bool autoReset) {
	 duration = d;
	 if (autoReset) reset();
 }

float Timer::getPercentComplete() {
	float current = (float)ci::app::getElapsedSeconds();
	return (current - (float)start) / (float)duration;
}

float Timer::getElapsedSeconds() {
	float current = ci::app::getElapsedSeconds();
	return current - start;
}

bool Timer::isComplete() {
	float current = ci::app::getElapsedSeconds();
	return (current - start) > duration;
}

void Timer::reset() {
	 start = ci::app::getElapsedSeconds();
 }