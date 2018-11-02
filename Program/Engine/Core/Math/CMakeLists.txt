include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(GLM REQUIRED)

set(MATH_INCLUDE
	Include/MathIncludes.h
	Include/RifeMath.h
	)
set(MATH_SOURCE
	Source/RifeMath.cpp
	)

set(MATH_ALL ${MATH_INCLUDE} ${MATH_SOURCE})

CreateModule(Engine/Core Math "${MATH_ALL}")

target_link_libraries(Math glm)

