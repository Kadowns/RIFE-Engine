#pragma once
#include <RifeMath.h>
#include <RifeCore.h>
#include <UniformBufferObject.h>

#define CAMERA Rife::Graphics::Camera::getInstance()

namespace Rife::Graphics {

    class Camera : public Base::Component {

    public:
		Camera() { s_instance = this; }
        Camera(float fov, float aspect, float near, float far);

        float getFov() { return m_fov; }
        float getAspect() { return m_aspect; }
        float getNear() { return m_near; }
        float getFar() { return m_far; }

        glm::vec3 getPosition() { return p_gameObject->getTransform()->m_position; }
        glm::vec3 getTarget() { return m_target; }
        glm::vec3 getUp() { return m_up; }

        Camera* updateProjection(float fov, float aspect, float near, float far);
        Camera* updateView();
        Ubo::uCamera* getUbo() { return &m_ubo; }
        glm::mat4 getProjection() { return m_projection; }
        glm::mat4 getView() { return m_view; }

        void update();

        static Camera* getInstance();

    private:

        static Camera* s_instance;

        Ubo::uCamera m_ubo;


        float m_fov, m_aspect, m_near, m_far;
        glm::mat4 m_projection, m_view;
        glm::vec3 m_position, m_target, m_up;

    };
}
