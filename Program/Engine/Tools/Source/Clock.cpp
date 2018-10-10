#include <Clock.h>

Clock* Clock::s_instance = nullptr;

Clock::Clock(){
}


Clock::~Clock()
{
}

Clock* Clock::getInstance(){
    return s_instance != nullptr ? s_instance : (s_instance = new Clock());
}

int Clock::getTotalFrames() {
    return m_frames;
}

double Clock::time() {
    return m_time;
}

double Clock::getFPS() {
    return m_frameRate;
}

double Clock::getLastFrameTime() {
    return m_lastFrameTime;
}

double Clock::clockToMilliseconds(clock_t ticks) {
    return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

void Clock::earlyUpdate() {
    m_earlyClocks = clock();
}

void Clock::lateUpdate(){
    
    m_lateClocks = clock();
    m_deltaClocks = m_lateClocks - m_earlyClocks;

    m_lastFrameTime = double(m_deltaClocks) / CLOCKS_PER_SEC;
    m_dt += m_lastFrameTime;
    m_time += m_lastFrameTime;
    m_frames++;
    if (m_dt > 1.0f / m_fpsUpdateRate) {
        m_frameRate = m_frames / m_dt;
        m_frames = 0;
        m_dt -= 1.0 / m_fpsUpdateRate;
    }
}

