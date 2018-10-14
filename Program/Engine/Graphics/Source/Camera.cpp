#include <Camera.h>

Graphics::Camera* Graphics::Camera::s_instance = nullptr;

Graphics::Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspect, float near, float far) {
	m_position = position;
	m_target = target;
	m_up = up;
	m_fov = fov;
	m_aspect = aspect;
	m_near = near;
	m_far = far;
	m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
	m_view = glm::lookAt(position, target, up);
    s_instance = this;
}

Graphics::Camera* Graphics::Camera::getInstance() {
    return s_instance;
}