cmake_minimum_required(VERSION 3.28)
project(ChessIP LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

# Copy assets to build directory
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})


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
    ${CMAKE_SOURCE_DIR}/src/SoundPlayer.cpp
)

# Header files
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/Board.h
    ${CMAKE_SOURCE_DIR}/src/Renderer.h
    ${CMAKE_SOURCE_DIR}/src/ResourceManager.h
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/Utils.h
    ${CMAKE_SOURCE_DIR}/src/SoundPlayer.h
)

add_executable(main ${SRC_FILES} ${HEADER_FILES})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio SFML::Network)

# Copy assets to build directory
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_DIR}"
        "$<TARGET_FILE_DIR:main>/assets"
    COMMENT "Copying assets to build directory"
)

# For MSVC to copy DLLs automatically
if(WIN32 AND MSVC)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SFML::Graphics>"
            "$<TARGET_FILE_DIR:main>"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SFML::Audio>"
            "$<TARGET_FILE_DIR:main>"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SFML::Network>"
            "$<TARGET_FILE_DIR:main>"
    )
endif()