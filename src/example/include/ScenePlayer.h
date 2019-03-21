#pragma once

#include <Window.h>

using namespace Rife::Graphics;
using namespace Rife::Tools;
using namespace Rife::Base;
using namespace Rife::Math;

class ScenePlayer : public Scene {

public:
    ScenePlayer();
    ~ScenePlayer();


protected:
    virtual void onLoadScene() override;

    // Inherited via Scene
    virtual void onUpdate() override;
    virtual void onLateUpdate() override;
    virtual void onRender() override;
    virtual void onTerminate() override;
};

