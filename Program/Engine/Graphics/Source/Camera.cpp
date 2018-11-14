#include <Camera.h>
#include <VulkanTools.h>
#include <Transform.h>

namespace Rife::Graphics {

    Camera* Camera::s_instance = nullptr;

    Camera::Camera(float fov, float aspect, float near, float far) {
       
        m_name = "Camera";
        updateProjection(fov, aspect, near, far);
        setupBuffer();
        s_instance = this;      
    }

    Camera::~Camera() {
        m_buffer.destroy();
    }

    glm::vec3 Camera::getPosition() { return getComponent<Transform>()->m_position; }

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
        auto t = getComponent<Transform>();

        m_position = t->m_position;
        m_target = m_position + t->getFront();
        m_up = t->getUp();
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

    void Camera::apply(VkDeviceMemory* memory, VkDeviceSize offset) {
        flushData(&m_ubo);
    }

    Camera* Camera::getInstance() {
        return s_instance;
    }

    void Camera::setupBuffer() {

        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VulkanTools::createBuffer(sizeof(m_ubo), info, m_buffer);
        m_buffer.map();
    }
}