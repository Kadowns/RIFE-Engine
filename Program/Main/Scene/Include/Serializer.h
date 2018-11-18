#pragma once

#include <RifeCore.h>

#include <vector>


namespace Rife::Data {

    class Serializer {
    public:

        static bool loadScene(const char* path, std::vector<Base::GameObject*>& gameObjects);
        static void saveScene(const char* path, std::vector<Base::GameObject*>& gameObjects);
    };

}
