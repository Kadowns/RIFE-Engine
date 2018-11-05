include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMMON_INCLUDE
		Include/DataLoader.h
		Include/RifeScripts.h
        Include/ScenePlayer.h	
		Include/RotatingCube.h
		Include/Movable.h
        )
set(COMMON_SOURCE
Include/DataLoader.h
        Source/DataLoader.cpp
        Source/ScenePlayer.cpp
        )

SeT(COMMON_ALL ${COMMON_INCLUDE} ${COMMON_SOURCE})
CreateModule(Main Common "${COMMON_ALL}" )
target_LINK_libraries(Common Application)