include(${CMAKE_HOME_DIRECTORY}/cmake/PrintProperties.cmake)
function(CreateModule MODULE_NAME MODULE_SOURCES)
    set(MODULE_PATH ${CMAKE_HOME_DIRECTORY}/Engine/${MODULE_NAME})
    message("Creating module ${MODULE_NAME} (${CMAKE_CURRENT_SOURCE_DIR}) (${MODULE_SOURCES})")
    add_library(${MODULE_NAME} ${MODULE_SOURCES})
    target_include_directories(${MODULE_NAME}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            )
    set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    print_target_properties(${MODULE_NAME})
endfunction(CreateModule)