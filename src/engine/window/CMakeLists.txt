include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(WINDOW_INCLUDE
        include/Window.h  
		include/Scene.h
		include/Database.h
        )
set(WINDOW_SOURCE
		source/Window.cpp
		source/Database.cpp
        )

set(WINDOW_ALL ${WINDOW_INCLUDE} ${WINDOW_SOURCE})
CreateModule(engine window "${WINDOW_ALL}")
target_link_libraries(window tools)