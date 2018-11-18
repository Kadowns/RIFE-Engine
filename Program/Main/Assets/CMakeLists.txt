include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(ASSETS_INCLUDE		
		Include/Movable.h
        Include/RotatingCube.h
		Include/TerrainGenerator.h
		Include/RifeAssets.h
        )
set(ASSETS_SOURCE
		Source/TerrainGenerator.cpp
	)

set(ASSETS_ALL ${ASSETS_INCLUDE} ${ASSETS_SOURCE})
CreateModule(Main Assets "${ASSETS_ALL}" )
target_link_libraries(Assets Application)