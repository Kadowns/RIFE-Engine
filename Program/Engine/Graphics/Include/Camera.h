#pragma once
#ifndef CAMERA
#include <Rm.h>

#define CAMERA Graphics::Camera::getInstance()

namespace Graphics {

    class Camera {

    public:
        Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspect, float near, float far);

        float getFov() { return m_fov; }
        float getAspect() { return m_aspect; }
        float getNear() { return m_near; }
        float getFar() { return m_far; }

        glm::vec3 getPosition() { return m_position; }
        glm::vec3 getTarget() { return m_target; }
        glm::vec3 getUp() { return m_up; }

        glm::mat4 getProjection() { return m_projection; }
        glm::mat4 getView() { return m_view; }

        static Camera* getInstance();

    private:

        static Camera* s_instance;

        float m_fov, m_aspect, m_near, m_far;
        glm::mat4 m_projection, m_view;
        glm::vec3 m_position, m_target, m_up;

    };
}
#endif // !CAMERA
