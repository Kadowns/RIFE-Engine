#pragma once

#include <RifeGraphics.h>

#include <map>

#define DATABASE Rife::Application::DataBase

namespace Rife::Application {

	class DataBase {

	public:

		static void loadData();
		static void unloadData();
		
		
		static Graphics::Mesh* getMesh(const char* name);
		static Graphics::Texture* getTexture(const char* name);
        static Graphics::Material* getMaterial(const char* name);

	private:
		static void loadTextures();
		static void unloadTextures();
		static void loadMeshes();
		static void unloadMeshes();
        static void loadMaterials();
        static void unloadMaterials();

		static std::map<const char*, Rife::Graphics::Mesh*> s_meshes;
		static std::map<const char*, Rife::Graphics::Texture*> s_textures;
        static std::map<const char*, Rife::Graphics::Material*> s_materials;
	};
}