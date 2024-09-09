cmake_minimum_required(VERSION 3.22.1)

project(DeadlyStagesDemo)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/../..")

# Find all .cpp files recursively in the base directory
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Remove files in the Platform directory
list(FILTER SOURCES EXCLUDE REGEX "${SRC_DIR}/Platform/.*")

# Find all .cpp files recursively in the Platform/PSP directory
file(GLOB_RECURSE PSP_SOURCES "${SRC_DIR}/Platform/PSP/*.cpp")

# Manually include the PSP files in the SOURCES list
list(APPEND SOURCES ${PSP_SOURCES})

# Optionally, print the list of found sources (for debugging)
# message(STATUS "Found source files: ${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ~/pspdev/psp/sdk/include/)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES} SDL2_image SDL2_ttf jpeg png16 z pspdebug
)

set(BUILD_PRX 1)
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
)