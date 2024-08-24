cmake_minimum_required( VERSION 3.20.0 )

project(SectorEngine)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

# Add all of the packages that we need to build our project.
# vcpkg should find them all for us
#find_package(glm CONFIG REQUIRED)
#find_package(imgui CONFIG REQUIRED)
#find_package(glfw3 CONFIG REQUIRED)

#set(ALL_LIBRARY_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/")

#find_path(GLFW_INCLUDE_DIR NAMES glfw3.h PATH_SUFFIXES GLFW)
#find_path(GLM_INCLUDE_DIR NAMES glm.hpp PATH_SUFFIXES glm)
#find_path(IMGUI_INCLUDE_DIR NAMES imgui.h PATH_SUFFIXES imgui)

# Our individual sub-projects
add_subdirectory(Game/)


