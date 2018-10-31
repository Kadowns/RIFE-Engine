#pragma once

#include <RifeGraphics.h>
#include <map>

#define DATABASE Rife::Application::DataBase


namespace Rife::Application {

	class DataBase {

	public:

		static void LoadMeshes();
		
		static Graphics::Mesh* getMesh(const char* name);

	private:

		static std::map<const char*, Rife::Graphics::Mesh*> s_meshes;

	};
}