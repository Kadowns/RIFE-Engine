#pragma once
#include <RifeMath.h>
#include <RifeCore.h>
#include <ShaderItem.h>


namespace Rife::Graphics {

	struct Transform : public Base::Component, public ShaderItem {

		glm::vec3 position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        Transform() : position(glm::vec3(0.0f)), m_rotation(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {
            m_name = "Transform";
			setupBuffer();
        }

        static size_t size() {
            return sizeof(m_ubo);
        }

        void serialize(std::ofstream& file) {
            std::string offset("    ");
            file << (offset + NAME(position) + ": " + std::to_string(position) + "\n").c_str();
            file << (offset + NAME(m_rotation) + ": " + std::to_string(m_rotation) + "\n").c_str();
            file << (offset + NAME(m_scale) + ": " + std::to_string(m_scale) + "\n").c_str();
        }

        glm::vec3 getFront() {
            return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        }

        glm::vec3 getUp() {
            return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
        }

        glm::vec3 getRight() {
            return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
        }

        glm::vec3 getEuler() { return glm::degrees(glm::eulerAngles(m_rotation)); }

        glm::mat4 getRotationMatrix() {
            return glm::mat4_cast(m_rotation);
        }

        glm::mat4 getModelMatrix() {
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 rotate = glm::mat4_cast(m_rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
            return translate * rotate * scale;
        }

        // Inherited via ShaderItem
        virtual void setupBuffer() override;

		virtual void updateBuffer(uint32_t imageIndex) override;

    private:
        struct {
            glm::mat4 model;
            glm::mat4 inverse;
        } m_ubo;
    };
}