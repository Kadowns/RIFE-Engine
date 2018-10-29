#pragma once

#include <ShaderItem.h>

namespace Rife::Graphics {
    namespace Ubo {
        struct uTransform : ShaderItem {
       
			glm::mat4 model;			

            void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {
                void* data;
                auto range = sizeof(model);
                vkMapMemory(*VK_WRAPPER->getDevice(), *memory, offset, range, 0, &data);
                memcpy(data, &model, range);
                vkUnmapMemory(*VK_WRAPPER->getDevice(), *memory);
            }
        };

        struct uCamera : ShaderItem {

            glm::mat4 vp;
            glm::vec4 cameraPos;

            void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {

                void* data;
                auto range = sizeof(vp) + sizeof(cameraPos);
                vkMapMemory(*VK_WRAPPER->getDevice(), *memory, offset, range, 0, &data);
                memcpy(data, &vp, range);
                vkUnmapMemory(*VK_WRAPPER->getDevice(), *memory);
            }
        };

        struct uMaterialProperties {

            glm::vec4 ambient;
            glm::vec4 diffuse;
            glm::vec4 specular;

           /* void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {

                struct {
                    glm::vec4 ambient;
                    glm::vec4 diffuse;
                    glm::vec4 specular;
                } temp;
                temp.ambient = ambient;
                temp.diffuse = diffuse;
                temp.specular = specular;

                void* data;
        
                vkMapMemory(*VK_WRAPPER->getDevice(), *memory, offset, sizeof(temp), 0, &data);
                memcpy(data, &temp, sizeof(temp));
                vkUnmapMemory(*VK_WRAPPER->getDevice(), *memory);
            }*/
        };

        struct uLight : ShaderItem {
            glm::vec4 direction;
            glm::vec4 ambient;
            glm::vec4 diffuse;

            //xyz = color, w = power
            glm::vec4 specular;

            void Apply(Shader* shader, VkDeviceMemory* memory, VkDeviceSize offset) {
                struct {
                    glm::vec4 direction;
                    glm::vec4 ambient;
                    glm::vec4 diffuse;
                    //xyz = color, w = power
                    glm::vec4 specular;
                }temp;

                temp.direction = direction;
                temp.ambient = ambient;
                temp.diffuse = diffuse;
                temp.specular = specular;

                void* data;
                vkMapMemory(*VK_WRAPPER->getDevice(), *memory, offset, sizeof(temp), 0, &data);
                memcpy(data, &temp, sizeof(temp));
                vkUnmapMemory(*VK_WRAPPER->getDevice(), *memory);
            }
        };
    }
}


