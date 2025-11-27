cmake_minimum_required(VERSION 3.15)

project(openwars)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

function(include_dependency depName gitURL gitTag)
	FetchContent_Declare(${depName}
		GIT_REPOSITORY	${gitURL}
		GIT_TAG			${gitTag}
		GIT_SHALLOW		TRUE
		GIT_PROGRESS	TRUE
	)
	
	FetchContent_MakeAvailable(${depName})
endfunction()

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

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} "${sources}")
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_ttf::SDL3_ttf
)
