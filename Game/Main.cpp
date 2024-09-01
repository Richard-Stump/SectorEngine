#include <iostream> 
#include <memory>
#include <filesystem>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Resource/MapLoader.hpp"

#include "Level.hpp"

/**
 * @brief Constructs a hard-coded test level with just a few sectors.
 * 
 * @details This test level contains a limited number of rectangular textures intended for testing the level
 *          format, and basic features of the rendering code. 
 */
std::unique_ptr<Level> buildTestLevel()
{
    auto level = std::make_unique<Level>();

// Add all the verticies for the level
    level->vertices.push_back(Vertex{ -32.0, -32.0 });  // Y = -32
    level->vertices.push_back(Vertex{  32.0, -32.0 });
    
    level->vertices.push_back(Vertex{ -96.0,  32.0 });  // Y = 32
    level->vertices.push_back(Vertex{ -32.0,  32.0 });
    level->vertices.push_back(Vertex{  32.0,  32.0 });
    level->vertices.push_back(Vertex{  96.0,  32.0 });

    level->vertices.push_back(Vertex{ -96.0,  96.0 });  // Y = 96
    level->vertices.push_back(Vertex{ -32.0,  96.0 });
    level->vertices.push_back(Vertex{  32.0,  96.0 });
    level->vertices.push_back(Vertex{  96.0,  96.0 });

    level->vertices.push_back(Vertex{ -96.0,  160.0 });  // Y = 160
    level->vertices.push_back(Vertex{ -32.0,  160.0 });
    level->vertices.push_back(Vertex{  32.0,  160.0 });
    level->vertices.push_back(Vertex{  96.0,  160.0 });

    level->vertices.push_back(Vertex{ -96.0,  224.0 });  // Y = 224
    level->vertices.push_back(Vertex{  96.0,  224.0 });


// Add all the walls, going in CCW winding order for defining the insideof the sectors
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 1, Wall::NO_BEHIND });    // Sector 0 (Done)
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 1, 2, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 4, 3, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 3, 0, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });    // Sector 1
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });    // Sector 2
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });    // Sector 3
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });    // Sector 4
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 0, 0, Wall::NO_BEHIND });

// Add all the sectors
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},  0.0,    64.0,   0,    4 }); 
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},  0.0,    64.0,   4,    8 });
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},  0.0,    64.0,   12,   4 });
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},  0.0,    64.0,   16,   4 });
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},  0.0,    64.0,   20,   6 });

    return std::move(level);
}

std::vector<std::string> getFilesystemLevels()
{
    return std::vector<std::string>();
}

std::unique_ptr<Level> getLevelSelection()
{
    std::cout << "Select the level you want to play\n";
    std::cout << "  0 - Hardcoded Test Level";

    std::vector<std::string> levels = getFilesystemLevels();

    for (size_t i = 1; i <= levels.size(); i++) {
        std::cout << std::format("  {} - {}", i, levels[i]) << "\n";
    }

    std::cout << std::endl;

    return std::move(buildTestLevel());
}

int main(int argc, char** argv)
{
    std::unique_ptr<Level> level = getLevelSelection();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initalize SDL2! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow("Sector Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        std::cerr << "Could not create window! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    try {
        DoomMapLoader loader;
        std::unique_ptr<Level> level = loader.loadLevel("Maps/Square.wad");
    }
    catch (std::exception e) {
        std::cerr << "Could not load level:\n" << e.what() << std::endl;
        std::cerr << "Aborting...";
        return -1;
    }


    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_Delay(100);
    }


    return 0;
}