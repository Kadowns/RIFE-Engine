include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(BASE_INCLUDE
	Include/Component.h
	Include/Object.h
	Include/GameObject.h
	Include/Clock.h
	Include/Event.h
	Include/RifeCore.h
	Include/RifePath.h
	)

set(BASE_SOURCE 
	Source/Component.cpp
	Source/Object.cpp
	Source/GameObject.cpp
	Source/Clock.cpp
	)
set(BASE_ALL ${BASE_INCLUDE} ${BASE_SOURCE})

CreateModule(Engine/Core Base "${BASE_ALL}")

target_link_libraries(Base Math)