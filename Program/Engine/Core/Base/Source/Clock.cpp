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



void Clock::earlyUpdate() {
    m_startTime = std::chrono::high_resolution_clock::now();
}

void Clock::lateUpdate(){
    
   
    m_endTime = std::chrono::high_resolution_clock::now();
    m_lastFrameTime = std::chrono::duration<double, std::chrono::seconds::period>(m_endTime - m_startTime).count();
    m_dt += m_lastFrameTime;
    m_time += m_lastFrameTime;
    m_frames++;
    if (m_dt > 1.0f / m_fpsUpdateRate) {
        m_frameRate = m_frames / m_dt;
        m_frames = 0;
        m_dt -= 1.0 / m_fpsUpdateRate;
    }
}

