#pragma once

#include <RifeCore.h>

#include <vector>


namespace Rife::Data {

    class Dataloader {
    public:
        //static void loadObj(
        //    const char* path//,
        //    //std::vector<glm::vec3>& positions,
        //    //std::vector<glm::vec3>& normals,
        //    //std::vector<glm::vec2>& uvs,
        //    //std::vector<uint32_t>& indices
        //);

        static bool loadScene(const char* path, std::vector<Base::GameObject*>& gameObjects);


    };
   


}
