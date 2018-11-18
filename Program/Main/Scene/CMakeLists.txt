include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(SCENE_INCLUDE		
		Include/Serializer.h
        Include/ScenePlayer.h	
        )
set(SCENE_SOURCE
		Source/Serializer.cpp
        Source/ScenePlayer.cpp
        )

set(SCENE_ALL ${SCENE_INCLUDE} ${SCENE_SOURCE})
CreateModule(Main Scene "${SCENE_ALL}" )
target_link_libraries(Scene Assets)