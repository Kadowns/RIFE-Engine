#include <Database.h>

#include <TextureFactory.h>
#include <MeshFactory.h>


namespace Rife::Data {
	std::map<std::string, Rife::Graphics::Mesh*> Database::s_meshes;
	std::map<std::string, Rife::Graphics::Texture*> Database::s_textures;
    std::map<std::string, Rife::Graphics::Material*> Database::s_materials;

	void Database::loadTextures() {
		std::cout << "Loading Textures..." << std::endl;
        s_textures["Skybox"] = Graphics::TextureFactory::loadCubemap("Skybox/skybox.ktx");
        s_textures["Sand"] = Graphics::TextureFactory::loadTexture("Terrain/sand.ktx");
		s_textures["Grass"] = Graphics::TextureFactory::loadTexture("Terrain/grass.ktx");                
        s_textures["Rock"] = Graphics::TextureFactory::loadTexture("Terrain/rock.ktx");
        s_textures["Snow"] = Graphics::TextureFactory::loadTexture("Terrain/snow.ktx");
		s_textures["Box_specular"] = Graphics::TextureFactory::loadTexture("Objects/box_specular.ktx");
        s_textures["Metal"] = Graphics::TextureFactory::loadTexture("Objects/metal.ktx");
        s_textures["Ship"] = Graphics::TextureFactory::loadTexture("Objects/ship.ktx");
		s_textures["Default"] = Graphics::TextureFactory::loadTexture("default_texture.ktx");
        
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
        s_meshes["Ship"] = s_meshes["Cube"];//Graphics::MeshFactory::loadMesh("Arc170.obj");
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
        s_materials["Metal"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Metal"), getTexture("Metal"), getTexture("Skybox"));
        s_materials["Ship"] = Graphics::MaterialFactory::surfaceMaterial(getTexture("Ship"), getTexture("Default"), getTexture("Skybox"));
        s_materials["Terrain"] = Graphics::MaterialFactory::terrainMaterial(
            getTexture("Snow"),
            getTexture("Rock"),
            getTexture("Grass"),
            getTexture("Sand")
        );
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
        auto& it = s_meshes.find(name);
        if (it != s_meshes.end()) {
            return it->second;
        }
		throw std::runtime_error("Failed to find mesh in the DataBase!");
		return nullptr;
	}

	Graphics::Texture* Database::getTexture(const char* name) {
        auto& it = s_textures.find(name);
        if (it != s_textures.end()) {
            return it->second;
        }
		throw std::runtime_error("Failed to find texture in the DataBase!");
		return nullptr;
	}

    Graphics::Material* Database::getMaterial(const char* name) {
        auto& it = s_materials.find(name);
        if (it != s_materials.end()) {
            return it->second;
        }
        throw std::runtime_error("Failed to find material in the DataBase!");
        return nullptr;
    }
}