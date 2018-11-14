#pragma once

#include <ShaderItem.h>
#include <RifeMath.h>

#define GLOBAL_LIGHTS Rife::Graphics::GlobalLights::getInstance()

namespace Rife::Graphics {

    class PointLight;
    class DirectionalLight;

    struct uPointLight {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec4 intensitys;
    };

    struct uDirectionalLight {
        glm::vec4 direction;
        glm::vec4 color;
        glm::vec4 intensitys;

    };

    class GlobalLights : public ShaderItem {

    public:

        size_t size() {
            return sizeof(m_ubo_lights);
        }

        static GlobalLights* getInstance() {
            return s_instance != nullptr ? s_instance : (s_instance = new GlobalLights());
        }

        void addPointLight(PointLight* point);

        void setDirectionalLight(DirectionalLight* directional);


        void updateLightInfo();

        void apply(VkDeviceMemory* memory, VkDeviceSize offset);

    private:

        GlobalLights() {}

        struct {
            uDirectionalLight directional;
            uPointLight point[16];
            int pointLightsCount;
        } m_ubo_lights;

        static GlobalLights* s_instance;

        DirectionalLight* m_directionalLight = nullptr;
        std::vector<PointLight*> m_pointLights;

        // Inherited via ShaderItem
        virtual void setupBuffer() override;
    };
}