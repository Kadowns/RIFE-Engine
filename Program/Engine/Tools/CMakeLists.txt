include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(TOOLS_INCLUDE Include/Keyboard.h Include/Clock.h)
set(TOOLS_SOURCE  Source/Keyboard.cpp Source/Clock.cpp)
set(TOOLS_ALL ${TOOLS_INCLUDE} ${TOOLS_SOURCE})
CreateModule(Engine Tools "${TOOLS_ALL}")

target_link_libraries(Tools Graphics)