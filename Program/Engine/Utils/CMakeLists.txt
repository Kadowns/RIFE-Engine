include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(GLM REQUIRED)


set(UTILS_INCLUDE
	Include/Rm.h
	Include/Types.h
	Include/Component.h
	Include/Object.h
	)
set(UTILS_SOURCE Source/Rm.cpp)
set(UTILS_ALL ${UTILS_INCLUDE} ${UTILS_SOURCE})
CreateModule(Utils "${UTILS_ALL}")

target_link_libraries(Utils glm)
