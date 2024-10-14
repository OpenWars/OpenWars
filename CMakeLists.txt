cmake_minimum_required(VERSION 3.15)

project(openwars)
set(CMAKE_CXX_STANDARD 17)

find_package(raylib 4.5 REQUIRED)
find_package(Lua REQUIRED)
add_subdirectory(sol2)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/src/zyx64sum.cpp")

add_executable(${PROJECT_NAME} "${sources}")

target_link_libraries(${PROJECT_NAME} raylib sol2 ${LUA_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
