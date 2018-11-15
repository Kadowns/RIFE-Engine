#include <GlobalLights.h>
#include <Lights.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

    GlobalLights* GlobalLights::s_instance = nullptr;

    void GlobalLights::addPointLight(PointLight* point) {
        m_pointLights.push_back(point);
        m_ubo_lights.pointLightsCount = m_pointLights.size();
    }

    void GlobalLights::setDirectionalLight(DirectionalLight* directional) {
        m_directionalLight = directional;
    }

    void GlobalLights::apply() {
        flushData(&m_ubo_lights);
    }

    void GlobalLights::setupBuffer() {
        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VulkanTools::createBuffer(sizeof(m_ubo_lights), info, m_buffer);
		m_buffer.map();
    }

	void GlobalLights::updateBuffer() {
		if (m_directionalLight != nullptr) {
			m_directionalLight->apply(m_ubo_lights.directional);
		}
		for (uint8_t i = 0; i < m_pointLights.size(); i++) {
			m_pointLights[i]->apply(m_ubo_lights.point[i]);
		}
		apply();
	}
}