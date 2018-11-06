include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMMON_INCLUDE		
		Include/RifeScripts.h
		Include/Serializer.h
        Include/ScenePlayer.h	
		Include/RotatingCube.h
		Include/Movable.h
        )
set(COMMON_SOURCE
        Source/Serializer.cpp
        Source/ScenePlayer.cpp
        )

SeT(COMMON_ALL ${COMMON_INCLUDE} ${COMMON_SOURCE})
CreateModule(Main Common "${COMMON_ALL}" )
target_LINK_libraries(Common Application)