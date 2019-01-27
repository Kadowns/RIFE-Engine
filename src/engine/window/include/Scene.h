#pragma once
#include <cstdint>

class Scene {
private:

public:
    virtual void init() = 0;
    virtual void awake() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void deinit() = 0;
    virtual void windowResized(const uint32_t& width, const uint32_t& height) {}
};
