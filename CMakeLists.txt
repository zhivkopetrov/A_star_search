#author Zhivko Petrov

cmake_minimum_required(VERSION 3.5.1)


#set global variables for the whole build system

set(PROJECT game)
project(${PROJECT})


set(PROJECT_ROOT_DIR "${${PROJECT}_SOURCE_DIR}")


#default build type
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)


set(WARNING_FLAGS "-Wall -Wextra -Werror -Wfloat-equal -Wundef -Wuninitialized")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wreorder -Wshadow -Wpointer-arith")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wcast-align -Wcast-qual")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wunused-parameter")


#set global compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")


#includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH} "${PROJECT_ROOT_DIR}/cmake")


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)


#include general include directories. This will affect all target includes
#that are invoked with add_directory command.
include_directories(${PROJECT_ROOT_DIR}
                    ${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${SDL2_MIXER_INCLUDE_DIR})
                    
                    
#file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "${PROJECT_ROOT_DIR}/*.cpp"
                  "${PROJECT_ROOT_DIR}/utils/*.cpp"
                  "${PROJECT_ROOT_DIR}/utils/input/*.cpp"
                  "${PROJECT_ROOT_DIR}/utils/time/*.cpp"
                  "${PROJECT_ROOT_DIR}/utils/drawing/*.cpp"
                  "${PROJECT_ROOT_DIR}/sdl/*.cpp"
                  "${PROJECT_ROOT_DIR}/sdl/containers/*.cpp"
                  "${PROJECT_ROOT_DIR}/managers/*.cpp"
                  "${PROJECT_ROOT_DIR}/gameentities/*.cpp"
                  "${PROJECT_ROOT_DIR}/gameentities/animators/*.cpp"
                  "${PROJECT_ROOT_DIR}/gameentities/optionsmenu/*.cpp"
                  "${PROJECT_ROOT_DIR}/pathfinding/*.cpp"
                  "${PROJECT_ROOT_DIR}/common/*.cpp")
                   
                   
#generate project binary
add_executable(${PROJECT}
               ${SOURCES})


#adding -std=c++14 flag
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 17)


target_link_libraries(${PROJECT}
                      ${SDL2_LIBRARY}              # -lSDL2 flag
                      ${SDL2_IMAGE_LIBRARY}        # -lSDL2_image flag
                      ${SDL2_TTF_LIBRARY}          # -lSDL2_ttf flag
                      ${SDL2_MIXER_LIBRARY}        # -lSDL2_mixer flag
                      m)                           # -lm flag
         




