#include <DataBase.h>



namespace Rife::Application {
	std::map<const char*, Rife::Graphics::Mesh*> DataBase::s_meshes;
	std::map<const char*, Rife::Graphics::Texture*> DataBase::s_textures;

	void DataBase::loadTextures() {
		std::cout << "Loading Textures..." << std::endl;
		s_textures["box"] = Graphics::TextureFactory::loadTexture("box.png");
		s_textures["box_specular"] = Graphics::TextureFactory::loadTexture("box_specular.png");
		s_textures["default"] = Graphics::TextureFactory::loadTexture("default.png");
		std::cout << "Textures loaded!" << std::endl;
	}

	void DataBase::unloadTextures() {
		for (auto it : s_textures) {
			delete it.second;
		}
		s_textures.clear();
	}

	void DataBase::loadMeshes() {
		std::cout << "Loading Meshes..." << std::endl;
		s_meshes["Cube"] = Graphics::MeshFactory::createCube();
		s_meshes["Plane"] = Graphics::MeshFactory::createPlane(20, 20);
		s_meshes["PolarSphere"] = Graphics::MeshFactory::createPolarSphere(32);
		s_meshes["NormalizedSphere"] = Graphics::MeshFactory::createNormalizedSphere(32);
		std::cout << "Meshes loaded!" << std::endl;
	}

	void DataBase::unloadMeshes() {
		for (auto it : s_meshes) {
			delete it.second;
		}
		s_meshes.clear();
	}

	void DataBase::loadData() {
		std::cout << "----------Loading data----------" << std::endl;
		loadMeshes();
		loadTextures();
		std::cout << "-----Finished loading data------" << std::endl;
	}

	void DataBase::unloadData() {
		unloadMeshes();
		unloadTextures();
	}

	Graphics::Mesh* DataBase::getMesh(const char* name) {
		//isso ai ta uma bosta
		for (auto it : s_meshes) {
			if (strcmp(it.first, name) == 0) {
				return it.second;
			}
		}
		throw std::runtime_error("Failed to find mesh in the DataBase!");
		return nullptr;
	}

	Graphics::Texture* DataBase::getTexture(const char* name) {
		for (auto it : s_textures) {
			if (strcmp(it.first, name) == 0) {
				return it.second;
			}
		}
		throw std::runtime_error("Failed to find texture in the DataBase!");
		return nullptr;
	}
}