cmake_minimum_required(VERSION 3.0)

project(DeadlyStagesDemo)

set(SOURCE_FILES
    main.cpp
    ../../game.cpp
    ../../screen/screen.cpp
    ../../screen/main_menu/main_menu_screen.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
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