include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(BASE_INCLUDE
	Include/Component.h
	Include/Object.h
	Include/GameObject.h
	Include/RifeCore.h
	)
set(BASE_SOURCE 
	Source/Component.cpp
	Source/Object.cpp
	Source/GameObject.cpp
	)
set(BASE_ALL ${BASE_INCLUDE} ${BASE_SOURCE})

CreateModule(Engine/Core Base "${BASE_ALL}")

target_link_libraries(Base Math)