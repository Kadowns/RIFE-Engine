include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMPONENTS_INCLUDE		
		include/Movable.h
        include/RotatingCube.h
		include/TerrainGenerator.h
		include/RifeAssets.h
        )
set(COMPONENTS_SOURCE
	source/TerrainGenerator.cpp
	)

set(COMPONENTS_ALL ${COMPONENTS_INCLUDE} ${COMPONENTS_SOURCE})
CreateModule(example components "${COMPONENTS_ALL}" )
target_link_libraries(components window)