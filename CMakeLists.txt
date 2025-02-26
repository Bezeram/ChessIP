cmake_minimum_required(VERSION 3.28)
project(ChessIP LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Source files
set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Board.cpp
    ${CMAKE_SOURCE_DIR}/src/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/ResourceManager.cpp
    ${CMAKE_SOURCE_DIR}/src/Application.cpp
)

# Header files
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/Board.h
    ${CMAKE_SOURCE_DIR}/src/Renderer.h
    ${CMAKE_SOURCE_DIR}/src/ResourceManager.h
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/Utils.h
)

# Grouping files into filters
source_group("src/Sources" FILES ${SRC_FILES})
source_group("src/Headers" FILES ${HEADER_FILES})

add_executable(main ${SRC_FILES} ${HEADER_FILES})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio SFML::Network)

