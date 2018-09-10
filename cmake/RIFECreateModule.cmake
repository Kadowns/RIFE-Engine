function(CreateModule MODULE_NAME MODULE_SOURCES)
    set(MODULE_PATH ${CMAKE_HOME_DIRECTORY}/Engine/${MODULE_NAME})
    message("Creating module ${MODULE_NAME} (${MODULE_PATH}) (${MODULE_SOURCES})")
    add_library(${MODULE_NAME} ${MODULE_SOURCES})
    target_include_directories(${MODULE_NAME}
            PUBLIC
            $<BUILD_INTERFACE:${MODULE_PATH}/Include>
            $<INSTALL_INTERFACE:Include>
            ${MODULE_PATH}/Include
            INTERFACE
            ${MODULE_PATH}/Source
            )
    set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
endfunction(CreateModule)