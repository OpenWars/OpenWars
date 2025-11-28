cmake_minimum_required(VERSION 3.15)

project(openwars)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Dependencies
function(include_dependency depName gitURL gitTag)
	FetchContent_Declare(${depName}
		GIT_REPOSITORY	${gitURL}
		GIT_TAG			${gitTag}
		GIT_SHALLOW		TRUE
		GIT_PROGRESS	TRUE
	)
	
	FetchContent_MakeAvailable(${depName})
endfunction()

## SDL
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
	message(STATUS "Downloading SDL3 from GitHub")
	include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.x)
else()
	message(STATUS "Using local SDL3")
endif()

find_package(SDL3_ttf QUIET)
if(NOT SDL3_ttf_FOUND)
	message(STATUS "Downloading SDL3_ttf from GitHub")
	include_dependency(SDL3_ttf https://github.com/libsdl-org/SDL_ttf.git release-3.2.x)
else()
	message(STATUS "Using local SDL3_ttf")
endif()

## MiniZIP
find_package(minizip QUIET)
if(NOT minizip_FOUND)
	message(STATUS "Downloading MiniZip from GitHub")
	include_dependency(minizip-ng https://github.com/zlib-ng/minizip-ng 4.0.10)
else()
	message(STATUS "Using local MiniZip")
endif()

# Assets
include_dependency(OpenWarsAssets https://github.com/OpenWars/resources main)

set(ASSETS_DIR ${openwarsassets_SOURCE_DIR})
set(ASSETS_PAK ${CMAKE_BINARY_DIR}/assets.pak)

add_custom_command(
    OUTPUT ${ASSETS_PAK}
    COMMAND ${CMAKE_COMMAND} -E tar cf ${ASSETS_PAK} --format=zip ${ASSETS_DIR}
    DEPENDS ${ASSETS_DIR}
)

set(ASSETS_CPP ${CMAKE_BINARY_DIR}/assets_pak.cpp)

add_custom_command(
    OUTPUT ${ASSETS_CPP}
    COMMAND xxd -i assets.pak > ${ASSETS_CPP}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${ASSETS_PAK}
)

add_library(EmbeddedAssets ${ASSETS_CPP})
target_compile_options(EmbeddedAssets PRIVATE -fPIC)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} "${sources}")
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_ttf::SDL3_ttf
	PRIVATE minizip
	PRIVATE EmbeddedAssets
)
