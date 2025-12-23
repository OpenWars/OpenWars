cmake_minimum_required(VERSION 3.15)

project(openwars)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

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
	
include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.x)
include_dependency(SDL3_ttf https://github.com/libsdl-org/SDL_ttf.git release-3.2.x)
include_dependency(SDL3_image https://hithub.com/libsdl-org/SDL_image.git release-3.2.x)
include_dependency(minizip https://github.com/zlib-ng/minizip-ng 4.0.10)

# Assets - Doesn't work with include_dependency and I don't have idea why
FetchContent_Declare(OpenWarsAssets
	GIT_REPOSITORY https://github.com/OpenWars/resources
	GIT_TAG main
	GIT_SHALLOW		TRUE
	GIT_PROGRESS	TRUE
)

FetchContent_MakeAvailable(OpenWarsAssets)

set(ASSETS_DIR ${openwarsassets_SOURCE_DIR})
set(ASSETS_PAK ${CMAKE_BINARY_DIR}/assets.pak)

add_custom_command(
    OUTPUT ${ASSETS_PAK}
    COMMAND zip -r ${ASSETS_PAK} . -x "*.git*"
    DEPENDS ${ASSET_FILES}
    WORKING_DIRECTORY ${ASSETS_DIR}
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
	PRIVATE SDL3_image::SDL3_image
	PRIVATE MINIZIP::minizip
	PRIVATE EmbeddedAssets
)
