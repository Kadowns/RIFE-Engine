#pragma once
#include <RifeMath.h>
#include <RifeCore.h>
#include <ShaderItem.h>


namespace Rife::Graphics {

	struct Transform : public Base::Component, public ShaderItem {

		glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        Transform() : m_position(glm::vec3(0.0f)), m_rotation(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {
            m_name = "Transform";
        }

        static size_t size() {
            return sizeof(glm::mat4);
        }

        void serialize(std::ofstream& file) {
            std::string offset("    ");
            file << (offset + NAME(m_position) + ": " + std::to_string(m_position) + "\n").c_str();
            file << (offset + NAME(m_rotation) + ": " + std::to_string(m_rotation) + "\n").c_str();
            file << (offset + NAME(m_scale) + ": " + std::to_string(m_scale) + "\n").c_str();
        }

        void apply(VkDeviceMemory* memory, VkDeviceSize offset) {
            flushData(memory, size(), offset, &getModelMatrix());
        }

        glm::vec3 getFront() {
            return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        }

        glm::vec3 getUp() {
            return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
        }

        glm::vec3 getEuler() { return glm::degrees(glm::eulerAngles(m_rotation)); }

        glm::mat4 getModelMatrix() {
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_position);
            glm::mat4 rotate = glm::mat4_cast(m_rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
            return translate * rotate * scale;
        }
	};
}