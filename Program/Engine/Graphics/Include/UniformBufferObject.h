#pragma once

#include <ShaderItem.h>

namespace Rife::Graphics {
    namespace Ubo {
        class uTransform : public ShaderItem {
       
		public:

			static unsigned int size() {
				return sizeof(uTransform) - sizeof(ShaderItem);
			}

			glm::mat4 model;			
			virtual ~uTransform() {}

            void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {
                void* data;
                auto range = size();
                vkMapMemory(VK_WRAPPER->getDevice(), *memory, offset, range, 0, &data);
                memcpy(data, &model, range);
                vkUnmapMemory(VK_WRAPPER->getDevice(), *memory);
            }
        };

		class uCamera : public ShaderItem {

		public:
            glm::mat4 vp;
            glm::vec4 cameraPos;

			virtual ~uCamera() {}

			static unsigned int size() {
				return sizeof(uCamera) - sizeof(ShaderItem);
			}

            void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {

                void* data;
				auto range = size();
                vkMapMemory(VK_WRAPPER->getDevice(), *memory, offset, range, 0, &data);
                memcpy(data, &vp, range);
                vkUnmapMemory(VK_WRAPPER->getDevice(), *memory);
            }
        };

        struct uMaterialProperties {

            glm::vec4 color;
            float specularPower;
        };

        class uLight : public ShaderItem {

		public:

            glm::vec4 direction;
            glm::vec4 ambient;
            glm::vec4 diffuse;

            //xyz = color, w = power
            glm::vec4 specular;

			virtual ~uLight() {}

			static unsigned int size() {
				return sizeof(uLight) - sizeof(ShaderItem);
			}

            void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {

				auto range = size();

                void* data;
                vkMapMemory(VK_WRAPPER->getDevice(), *memory, offset, range, 0, &data);
                memcpy(data, &direction, range);
                vkUnmapMemory(VK_WRAPPER->getDevice(), *memory);
            }
        };
    }
}
