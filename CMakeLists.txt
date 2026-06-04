cmake_minimum_required(VERSION 3.20)
project(Bloomfall LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE BLOOMFALL_SOURCES CONFIGURE_DEPENDS
    src/game/*.cpp
    src/engine/*.cpp
    src/main.cpp
)

add_executable(Bloomfall ${BLOOMFALL_SOURCES})

target_include_directories(Bloomfall
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
)

target_link_libraries(Bloomfall
    PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        glad::glad
        glm::glm
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(Bloomfall PRIVATE -Wall -Wextra -Wno-unused-parameter)
elseif(MSVC)
    target_compile_options(Bloomfall PRIVATE /W4 /wd4100 /wd4996 /MP)
endif()
