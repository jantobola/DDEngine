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

void Timer::start() {
	current = time();
	delta = current - previous;
	previous = current;

	if (current - tickTime >= FPS_REFRESH_INTERVAL) {
		FPS = counter * (1000 / FPS_REFRESH_INTERVAL);
		counter = 0;
		tickTime = current;
	}

	counter++;
}

long long DDEngine::Timer::time() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}

int Timer::getFPS() {
	return FPS;
}

float Timer::velocity() {
	return (float) ((double) delta) / 1000.0f;
}