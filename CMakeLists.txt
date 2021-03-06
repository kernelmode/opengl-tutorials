cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -std=c++11")
project(opengl-tutorials)

# Config variable for CLion IDE. Makes CLion place output files in $HOME/Projects/opengl-tutorials/bin for relative paths in examples to work
set(LOGLE_CLION_CONFIG ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(LOGLE_CLION_CONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/Projects/opengl-tutorials/bin)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

set(LOGLE_WINDOWS_DEPS ${CMAKE_SOURCE_DIR}/lib/libsoil2.lib ${CMAKE_SOURCE_DIR}/lib/glfw3.lib opengl32.lib)
set(LOGLE_UNIX_DEPS ${GLFW_LIBRARIES} glfw3 ${CMAKE_SOURCE_DIR}/lib/libsoil2.a)

add_subdirectory(glfw-3.0.4)
add_subdirectory(core)
add_subdirectory(shader)
add_subdirectory(render)
add_subdirectory(input)
include_directories(core shader render input glfw-3.0.4/include glm .)

add_executable(example Example.cpp)

if(UNIX)
	message(STATUS "Linking libraries for linux...")
        message(STATUS ${GLFW_LIBRARIES})
	target_link_libraries(example ${LOGLE_UNIX_DEPS} render core)
elseif(WIN32)
	message(STATUS "Linking libraries for windows...")
	target_link_libraries(example render core ${LOGLE_WINDOWS_DEPS})
else()
	message(SEND_ERROR "Sorry, this OS is not supported yet")
endif()