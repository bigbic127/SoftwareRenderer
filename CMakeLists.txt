cmake_minimum_required(VERSION 3.19)
project(SoftwareRenderer)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm"
)

add_executable(${PROJECT_NAME} ${SOURCES})

include(FetchContent)
#stb image
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)
add_library(STB_LIB INTERFACE)
target_include_directories(STB_LIB INTERFACE
    ${stb_SOURCE_DIR}
)
#tiny gltf
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
#SDL
FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.8
)
set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(sdl2)
#ImGUI
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.1
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_SRC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
    ${imgui_SOURCE_DIR}/imconfig.h
)
add_library(IMGUI_LIB STATIC ${IMGUI_SRC})
target_include_directories(IMGUI_LIB PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
if(APPLE)
    target_link_libraries(IMGUI_LIB PUBLIC SDL2)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        IMGUI_LIB
        GLTF_LIB
        STB_LIB
        "-framework Cocoa"
        "-framework AppKit"
        "-framework UniformTypeIdentifiers"
    )
elseif(WIN32)
    target_link_libraries(IMGUI_LIB PUBLIC SDL2 SDL2main)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        IMGUI_LIB
        GLTF_LIB
        STB_LIB
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    OUTPUT_NAME_DEBUG   "${PROJECT_NAME}_debug"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}_release"
)
if(MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()
