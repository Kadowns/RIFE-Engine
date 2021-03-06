#pragma once

#include <RifeMath.h>
#include <RifeCore.h>
#include <Transform.h>

namespace Rife::Graphics {

	class GlobalLights;
    struct uDirectionalLight;
    struct uPointLight;

	class Light : public Base::Component {

	public:

        void serialize(std::ofstream& file) {
           
        }

	protected:

		glm::vec3 m_color;
		float m_intensity;

	};

    class DirectionalLight : public Light {
    public:

        DirectionalLight() { m_name = "DirectionalLight"; }

        DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);

        ~DirectionalLight() { }


        virtual void onAwake() override {
            m_transform = p_gameObject->getComponent<Transform>();
        }

        void apply(uDirectionalLight& ubo);

    private:
        Transform* m_transform;
        glm::vec3 m_direction;
    };

    class PointLight : public Light {
    public:

        PointLight() { m_name = "PointLight"; }
        PointLight(float constant, float linear, float quadratic);

        ~PointLight() { }

        virtual void onAwake() override {
            m_transform = p_gameObject->getComponent<Transform>();
        }

        virtual void onUpdate() override {
            position = m_transform->position;
        }

        void apply(uPointLight& ubo);

    private:     

        Transform* m_transform;

        glm::vec3 position;
        float m_constant;
        float m_linear;
        float m_quadratic;
    };

}