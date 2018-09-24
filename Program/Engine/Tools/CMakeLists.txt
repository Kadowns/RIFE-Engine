include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(TOOLS_INCLUDE Include/Keyboard.h Include/Clock.h Include/Rm.h Include/Types.h)
set(TOOLS_SOURCE  Source/Keyboard.cpp Source/Clock.cpp Source/Rm.cpp)
set(TOOLS_ALL ${TOOLS_INCLUDE} ${TOOLS_SOURCE})
CreateModule(Tools "${TOOLS_ALL}")

target_link_libraries(Tools Graphics)