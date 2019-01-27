include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(GLM REQUIRED)

set(MATH_INCLUDE
	include/MathIncludes.h
	include/RifeMath.h
	include/PerlinNoise.h
	)
set(MATH_SOURCE
	source/RifeMath.cpp
	source/PerlinNoise.cpp
	)

set(MATH_ALL ${MATH_INCLUDE} ${MATH_SOURCE})

CreateModule(engine/core math "${MATH_ALL}")

target_link_libraries(math glm)

