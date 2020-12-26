#include "Timer.h"

#include <cmath>
#include <chrono>

Timer::Timer()
{
	previous_frame_time_ = getCurrentTime();
	current_frame_time_ = getCurrentTime();
}

using namespace std::chrono;
long long Timer::getCurrentTime()
{
	milliseconds ms = duration_cast<milliseconds> (
		system_clock::now().time_since_epoch()
	);
	return ms.count();
}

void Timer::update()
{
	previous_frame_time_ = current_frame_time_;
}

void Timer::updateTime()
{
	time += frame_delta * 1000 * seconds_per_hour;
	time = fmod(time, 24000.f);
}

void Timer::calculateDelta()
{
	current_frame_time_ = getCurrentTime();
	// in seconds
	frame_delta = float(current_frame_time_ - previous_frame_time_) / 1000.f; 
}

void Timer::updateFps()
{
	frame_count_++;
	delta_time_ += frame_delta;
	if (delta_time_ > 1.f / update_rate_) {
		fps = (float) frame_count_ / delta_time_;
		frame_count_ = 0;
		delta_time_ -= 1.f / update_rate_;
	}
}

