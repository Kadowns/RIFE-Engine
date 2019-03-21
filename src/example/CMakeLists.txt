include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMPONENTS_INCLUDE		
		include/ScenePlayer.h
		include/Movable.h
        include/RotatingCube.h
		include/TerrainGenerator.h
		include/RifeAssets.h
        )
set(COMPONENTS_SOURCE
    source/ScenePlayer.cpp
	source/TerrainGenerator.cpp
	)

set(COMPONENTS_ALL ${COMPONENTS_INCLUDE} ${COMPONENTS_SOURCE})
CreateModule(example gamesrc "${COMPONENTS_ALL}" )
target_link_libraries(gamesrc window)

add_executable(RIFE_Engine main.cpp)
target_link_libraries(RIFE_Engine gamesrc)