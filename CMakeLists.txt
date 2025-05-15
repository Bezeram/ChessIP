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

# Pieces header files
set (PIECES_HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/pieces/AllPiecesHeader.h
    ${CMAKE_SOURCE_DIR}/src/pieces/BasePiece.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteWitch.h
    ${CMAKE_SOURCE_DIR}/src/pieces/King.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteKnight.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteAlchemist.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteArcher.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteBuilder.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteGrimReaper.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhitePlagueDoctor.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteRogue.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteTrebuchet.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteWarlord.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteSamurai.h
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteDragon.h
)

# Pieces source files
set (PIECES_SOURCE_FILES
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteWitch.cpp
    ${CMAKE_SOURCE_DIR}/src/pieces/King.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteKnight.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteAlchemist.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteArcher.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteBuilder.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteGrimReaper.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhitePlagueDoctor.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteRogue.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteTrebuchet.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteWarlord.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteSamurai.cpp
    
    ${CMAKE_SOURCE_DIR}/src/pieces/WhiteDragon.cpp
)

# Source files
set(SOURCE_FILES
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
source_group("src/Core/Sources" FILES ${SOURCE_FILES})
source_group("src/Core/Headers" FILES ${HEADER_FILES})
source_group("src/pieces/Sources" FILES ${PIECES_SOURCE_FILES})
source_group("src/pieces/Headers" FILES ${PIECES_HEADER_FILES})

add_executable(main ${SOURCE_FILES} ${HEADER_FILES} ${PIECES_SOURCE_FILES} ${PIECES_HEADER_FILES})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio SFML::Network)
