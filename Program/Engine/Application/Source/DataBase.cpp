#include <DataBase.h>



namespace Rife::Application {
	std::map<const char*, Rife::Graphics::Mesh*> DataBase::s_meshes;

	void DataBase::LoadMeshes() {
		s_meshes.insert(std::pair<const char*, Graphics::Mesh*>("Cube", Graphics::MeshFactory::createCube()));
		s_meshes.insert(std::pair<const char*, Graphics::Mesh*>("Plane", Graphics::MeshFactory::createPlane(10, 10)));
		s_meshes.insert(std::pair<const char*, Graphics::Mesh*>("Sphere", Graphics::MeshFactory::createSphere(10, 10)));
	}

	Graphics::Mesh* DataBase::getMesh(const char* name) {
		auto it = s_meshes.find(name);
		if (it != s_meshes.end()) {
			return it->second;
		}
		else {
			throw std::runtime_error("Mesh was not found in the DataBase!");
			return nullptr;
		}
		
	}


}