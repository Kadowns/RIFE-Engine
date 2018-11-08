include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMMON_INCLUDE		
		Include/Serializer.h
		Include/RifeScripts.h
        Include/ScenePlayer.h	
		Include/RotatingCube.h
		Include/Movable.h
        )
set(COMMON_SOURCE
		Source/Serializer.cpp
        Source/ScenePlayer.cpp
        )

set(COMMON_ALL ${COMMON_INCLUDE} ${COMMON_SOURCE})
CreateModule(Main Common "${COMMON_ALL}" )
target_link_libraries(Common Application)