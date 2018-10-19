include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
message("Creating core")
set(APPLICATION_INCLUDE
        Include/Application.h  
		Include/Scene.h
        )
set(APPLICATION_SOURCE
        Source/Application.cpp
        )

set(APPLICATION_ALL ${APPLICATION_INCLUDE} ${APPLICATION_SOURCE})
CreateModule(Application "${APPLICATION_ALL}")
target_link_libraries(Application Tools Graphics)