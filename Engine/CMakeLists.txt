include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(GRAPHICS_INCLUDE Graphics/VkUtilities.h)
set(GRAPHICS_DEFINITION Graphics/VkUtilities.cpp)
set(GRAPHICS_ALL ${GRAPHICS_INCLUDE} ${GRAPHICS_DEFINITION})
CreateModule(Graphics ${GRAPHICS_ALL})

set(APPLICATION_INCLUDE Application/Test.h Application/Triangle.h)
set(APPLICATION_DEFINITION Application/Test.cpp Application/Triangle.cpp)
set(APPLICATION_ALL ${APPLICATION_INCLUDE} ${APPLICATION_GENERAL})
CreateModule(Application ${APPLICATION_ALL})

add_subdirectory(Core)