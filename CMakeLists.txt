cmake_minimum_required(VERSION 3.19)
project(SoftwareRenderer)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2/lib/x64
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL2
    SDL2main
    glfw3dll
    opengl32
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    OUTPUT_NAME_DEBUG   "${PROJECT_NAME}_debug"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}_release"
)
