cmake_minimum_required(VERSION 3.19)
project(SoftwareRenderer)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

include(FetchContent)
FetchContent_Declare(
    gltf
    GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
    GIT_TAG v2.9.6
)
FetchContent_MakeAvailable(gltf)
set(GLTF_SRC
    ${gltf_SOURCE_DIR}/tiny_gltf.cc
)
add_library(GLTF_LIB STATIC ${GLTF_SRC})
target_include_directories(GLTF_LIB PUBLIC
    ${gltf_SOURCE_DIR}
)
FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.8
)
set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(sdl2)

if(APPLE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)

elseif(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2
        SDL2main
        GLTF_LIB
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    OUTPUT_NAME_DEBUG   "${PROJECT_NAME}_debug"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}_release"
)