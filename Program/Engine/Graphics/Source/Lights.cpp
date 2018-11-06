#include <Lights.h>
#include <GlobalLights.h>

namespace Rife::Graphics {

    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity) {
        m_name = "DirectionalLight";
        m_direction = direction;
        m_color = color;
        m_intensity = intensity;
        GLOBAL_LIGHTS->setDirectionalLight(this);
    }

    void DirectionalLight::awake() {
        m_transform = p_gameObject->getComponent<Transform>();
    }

    void DirectionalLight::apply(uDirectionalLight& ubo) {
        ubo.color = glm::vec4(m_color, 1.0f);
        ubo.direction = glm::vec4(m_direction, 1.0f) + glm::vec4(m_transform->getFront(), 1.0f);
        ubo.intensitys = glm::vec4(0.1f, m_intensity, 0.0f, 0.0f);
    }

    PointLight::PointLight(float constant, float linear, float quadratic) {
        m_name = "PointLight";
        m_constant = constant;
        m_linear = linear;
        m_quadratic = quadratic;
        m_intensity = 4.0f;
        m_color = glm::vec3(1.0f);
        GLOBAL_LIGHTS->addPointLight(this);
    }

    void PointLight::apply(uPointLight& ubo) {
        ubo.position = glm::vec4(m_position, 1.0f);
        ubo.color = glm::vec4(m_color, 1.0f);
        ubo.intensitys = glm::vec4(m_constant, m_linear, m_quadratic, m_intensity);
    }
}