#pragma once

#include <windows.h>

#define FPS_REFRESH_INTERVAL 1000

namespace DDEngine
{
class Timer {

	private:
		long current;
		long previous;

		int counter;
		long tickTime;
		int FPS;

	public:
		long delta;

		Timer();
		~Timer();

		void init();
		void start();
		void pause();

		int getFPS();
		float velocity();

};
}