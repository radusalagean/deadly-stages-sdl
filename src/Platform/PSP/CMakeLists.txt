cmake_minimum_required(VERSION 3.22.1)

project(DeadlyStagesDemo)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../../*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/pspdev/psp/sdk/include/)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES} SDL2_image jpeg png16 z pspdebug
)

if(PSP)
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
endif()