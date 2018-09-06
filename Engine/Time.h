#pragma once
#include <time.h>

#define TIME Time::getInstance()

class Time {
private:
	static Time* m_instance;
	clock_t m_earlyClocks, m_lateClocks, m_deltaClocks;
	int m_frames = 0;
	float m_fpsUpdateRate = 10.0f;
	double m_frameRate = 30;
	double m_dt = 0;

	double clockToMilliseconds(clock_t clocks);

public:
	Time();
	~Time();
	static Time* getInstance();

	int getTotalFrames();
	double getFPS();	
	double getLastFrameTime();
	void earlyUpdate();
	void lateUpdate();

};

