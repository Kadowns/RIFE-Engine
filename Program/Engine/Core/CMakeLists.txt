include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
message("Creating core")
set(CORE_INCLUDE
        Include/Application.h  
		Include/Scene.h
        )
set(CORE_SOURCE
        Source/Application.cpp
        )

set(CORE_ALL ${CORE_INCLUDE} ${CORE_SOURCE})
CreateModule(Core "${CORE_ALL}")
target_link_libraries(Core Tools Graphics)