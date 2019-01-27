include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(TOOLS_INCLUDE
	include/Keyboard.h
	include/Mouse.h
	include/Random.h
	)

set(TOOLS_SOURCE 
	source/Keyboard.cpp
	source/Mouse.cpp
	source/Random.cpp
	)

set(TOOLS_ALL ${TOOLS_INCLUDE} ${TOOLS_SOURCE})

CreateModule(engine tools "${TOOLS_ALL}")

target_link_libraries(tools graphics)