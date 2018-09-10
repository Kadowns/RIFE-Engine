include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
message("Creating app")
set(APPLICATION_INCLUDE
        Window.h
        Window.cpp
        Test.h Triangle.h
        )
set(APPLICATION_DEFINITION
        Test.cpp Triangle.cpp
        )
set(APPLICATION_SCENE
        Scene.h
        )
set(APPLICATION_ALL ${APPLICATION_INCLUDE} ${APPLICATION_DEFINITION} ${APPLICATION_SCENE})
CreateModule(Application "${APPLICATION_ALL}")
target_link_libraries(Application Tools)