include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
set(COMMON_INCLUDE
        Include/Test.h 
        Include/Triangle.h	
		Include/RotatingCube.h
        )
set(COMMON_SOURCE
        Source/Test.cpp
        Source/Triangle.cpp
        )

set(COMMON_ALL ${COMMON_INCLUDE} ${COMMON_SOURCE})
CreateModule(Main Common "${COMMON_ALL}" )
target_link_libraries(Common Application)