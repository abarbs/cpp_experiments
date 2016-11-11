cmake_minimum_required(VERSION 3.3)
project(xrandr)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Xrandr REQUIRED)
include_directories(${XRANDR_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
set(EXTERN_PROJECT_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/work")
ExternalProject_Add(
        sdl2
        PREFIX "${EXTERN_PROJECT_PREFIX}/sdl2"
        HG_REPOSITORY http://hg.libsdl.org/SDL/
        HG_TAG release-2.0.3
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}"
        UPDATE_COMMAND ""
)
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include" )
link_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/lib")

add_executable(refreshrates src/refreshrates.cpp)

target_link_libraries(refreshrates ${XRANDR_LIBRARIES} X11 Xinerama)

add_executable(glx src/glx.cpp)
target_link_libraries(glx ${XRANDR_LIBRARIES} X11 GL)

add_executable(sdl-list-displays src/sdl_list_displays.cpp)
target_link_libraries(sdl-list-displays SDL2)
add_dependencies(sdl-list-displays sdl2)
