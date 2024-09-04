cmake_minimum_required( VERSION 3.20.0 )

project(SectorEngine)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

# Add all of the packages that we need to build our project.
# vcpkg should find them all for us
find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

#set(ALL_LIBRARY_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/")

find_path(SDL2_INCLUDE_DIR NAMES SDL.h SDL_Main.h)
find_path(GLM_INCLUDE_DIR NAMES glm.hpp PATH_SUFFIXES glm)

add_subdirectory(lib/)

# Our individual sub-projects
add_subdirectory(Game/)

file(GLOB_RECURSE GameDataFiles ${CMAKE_SOURCE_DIR}/GameData/*)

message(warning ${GameDataFiles})

add_custom_target(GameData SOURCES
	${GameDataFiles}
)


