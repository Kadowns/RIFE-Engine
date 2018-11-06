#include <Serializer.h>
#include <RifePath.h>

#include <RifeScripts.h>
#include <RifeGraphics.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace Rife::Data {

    bool Serializer::loadScene(const char* path, std::vector<Base::GameObject*>& gameObjects) {
        std::ifstream file(SCENE_FOLDER + std::string(path));

        if (!file.is_open()) {
            std::cout << "n abri essa merda\n";
            return false;
        }

        std::string line;
        size_t index;
        while (std::getline(file, line)) {

            std::istringstream word(line);
            std::string header;
            word >> header;
            if (header == "g-") {
                gameObjects.push_back(new Base::GameObject(new Graphics::Transform()));
                index = gameObjects.size() - 1;
                word >> header;
                while (header != "-g") {

                    word >> header;



                }



            }

            file.close();


        }

    }

    void Serializer::saveScene(const char* path, std::vector<Base::GameObject*>& gameObjects) {
        std::ofstream file((SCENE_FOLDER + std::string(path)));

        if (!file.is_open()) {
            std::cout << "Failed to save scene!" << std::endl;
            return;
        }

        for (auto obj : gameObjects) {
            obj->serialize(file);
        }

        file.close();

    }
}


