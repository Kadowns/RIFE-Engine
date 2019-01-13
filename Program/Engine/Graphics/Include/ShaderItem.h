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
            for (auto& buffer : m_buffers) {
                buffer.unmap();
                buffer.destroy();
            }
			
		};
        virtual void setupBuffer() = 0;
		virtual void updateBuffer(uint32_t index) = 0;

        Buffer& getBuffer(uint32_t index);

		static void updateBuffers(uint32_t index);

    protected:

        void flushData(void* data, uint32_t index);

        std::vector<Buffer> m_buffers;

	private:
		static std::vector<ShaderItem*> s_items;
    };
}