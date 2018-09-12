include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
message("Creating app")
set(APPLICATION_INCLUDE
        Include/Application.h  
        Include/Test.h 
		Include/Triangle.h
		Include/Scene.h
        )
set(APPLICATION_SOURCE
        Source/Application.cpp
        Source/Test.cpp
		Source/Triangle.cpp
        )

set(APPLICATION_ALL ${APPLICATION_INCLUDE} ${APPLICATION_SOURCE})
CreateModule(Application "${APPLICATION_ALL}")
target_link_libraries(Application Tools Graphics)