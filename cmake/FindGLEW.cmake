add_library(GLEW STATIC IMPORTED GLOBAL)
target_include_directories(GLEW
        INTERFACE
        ${CMAKE_HOME_DIRECTORY}/libs/glew/include
        )
set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/libs/glew/lib/Release/x64/glew32.lib)
set(GLEW_FOUND True)