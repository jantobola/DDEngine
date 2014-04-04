#include "Timer.h"

using namespace DDEngine;

Timer::Timer() {
	current = 0;
	previous = 0;
	counter = 0;
	tickTime = 0;
	FPS = 0;
	delta = 1;
}

Timer::~Timer() {

}

void DDEngine::Timer::init() {
	previous = timeGetTime();
	tickTime = previous;
}

void DDEngine::Timer::pause()
{
	delta = 0;
}

void Timer::start() {
	current = timeGetTime();
	delta = current - previous;
	previous = current;

	if (timeGetTime() - tickTime >= FPS_REFRESH_INTERVAL) {
		FPS = counter * (1000 / FPS_REFRESH_INTERVAL);
		counter = 0;
		tickTime = timeGetTime();
	}

	counter++;
}

int Timer::getFPS() {
	return FPS;
}

float Timer::velocity() {
	return (float) ((double) delta) / 1000.0f;
}