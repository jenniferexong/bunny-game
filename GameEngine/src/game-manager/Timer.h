#pragma once

class Timer {
private:
	int frame_count_ = 0;
	float delta_time_= 0.f;
	float update_rate_ = 4.f;
	long long previous_frame_time_;
	long long current_frame_time_;

	long long getCurrentTime();

public:
	Timer();

	float frame_delta = 0.f;
	float fps = 0.f;
	float time = 0.f;

	void update();
	void updateTime();
	void calculateDelta();
	void updateFps();
};
