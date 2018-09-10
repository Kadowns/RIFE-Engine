add_library(GLFW STATIC IMPORTED GLOBAL)
target_include_directories(GLFW
        INTERFACE
        ${CMAKE_HOME_DIRECTORY}/libs/glfw/include
        )
set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/libs/glfw/lib-vc2015/glfw3.lib)
set(GLFW_FOUND True)