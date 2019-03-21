include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(BASE_INCLUDE
	include/Component.h
	include/Object.h
	include/GameObject.h
	include/Clock.h
	include/Event.h
	include/Scene.h
	include/RifeCore.h
	include/RifePath.h
	)

set(BASE_SOURCE 
	source/Component.cpp
	source/Object.cpp
	source/GameObject.cpp
	source/Clock.cpp
	source/Scene.cpp
	)
set(BASE_ALL ${BASE_INCLUDE} ${BASE_SOURCE})

CreateModule(engine/core base "${BASE_ALL}")

target_link_libraries(base math)