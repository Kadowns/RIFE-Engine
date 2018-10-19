include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(GLM REQUIRED)

set(MATH_INCLUDE
	Include/Rm.h
	Include/Transform.h
	)
set(MATH_SOURCE 
	Source/Rm.cpp
	)
set(MATH_ALL ${MATH_INCLUDE} ${MATH_SOURCE})

CreateModule(Engine/Core Math "${MATH_ALL}")

target_link_libraries(Math glm)

