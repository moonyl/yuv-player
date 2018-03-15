# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (yuv-player)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
 message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
 message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies 
add_subdirectory (glew-2.1.0/build/cmake)
add_subdirectory (glfw)
add_subdirectory (glm)

include_directories(
	glfw/include/
	glew-2.1.0/include/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	glew
)

# yuv-player
add_executable(yuv-player
	player/yuv-player.cpp
)

target_link_libraries(yuv-player
	${ALL_LIBS}
)