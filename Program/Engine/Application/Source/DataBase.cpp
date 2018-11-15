#include <Database.h>

#include <TextureFactory.h>
#include <MeshFactory.h>


namespace Rife::Data {
	std::map<const char*, Rife::Graphics::Mesh*> Database::s_meshes;
	std::map<const char*, Rife::Graphics::Texture*> Database::s_textures;
    std::map<const char*, Rife::Graphics::Material*> Database::s_materials;

	void Database::loadTextures() {
		std::cout << "Loading Textures..." << std::endl;
        s_textures["Skybox"] = Graphics::TextureFactory::loadCubemap("skybox.ktx");
		s_textures["Box"] = Graphics::TextureFactory::loadTexture("box.ktx");
		s_textures["Box_specular"] = Graphics::TextureFactory::loadTexture("box_specular.ktx");
        s_textures["Metal"] = Graphics::TextureFactory::loadTexture("metal.ktx");
		s_textures["Default"] = Graphics::TextureFactory::loadTexture("default_texture.ktx");
        s_textures["Ship"] = Graphics::TextureFactory::loadTexture("ship.ktx");
		std::cout << "Textures loaded!" << std::endl;
	}

	void Database::unloadTextures() {
		for (auto it : s_textures) {
			delete it.second;
		}
		s_textures.clear();
	}

	void Database::loadMeshes() {
		std::cout << "Loading Meshes..." << std::endl;
		s_meshes["Cube"] = Graphics::MeshFactory::createCube();
        s_meshes["Skybox"] = Graphics::MeshFactory::createSkybox();
		s_meshes["Plane"] = Graphics::MeshFactory::createPlane(20, 20);
		s_meshes["PolarSphere"] = Graphics::MeshFactory::createPolarSphere(32);
		s_meshes["NormalizedSphere"] = Graphics::MeshFactory::createNormalizedSphere(32);
       // s_meshes["Ship"] = Graphics::MeshFactory::loadMesh("Arc170.obj");
		std::cout << "Meshes loaded!" << std::endl;
	}

    Graphics::Mesh* Database::insertMesh(Graphics::Mesh* mesh, const char* name) {
        if (s_meshes.count(name) != 0) {
            throw std::runtime_error("A mesh with the name " + std::string(name) + " already exists!");
        }
        s_meshes[name] = mesh;
        return mesh;
    }

	void Database::unloadMeshes() {
		for (auto it : s_meshes) {
			delete it.second;
		}
		s_meshes.clear();
	}

    void Database::loadMaterials() {
        std::cout << "Loading Materials..." << std::endl;
        s_materials["Default"] = Graphics::MaterialFactory::defaultMaterial();
        s_materials["Skybox"] = Graphics::MaterialFactory::skyboxMaterial(getTexture("Skybox"));
        s_materials["Box"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Box"), getTexture("Box_specular"), getTexture("Skybox"));
        s_materials["Metal"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Metal"), getTexture("Metal"), getTexture("Skybox"));
        s_materials["Ship"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Ship"), getTexture("Default"), getTexture("Skybox"));
        std::cout << "Materials loaded!" << std::endl;
    }

    void Database::unloadMaterials() {
        for (auto it : s_materials) {
            delete it.second;
        }
        s_materials.clear();
    }

	void Database::loadData() {
		std::cout << "----------Loading data----------" << std::endl;
		loadMeshes();
		loadTextures();
        loadMaterials();
		std::cout << "-----Finished loading data------" << std::endl;
	}

	void Database::unloadData() {
        unloadMaterials();
		unloadTextures();
        unloadMeshes();
	}

	Graphics::Mesh* Database::getMesh(const char* name) {
		//isso ai ta uma bosta     
		for (auto it : s_meshes) {
			if (strcmp(it.first, name) == 0) {
				return it.second;
			}
		}
		throw std::runtime_error("Failed to find mesh in the DataBase!");
		return nullptr;
	}

	Graphics::Texture* Database::getTexture(const char* name) {
		for (auto it : s_textures) {
			if (strcmp(it.first, name) == 0) {
				return it.second;
			}
		}
		throw std::runtime_error("Failed to find texture in the DataBase!");
		return nullptr;
	}

    Graphics::Material* Database::getMaterial(const char* name) {
        for (auto it : s_materials) {
            if (strcmp(it.first, name) == 0) {
                return it.second;
            }
        }
        throw std::runtime_error("Failed to find material in the DataBase!");
        return nullptr;
    }
}