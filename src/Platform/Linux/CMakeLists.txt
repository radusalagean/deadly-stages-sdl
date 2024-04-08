cmake_minimum_required(VERSION 3.22.1)

project(DeadlyStagesDemo)

set(SOURCES
    "${CMAKE_SOURCE_DIR}/../../Platform/Linux/main.cpp"
    "${CMAKE_SOURCE_DIR}/../../Game.cpp"
    "${CMAKE_SOURCE_DIR}/../../Core/ImageAsset.cpp"
    "${CMAKE_SOURCE_DIR}/../../Debug/Logger.cpp"
    "${CMAKE_SOURCE_DIR}/../../Screen/Screen.cpp"
    "${CMAKE_SOURCE_DIR}/../../Screen/MainMenu/MainMenuScreen.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/pspdev/psp/sdk/include/)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES} SDL2_image
)
