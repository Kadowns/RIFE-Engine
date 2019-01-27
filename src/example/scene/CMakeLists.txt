include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(SCENE_INCLUDE		
		include/Serializer.h
        include/ScenePlayer.h	
        )
set(SCENE_SOURCE
		source/Serializer.cpp
        source/ScenePlayer.cpp
        )

set(SCENE_ALL ${SCENE_INCLUDE} ${SCENE_SOURCE})
CreateModule(example scene "${SCENE_ALL}" )
target_link_libraries(scene components)