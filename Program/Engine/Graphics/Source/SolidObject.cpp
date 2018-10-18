#include <SolidObject.h>

Entity::SolidObject::SolidObject(Mesh* mesh) {
	m_mesh = mesh;
	m_transform = new Transform();
	m_renderer = new MeshRenderer(m_mesh, this);
}

Entity::SolidObject::~SolidObject() {
	delete m_mesh;
	delete m_renderer;
	delete m_transform;
}

void Entity::SolidObject::setup() {

}

void Entity::SolidObject::update(float secs) {
	m_transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
}
