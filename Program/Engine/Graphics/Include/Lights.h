#pragma once

#include <RifeMath.h>
#include <RifeCore.h>
#include <Transform.h>

#define GLOBAL_LIGHTS Rife::Graphics::GlobalLights::getInstance()

namespace Rife::Graphics {

	class GlobalLights;

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
			//GLOBAL_LIGHTS->setDirectionalLight(this);
        }  

		void apply(uDirectionalLight& ubo) {
			ubo.color = glm::vec4(m_color, 1.0f);
			ubo.direction = glm::vec4(m_direction, 1.0f);
			ubo.intensitys = glm::vec4(0.1f, m_intensity, 0.0f, 0.0f);
		}

    private:

        glm::vec3 m_direction;
    };

    class PointLight : public Light {
    public:

        PointLight() {}
        PointLight(float constant, float linear, float quadratic) {
            m_constant = constant;
            m_linear = linear;
            m_quadratic = quadratic;
            m_intensity = 4.0f;
            m_color = glm::vec3(1.0f);
			//GLOBAL_LIGHTS->addPointLight(this);
        }

        void awake() {
            m_transform = p_gameObject->getComponent<Transform>();
        }

        void update() {
            m_position = m_transform->m_position;
        }

		void apply(uPointLight& ubo) {
			ubo.position = glm::vec4(m_position, 1.0f);
			ubo.color = glm::vec4(m_color, 1.0f);
			ubo.intensitys = glm::vec4(m_constant, m_linear, m_quadratic, m_intensity);
		}

    private:     

        Transform* m_transform;

        glm::vec3 m_position;
        float m_constant;
        float m_linear;
        float m_quadratic;
    };

	class GlobalLights : public ShaderItem {

	public:

		static size_t size() {
			return sizeof(m_ubo_lights);
		}

		static GlobalLights* getInstance() {
			return s_instance != nullptr ? s_instance : (s_instance = new GlobalLights());
		}

		void addPointLight(PointLight* point) {
			m_pointLights.push_back(point);
			m_ubo_lights.pointLightsCount = m_pointLights.size();
		}

		void setDirectionalLight(DirectionalLight* directional) {
			m_directionalLight = directional;
		}


		void updateLightInfo() {
			m_directionalLight->apply(m_ubo_lights.directional);
			for (uint8_t i = 0; i < m_pointLights.size(); i++) {
				m_pointLights[i]->apply(m_ubo_lights.point[i]);
			}
		}

		void apply(VkDeviceMemory* memory, VkDeviceSize offset) {
			flushData(memory, size(), offset, &m_ubo_lights);
		}

	private:

		GlobalLights() {}

		struct {
			uDirectionalLight directional;
			uPointLight point[16];
			int pointLightsCount;
		} m_ubo_lights;

		static GlobalLights* s_instance;

		DirectionalLight* m_directionalLight;
		std::vector<PointLight*> m_pointLights;
	};

}