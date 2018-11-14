#pragma once

#include <RifeCore.h>
#include <ShaderItem.h>
#include <RifeMath.h>

#define SKYBOX Rife::Graphics::Skybox::getInstance()

namespace Rife::Graphics {

    class Skybox : public ShaderItem {

    public:

        Skybox();

        static Skybox* getInstance();


		static size_t size();

        void serialize(std::ofstream& file) {

        }

        void apply();

    private:

        struct {

            glm::mat4 m_projection;
            glm::mat4 m_view;

        } m_ubo;     


        // Inherited via ShaderItem
        virtual void setupBuffer() override;


		// Inherited via ShaderItem
		virtual void updateUniformBuffer() override;

	};
}