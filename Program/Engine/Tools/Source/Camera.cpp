#include <Camera.h>

namespace Rife::Tools {

    Camera* Camera::s_instance = nullptr;

    Camera::Camera(float fov, float aspect, float near, float far) {
       
        updateProjection(fov, aspect, near, far);
        s_instance = this;
    }

    Camera* Camera::updateProjection(float fov, float aspect, float near, float far) {
        m_fov = fov;
        m_aspect = aspect;
        m_near = near;
        m_far = far;
        m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
        m_projection[1][1] *= -1;
        return this;
    }

    Camera* Camera::updateView() {
        m_position = p_gameObject->getTransform()->m_position;
        m_target = m_position + glm::vec3(0.0f, 0.0f, -1.0f);
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_view = glm::lookAt(m_position, m_target, m_up);
        return this;
    }

    Camera* Camera::getInstance() {
        return s_instance;
    }
}