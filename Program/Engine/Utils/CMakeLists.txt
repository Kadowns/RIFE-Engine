include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Eigen3 3.3 REQUIRED)

set(UTILS_INCLUDE Include/Rm.h Include/Types.h)
set(UTILS_SOURCE Source/Rm.cpp)
set(UTILS_ALL ${UTILS_INCLUDE} ${UTILS_SOURCE})
CreateModule(Utils "${UTILS_ALL}")

IF(NOT EIGEN3_FOUND)
	message("NÃO ACHEI ESSA CARALHA ${EIGEN3_ROOT_DIR}")

ELSE()
    target_link_libraries(Utils Eigen3::Eigen)
    message("ACHEI ESSA CARALHA $ENV{EIGEN3_DEFINITIONS}")
ENDIF()

