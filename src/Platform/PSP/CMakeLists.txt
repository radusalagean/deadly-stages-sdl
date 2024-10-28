cmake_minimum_required(VERSION 3.22.1)

project(DeadlyStagesRemix)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/../..")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

# Find all .cpp files recursively in the base directory
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Remove files in the Platform directory
list(FILTER SOURCES EXCLUDE REGEX "${SRC_DIR}/Platform/.*")

# Find all .cpp files recursively in the Platform/PSP directory
file(GLOB_RECURSE PSP_SOURCES "${SRC_DIR}/Platform/PSP/*.cpp")

# Add tinyxml2 to the SOURCES list
list(APPEND SOURCES "${SRC_DIR}/../vendor/tinyxml2/tinyxml2.cpp")   

# Manually include the PSP files in the SOURCES list
list(APPEND SOURCES ${PSP_SOURCES})

# Optionally, print the list of found sources (for debugging)
# message(STATUS "Found source files: ${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ~/pspdev/psp/sdk/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR}/../vendor/tinyxml2)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2main SDL2_ttf SDL2_mixer SDL2 GL freetype png z pspvram pspdisplay pspvfpu pspgu pspge pspdebug psphprm pspctrl pspaudio harfbuzz bz2 SDL2_image jpeg png16 modplug vorbis vorbisfile vorbisenc ogg
)

create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH ${CMAKE_SOURCE_DIR}/ICON0.PNG
    BACKGROUND_PATH ${CMAKE_SOURCE_DIR}/PIC1.PNG
    PREVIEW_PATH NULL
    TITLE "Deadly Stages Remix"
    BUILD_PRX 1
)
