#include <Camera.h>

#include <Transform.h>

namespace Rife::Graphics {

    Camera* Camera::s_instance = nullptr;

    Camera::Camera(float fov, float aspect, float near, float far) {
       
        updateProjection(fov, aspect, near, far);
        s_instance = this;
    }

    glm::vec3 Camera::getPosition() { return p_gameObject->getComponent<Transform>()->m_position; }

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
        auto t = p_gameObject->getComponent<Transform>();

        m_position = t->m_position;
        m_target = m_position + t->m_front;
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_view = glm::lookAt(m_position, m_target, m_up);
        return this;
    }

    void Camera::update() {
        updateView();
        m_ubo.position = glm::vec4(m_position, 0.0f);
        m_ubo.vp = m_projection * m_view;
    }

    size_t Camera::size() {
        return sizeof(m_ubo);
    }

    void Camera::apply(VkDeviceMemory * memory, VkDeviceSize offset) {
        flushData(memory, size(), offset, &m_ubo);
    }

    Camera* Camera::getInstance() {
        return s_instance;
    }
}