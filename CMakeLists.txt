CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(Pacman)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    SET(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2/macos_arm64)
    SET(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${SDL2_LIB_DIR}/sdl2/2.30.9/lib/cmake/SDL2")
    SET(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${SDL2_LIB_DIR}/sdl2_image/2.8.2_2/lib/cmake/SDL2_image")
    SET(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${SDL2_LIB_DIR}/sdl2_mixer/2.8.0/lib/cmake/SDL2_mixer")
    SET(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${SDL2_LIB_DIR}/sdl2_ttf/2.22.0/lib/cmake/SDL2_ttf")

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)

ELSEIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    # Compiler GCC
    SET(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++")
    SET(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc")

    SET(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2/windows)
    SET(SDL2_LIBRARY ${SDL2_LIB_DIR}/SDL2.lib)
    SET(SDL2_IMAGE_LIBRARY ${SDL2_LIB_DIR}/SDL2_image.lib)
    SET(SDL2_TTF_LIBRARY ${SDL2_LIB_DIR}/SDL2_ttf.lib)
    SET(SDL2_MIXER_LIBRARY ${SDL2_LIB_DIR}/SDL2_mixer.lib)
    SET(SDL2_MAIN_LIBRARY ${SDL2_LIB_DIR}/SDL2main.lib)
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported platform: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")  # Flag -g para depuración
endif()

# Modules, Source and Executable
SET(SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/src")
FILE(GLOB_RECURSE MODULES_SOURCES ${SOURCE_DIR}/*.cppm ${SOURCE_DIR}/*.cpp)
SET(SOURCES ${MODULES_SOURCES} "${CMAKE_SOURCE_DIR}/main.cpp")
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})

# Include Lib headers (Like SDL)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/SDL2/include)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/code/include)

# Resources for copying them into Binary DIR
SET(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
SET(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")
ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
)

# Windows needs DLL files to be in the Binary DIR
IF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    FOREACH(DLL_FILENAME SDL2.dll SDL2_image.dll SDL2_ttf.dll SDL2_mixer.dll)
    ADD_CUSTOM_COMMAND(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_LIB_DIR}/${DLL_FILENAME}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    ENDFOREACH()
ENDIF()

IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    SET(MACOS_FRAMEWORKS 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreFoundation" 
        "-framework CoreVideo"
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework AVFoundation"
        "-framework GameController"
        "-framework Metal"
        "-framework Foundation"
        "-framework ForceFeedback"
        "-framework CoreHaptics"
        "-framework Carbon")

    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
        ${MACOS_FRAMEWORKS})
ELSEIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        PRIVATE
        ${SDL2_MAIN_LIBRARY}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})
ENDIF()

MESSAGE(STATUS "C++ standard set to: ${CMAKE_CXX_STANDARD}")