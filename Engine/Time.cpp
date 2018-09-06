#include "Time.h"

Time* Time::m_instance = nullptr;

Time::Time(){
}


Time::~Time()
{
}

Time* Time::getInstance(){
	return m_instance != nullptr ? m_instance : (m_instance = new Time());
}

int Time::getTotalFrames() {
	return m_frames;
}

double Time::getFPS() {
	return m_frameRate;
}

double Time::getLastFrameTime() {
	double elapsed_secs = double(m_deltaClocks) / CLOCKS_PER_SEC;
	return elapsed_secs;
}

double Time::clockToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

void Time::earlyUpdate() {
	m_earlyClocks = clock();
}

void Time::lateUpdate(){
	
	m_lateClocks = clock();
	m_deltaClocks = m_lateClocks - m_earlyClocks;
	m_dt += getLastFrameTime();
	m_frames++;
	if (m_dt > 1.0f / m_fpsUpdateRate) {
		m_frameRate = m_frames / m_dt;
		m_frames = 0;
		m_dt -= 1.0 / m_fpsUpdateRate;
	}
}

