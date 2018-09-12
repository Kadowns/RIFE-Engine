#pragma once
#include <time.h>

#define TIME Clock::getInstance()

class Clock {
private:
	static Clock* m_instance;
	clock_t m_earlyClocks, m_lateClocks, m_deltaClocks;
	int m_frames = 0;
	float m_fpsUpdateRate = 4.0f;
	double m_frameRate = 30;
	double m_dt = 0;

	double clockToMilliseconds(clock_t clocks);

public:
	Clock();
	~Clock();
	static Clock* getInstance();

	int getTotalFrames();
	double getFPS();	
	double getLastFrameTime();
	void earlyUpdate();
	void lateUpdate();

};

