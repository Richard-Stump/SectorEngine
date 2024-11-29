
cmake_minimum_required( VERSION 3.20.0 )


set( SOURCE_FILES
    Main.cpp

    Renderer/Renderer.cpp
    Renderer/OpenGL.cpp
    Renderer/Shader.cpp

    Resource/MapLoader.cpp
    Resource/WadFile.cpp
)

set( HEADER_FILES
    Level.hpp

    Renderer/Renderer.hpp
    Renderer/OpenGL.hpp
    Renderer/Shader.hpp

    Resource/MapLoader.hpp
    Resource/WadFile.hpp
    
    Utility/Timer.hpp
)

add_executable(SectorEngine
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(SectorEngine 
    SDL2::SDL2 
    SDL2::SDL2main
    glm::glm
    imgui::imgui

    glad
)

target_include_directories(SectorEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${CDT_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
)
