#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Event.h>


typedef Rife::Base::Event<> OnCleanupRenderer;
typedef Rife::Base::Event<> OnCleanupPipeline;
typedef Rife::Base::Event<> OnRecreateRenderer;
typedef Rife::Base::Event<> OnRecreatePipeline;
typedef Rife::Base::Event<const uint32_t&> OnDraw;