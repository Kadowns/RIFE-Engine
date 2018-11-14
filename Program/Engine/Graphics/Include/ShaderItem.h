#pragma once

#include <VulkanData.h>

#include <Buffer.h>

namespace Rife::Graphics {

	enum SHADER_ITEM_TYPE {
		SHADER_ITEM_TYPE_TRANSFORM = 1,
		SHADER_ITEM_TYPE_CAMERA = 2,
		SHADER_ITEM_TYPE_LIGHTS = 3,
		SHADER_ITEM_TYPE_SKYBOX = 4,
	};


    class ShaderItem {

    public:

        ShaderItem() {
			s_items.push_back(this);
		}

        virtual ~ShaderItem() {
			m_buffer.unmap();
			m_buffer.destroy();
		};
        virtual void apply() = 0;
        virtual void setupBuffer() = 0;
		virtual void updateUniformBuffer() = 0;

        Buffer& getBuffer();

		static void updateUniformBuffers();

    protected:

        void flushData(void* data);

        Buffer m_buffer;

	private:
		static std::vector<ShaderItem*> s_items;
    };
}