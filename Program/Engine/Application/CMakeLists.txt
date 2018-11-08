include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(APPLICATION_INCLUDE
        Include/Application.h  
		Include/Scene.h
		Include/Database.h
        )
set(APPLICATION_SOURCE
        Source/Application.cpp
		Source/Database.cpp
        )

set(APPLICATION_ALL ${APPLICATION_INCLUDE} ${APPLICATION_SOURCE})
CreateModule(Engine Application "${APPLICATION_ALL}")
target_link_libraries(Application Tools Graphics)