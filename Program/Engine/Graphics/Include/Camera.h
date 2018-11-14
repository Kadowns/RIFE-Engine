#pragma once
#include <RifeMath.h>
#include <RifeCore.h>

#include <ShaderItem.h>
#include <string>

#define CAMERA Rife::Graphics::Camera::getInstance()

namespace Rife::Graphics {

    class Camera : public Base::Component, public ShaderItem {

    public:
		Camera() { m_name = "Camera"; s_instance = this; }
        Camera(float fov, float aspect, float near, float far);
        ~Camera();

        void serialize(std::ofstream& file) {
            std::string offset("    ");
            file << (offset + NAME(m_fov) + ": " + std::to_string(m_fov) + "\n").c_str();
            file << (offset + NAME(m_near) + ": " + std::to_string(m_near) + "\n").c_str();
            file << (offset + NAME(m_far) + ": " + std::to_string(m_far) + "\n").c_str();
        }

        float getFov() { return m_fov; }
        float getAspect() { return m_aspect; }
        float getNear() { return m_near; }
        float getFar() { return m_far; }

        glm::vec3 getPosition();
        glm::vec3 getTarget() { return m_target; }
        glm::vec3 getUp() { return m_up; }

        Camera* updateProjection(float fov, float aspect, float near, float far);
        Camera* updateView();
        glm::mat4 getProjection() { return m_projection; }
        glm::mat4 getView() { return m_view; }

        void update();

        size_t size();

        void apply(VkDeviceMemory* memory, VkDeviceSize offset);
        // Inherited via ShaderItem
        static Camera* getInstance();

    private:

        static Camera* s_instance;

        struct {
            glm::mat4 vp;
            glm::vec4 position;
        } m_ubo;


        float m_fov, m_aspect, m_near, m_far;
        glm::mat4 m_projection, m_view;
        glm::vec3 m_position, m_target, m_up;

        virtual void setupBuffer() override;

        

    };
}
