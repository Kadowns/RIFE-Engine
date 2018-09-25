include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(UTILS_INCLUDE Include/Rm.h Include/Types.h)
set(UTILS_SOURCE Source/Rm.cpp)
set(UTILS_ALL ${UTILS_INCLUDE} ${UTILS_SOURCE})
CreateModule(Utils "${UTILS_ALL}")