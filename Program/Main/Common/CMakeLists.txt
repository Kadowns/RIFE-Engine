include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMMON_INCLUDE
        Include/Test.h 
        Include/Triangle.h	
		Include/RotatingCube.h
		Include/Movable.h
        )
set(COMMON_SOURCE
        Source/Test.cpp
        Source/Triangle.cpp
        )

SeT(COMMON_ALL ${COMMON_INCLUDE} ${COMMON_SOURCE})
CreateModule(Main Common "${COMMON_ALL}" )
target_LINK_libraries(Common Application)