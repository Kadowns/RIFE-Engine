#pragma once
#include <VkUtilities.h>

namespace Rife::Graphics {

    class Shader;

    class ShaderItem {
    public:
		virtual ~ShaderItem() {}
        virtual void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) = 0;
    };
}