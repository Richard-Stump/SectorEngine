
cmake_minimum_required( VERSION 3.20.0 )


set( SOURCE_FILES
    Main.cpp

    Renderer/Renderer.cpp
    Renderer/OpenGL.cpp
    Renderer/Shader.cpp

    Resource/MapLoader.cpp
)

set( HEADER_FILES
    Level.hpp

    Renderer/Renderer.hpp
    Renderer/OpenGL.hpp
    Renderer/Shader.hpp

    Resource/MapLoader.hpp

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

    glad
)

message(WARNING ${GLAD_INCLUDE_DIR})

target_include_directories(SectorEngine PRIVATE 
    ${SDL2_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
