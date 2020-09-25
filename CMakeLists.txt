cmake_minimum_required(VERSION 3.10.0)
project(ezy)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "src/*.c")

if (CMAKE_HOST_APPLE)
  list(APPEND SOURCES "src/libs.m")
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
  SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY})
endif()

add_executable(ezy ${SOURCES})
set_property(TARGET ezy PROPERTY C_STANDARD 99)
target_link_libraries(ezy ${EXTRA_LIBS})

