#pragma once

#include <RifeMath.h>
#include <RifeCore.h>
#include <Transform.h>

#define GLOBAL_LIGHTS Rife::Graphics::GlobalLights::getInstance()

namespace Rife::Graphics {

    class GlobalLights;

    class Light : public Base::Component {
    
    public:

    protected:

        glm::vec3 m_color;
        float m_intensity;

    };

    class DirectionalLight : public Light {
    public:

        DirectionalLight() {}

        DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity) {
            m_direction = direction;
            m_color = color;
            m_intensity = intensity;
        }
       
    private:
        friend GlobalLights;

        glm::vec3 m_direction;
    };

    class PointLight : public Light {
    public:

        PointLight() {}
        PointLight(float constant, float linear, float quadratic) {
            m_constant = constant;
            m_linear = linear;
            m_quadratic = quadratic;
        }

        void awake() {
            m_transform = p_gameObject->getComponent<Transform>();
        }

        void update() {
            m_position = m_transform->m_position;
        }

    private:
        friend GlobalLights;

        Transform* m_transform;

        glm::vec3 m_position;
        float m_constant;
        float m_linear;
        float m_quadratic;
    };


    class GlobalLights : public ShaderItem {

    public:

        GlobalLights(DirectionalLight& directional, PointLight& point) {
            m_directionalLight = directional;
            m_pointLight = point;
            s_instance = this;
        }

        static size_t size() {
            return sizeof(m_ubo_lights);
        }

        static GlobalLights* getInstance() {
            return s_instance;
        }

        void updateLightInfo() {
            m_ubo_lights.directional.direction = glm::vec4(m_directionalLight.m_direction, 0.0f);
            m_ubo_lights.directional.color = glm::vec4(m_directionalLight.m_color, 1.0f);
            m_ubo_lights.directional.intensitys = glm::vec4(0.1f, m_directionalLight.m_intensity, 0.0f, 0.0f);

            m_ubo_lights.point.position = glm::vec4(m_pointLight.m_position, 1.0f);
            m_ubo_lights.point.color = glm::vec4(m_pointLight.m_color, 1.0f);
            m_ubo_lights.point.intensitys = glm::vec4(m_pointLight.m_constant, m_pointLight.m_linear, m_pointLight.m_quadratic, m_pointLight.m_intensity);
        }

        void apply(VkDeviceMemory* memory, VkDeviceSize offset) {
            flushData(memory, size(), offset, &m_ubo_lights);
        }

    private:

        struct {
            struct {
                glm::vec4 direction;
                glm::vec4 color;
                glm::vec4 intensitys;
            } directional;

            struct {
                glm::vec4 position;
                glm::vec4 color;
                glm::vec4 intensitys;
            } point;
        } m_ubo_lights;

        static GlobalLights* s_instance;

        DirectionalLight m_directionalLight;
        PointLight m_pointLight;
    };

}