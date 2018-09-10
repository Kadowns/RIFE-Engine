include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(GRAPHICS_INCLUDE VkUtilities.h)
set(GRAPHICS_DEFINITION VkUtilities.cpp)
set(GRAPHICS_ALL ${GRAPHICS_INCLUDE} ${GRAPHICS_DEFINITION})
CreateModule(Graphics "${GRAPHICS_ALL}")
