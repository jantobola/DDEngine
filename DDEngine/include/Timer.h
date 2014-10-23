#pragma once

#include <windows.h>

#define FPS_REFRESH_INTERVAL 1000

namespace DDEngine
{
class Timer {

	private:
		long long current;
		long long previous;

		int counter;
		long long tickTime;
		int FPS;

		long long time();

	public:
		long long delta;

		Timer();
		~Timer();

		void init();
		void start();
		void pause();

		int getFPS();
		float velocity();

};
}