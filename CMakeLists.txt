cmake_minimum_required(VERSION 3.15)
project(Memorama VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

message(STATUS "=== SDL2 Memorama Game - Auto Configuration ===")

# Auto-detect SDL2 installation
set(SDL2_FOUND FALSE)
set(SDL2_SEARCH_PATHS
    "C:/Users/$ENV{USERNAME}/SDL2"
    "C:/SDL2"
    "C:/dev/SDL2"
    "C:/msys64/mingw64"
    "C:/mingw64"
)

# Find SDL2 in various structures
foreach(SEARCH_PATH ${SDL2_SEARCH_PATHS})
    if(EXISTS "${SEARCH_PATH}")
        message(STATUS "Checking: ${SEARCH_PATH}")

        # Check for MinGW structure (separated SDL2/SDL2_ttf)
        if(EXISTS "${SEARCH_PATH}/SDL2/x86_64-w64-mingw32/include/SDL2/SDL.h")
            set(SDL2_ROOT_DIR "${SEARCH_PATH}")
            set(SDL2_INCLUDE_DIR "${SEARCH_PATH}/SDL2/x86_64-w64-mingw32/include/SDL2")
            set(SDL2_LIBRARY_DIR "${SEARCH_PATH}/SDL2/x86_64-w64-mingw32/lib")
            set(SDL2_BINARY_DIR "${SEARCH_PATH}/SDL2/x86_64-w64-mingw32/bin")
            set(SDL2_TTF_INCLUDE_DIR "${SEARCH_PATH}/SDL2_ttf/x86_64-w64-mingw32/include/SDL2")
            set(SDL2_TTF_LIBRARY_DIR "${SEARCH_PATH}/SDL2_ttf/x86_64-w64-mingw32/lib")
            set(SDL2_TTF_BINARY_DIR "${SEARCH_PATH}/SDL2_ttf/x86_64-w64-mingw32/bin")
            set(SDL2_STRUCTURE "MinGW-Separated")
            set(SDL2_FOUND TRUE)
            break()
        endif()

        # Check for standard MinGW structure
        if(EXISTS "${SEARCH_PATH}/include/SDL2/SDL.h")
            set(SDL2_ROOT_DIR "${SEARCH_PATH}")
            set(SDL2_INCLUDE_DIR "${SEARCH_PATH}/include/SDL2")
            set(SDL2_LIBRARY_DIR "${SEARCH_PATH}/lib")
            set(SDL2_BINARY_DIR "${SEARCH_PATH}/bin")
            set(SDL2_TTF_INCLUDE_DIR "${SEARCH_PATH}/include/SDL2")
            set(SDL2_TTF_LIBRARY_DIR "${SEARCH_PATH}/lib")
            set(SDL2_TTF_BINARY_DIR "${SEARCH_PATH}/bin")
            set(SDL2_STRUCTURE "MinGW-Single")
            set(SDL2_FOUND TRUE)
            break()
        endif()

        # Check for MSVC structure
        if(EXISTS "${SEARCH_PATH}/SDL2/include/SDL.h")
            set(SDL2_ROOT_DIR "${SEARCH_PATH}")
            set(SDL2_INCLUDE_DIR "${SEARCH_PATH}/SDL2/include")
            set(SDL2_LIBRARY_DIR "${SEARCH_PATH}/SDL2/lib/x64")
            set(SDL2_BINARY_DIR "${SEARCH_PATH}/SDL2/lib/x64")
            set(SDL2_TTF_INCLUDE_DIR "${SEARCH_PATH}/SDL2_ttf/include")
            set(SDL2_TTF_LIBRARY_DIR "${SEARCH_PATH}/SDL2_ttf/lib/x64")
            set(SDL2_TTF_BINARY_DIR "${SEARCH_PATH}/SDL2_ttf/lib/x64")
            set(SDL2_STRUCTURE "MSVC")
            set(SDL2_FOUND TRUE)
            break()
        endif()
    endif()
endforeach()

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found! Please install SDL2 development libraries.")
endif()

message(STATUS "Found SDL2: ${SDL2_STRUCTURE} structure")
message(STATUS "SDL2 Root: ${SDL2_ROOT_DIR}")
message(STATUS "SDL2 Include: ${SDL2_INCLUDE_DIR}")

# Find libraries
if(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_library(SDL2_LIBRARY NAMES SDL2 libSDL2.dll.a PATHS ${SDL2_LIBRARY_DIR} NO_DEFAULT_PATH)
    find_library(SDL2MAIN_LIBRARY NAMES SDL2main libSDL2main.a PATHS ${SDL2_LIBRARY_DIR} NO_DEFAULT_PATH)
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf libSDL2_ttf.dll.a PATHS ${SDL2_TTF_LIBRARY_DIR} NO_DEFAULT_PATH)
else()
    find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${SDL2_LIBRARY_DIR} NO_DEFAULT_PATH)
    find_library(SDL2MAIN_LIBRARY NAMES SDL2main PATHS ${SDL2_LIBRARY_DIR} NO_DEFAULT_PATH)
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS ${SDL2_TTF_LIBRARY_DIR} NO_DEFAULT_PATH)
endif()

if(NOT SDL2_LIBRARY OR NOT SDL2MAIN_LIBRARY)
    message(FATAL_ERROR "SDL2 libraries not found!")
endif()

# Set up libraries with correct order for MinGW
set(SDL_LIBRARIES ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})
if(SDL2_TTF_LIBRARY)
    list(APPEND SDL_LIBRARIES ${SDL2_TTF_LIBRARY})
endif()

message(STATUS "SDL2 Libraries: ${SDL_LIBRARIES}")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${SDL2_INCLUDE_DIR})
if(SDL2_TTF_INCLUDE_DIR AND NOT "${SDL2_TTF_INCLUDE_DIR}" STREQUAL "${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_TTF_INCLUDE_DIR})
endif()

# Source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set console subsystem for MinGW
if(WIN32 AND MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "-Wl,--subsystem,console"
    )
endif()

# Link libraries with correct order for MinGW
if(WIN32 AND MINGW)
    # Critical: mingw32 must come BEFORE SDL2main for MinGW
    target_link_libraries(${PROJECT_NAME} mingw32 ${SDL_LIBRARIES} user32 gdi32 winmm)
else()
    target_link_libraries(${PROJECT_NAME} ${SDL_LIBRARIES})
endif()

# Copy DLLs and data (Windows only)
if(WIN32)
    if(EXISTS "${SDL2_BINARY_DIR}/SDL2.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_BINARY_DIR}/SDL2.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()

    if(EXISTS "${SDL2_TTF_BINARY_DIR}/SDL2_ttf.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_BINARY_DIR}/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()

    # Copy data directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data")
endif()

# Compiler settings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

message(STATUS "=== Configuration Complete ===")
message(STATUS "Ready to build!")