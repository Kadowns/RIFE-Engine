#include <DataBase.h>

#include <TextureFactory.h>
#include <MeshFactory.h>


namespace Rife::Application {
	std::map<const char*, Rife::Graphics::Mesh*> DataBase::s_meshes;
	std::map<const char*, Rife::Graphics::Texture*> DataBase::s_textures;
    std::map<const char*, Rife::Graphics::Material*> DataBase::s_materials;

	void DataBase::loadTextures() {
		std::cout << "Loading Textures..." << std::endl;
		s_textures["Box"] = Graphics::TextureFactory::loadTexture("box.png");
		s_textures["Box_specular"] = Graphics::TextureFactory::loadTexture("box_specular.png");
        s_textures["Metal"] = Graphics::TextureFactory::loadTexture("metal.png");
        s_textures["Metal_specular"] = Graphics::TextureFactory::loadTexture("metal_specular.png");
		s_textures["Default"] = Graphics::TextureFactory::loadTexture("default.png");
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

    void DataBase::loadMaterials() {
        std::cout << "Loading Materials..." << std::endl;
        s_materials["Default"] = Graphics::MaterialFactory::defaultMaterial();
        s_materials["Box"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Box"), getTexture("Box_specular"));
        s_materials["Metal"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Metal"), getTexture("Metal_specular"));
        std::cout << "Materials loaded!" << std::endl;
    }

    void DataBase::unloadMaterials() {
        for (auto it : s_materials) {
            delete it.second;
        }
        s_materials.clear();
    }

	void DataBase::loadData() {
		std::cout << "----------Loading data----------" << std::endl;
		loadMeshes();
		loadTextures();
        loadMaterials();
		std::cout << "-----Finished loading data------" << std::endl;
	}

	void DataBase::unloadData() {
        unloadMaterials();
		unloadTextures();
        unloadMeshes();
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

    Graphics::Material* DataBase::getMaterial(const char* name) {
        for (auto it : s_materials) {
            if (strcmp(it.first, name) == 0) {
                return it.second;
            }
        }
        throw std::runtime_error("Failed to find material in the DataBase!");
        return nullptr;
    }
}