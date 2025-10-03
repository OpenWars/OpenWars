cmake_minimum_required(VERSION 3.15)

project(openwars)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
find_package(SDL3_ttf REQUIRED CONFIG)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} "${sources}")

target_link_libraries(${PROJECT_NAME} 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)