#pragma once
#include <chrono>

#define TIME Clock::getInstance()

class Clock {
private:
    static Clock* s_instance;
    std::chrono::time_point<std::chrono::steady_clock> m_startTime, m_endTime;
    int m_frames = 0;
    float m_fpsUpdateRate = 1.5f;
    double m_frameRate = 30;
    double m_time = 0;
    double m_lastFrameTime;
    double m_dt = 0;
    
public:
    Clock();
    ~Clock();
    
    static Clock* getInstance();

    int getTotalFrames();
    double time();
    double getFPS();	
    double getLastFrameTime();
    void earlyUpdate();
    void lateUpdate();

};

