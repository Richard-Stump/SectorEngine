#include <iostream> 
#include <memory>
#include <filesystem>
#include <map>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "Level.hpp"
#include "Renderer/Renderer.hpp"
#include "Resource/WadFile.hpp"

#include <SDL2/SDL_opengl.h>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>


struct
{
    glm::vec3   pos = { 0, 0, 4 };
    float       angle = glm::radians(90.0); 
    float       yaw = 0.0f;
} player;


void moveSectorUpAndDown(Level& level, float deltaTime) {
    static float timer = 0.0;
    static bool moving = 0;
    static int direction = -1;
    static float initialFloorZ = level.sectors[1].floorZ;
    static float initialCeilingZ = level.sectors[1].ceilingZ;

    const float moveAmount = 64.0f;
    const float moveSpeed = 24.0f;
    const float pauseTime = 4.0f;

    // If moving, updaate 
    if (moving) {
        Sector& sector = level.sectors[1];

        sector.floorZ += moveSpeed * direction * deltaTime;
        sector.ceilingZ += moveSpeed * direction * deltaTime;

        // If we reach the end of our movement range, stop the movement and clamp the position. 
        if (direction < 0 && sector.floorZ <= initialFloorZ) {
            sector.floorZ = initialFloorZ;
            sector.ceilingZ = initialCeilingZ;

            moving = false;
        }
        if (direction > 0 && sector.floorZ >= initialFloorZ + moveAmount) {
            sector.floorZ = initialFloorZ + moveAmount;
            sector.ceilingZ = initialCeilingZ + moveAmount;

            moving = false;
        }
    }
    else { // Wait to move again
        timer += deltaTime;

        if (timer >= pauseTime) {
            direction *= -1;
            timer = 0.0f;
            moving = true;
        }
    }
}

void moveSectorInCircle(Level& level, float deltaTime)
{
#if 0
    // Config
    const float radius = 128.0f;
    constexpr float speed = glm::radians(20.0f);    // Radians per second for circle traversal

    // data we need to calculate positions
    static std::vector<uint32_t> vertices;
    static std::map<uint32_t, glm::vec2> offsets;

    static bool firstRun = true;
    static float angle = 0.0f;

    Sector& sector = level.sectors[1];

    // For the first run of the function, collect a list of vertices for the sector  
    // and map each of the start positions for the vertices. 
    if (firstRun) {
        for (uint32_t i = 0; i < sector.wallCount; i++) {
            const Wall& wall = level.walls[sector.firstWall + i];

            vertices.push_back(wall.startVertex);
            offsets[wall.startVertex] = level.vertices[wall.startVertex];
        }

        firstRun = false;
    }

    // New center for the sector
    glm::vec2 center = glm::vec2{ cos(angle), sin(angle) } *radius;

    // Loop through and update the position of each of the vertices since both sectors 
    // referece the same set of vertices, we don't needto update anything for the 
    // other sector. 
    for (uint32_t vertexIndex : vertices) {
        Vertex& vertex = level.vertices[vertexIndex];

        vertex = center + offsets[vertexIndex];
    }

    angle += speed * deltaTime;
#endif
}

/**w
 * @brief Constructs a hard-coded test level with just a few sectors.
 * 
 * @details This test level contains a limited number of rectangular textures intended for testing the level
 *          format, and basic features of the rendering code. 
 */
std::unique_ptr<Level> buildTestLevel()
{
    auto level = std::make_unique<Level>();
#if 0    
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

// Add all the walls, going in CCW winding order for defining the inside of the sectors
    level->walls.push_back(Wall{ {1.0, 0.0, 0.0}, 0, 1, Wall::NO_BEHIND });    // Sector 0 (Done)
    level->walls.push_back(Wall{ {1.0, 0.0, 0.0}, 1, 2, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 0.0, 0.0}, 4, 3, 1});
    level->walls.push_back(Wall{ {1.0, 0.0, 0.0}, 3, 0, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 3, 5,     0 });    // Sector 1
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 4, 6,     Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 5, 7,     Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 9, 8,     2 });
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 8, 9,     Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 7, 10,    3 });
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 6, 11,    Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {0.0, 1.0, 0.0}, 2, 4,     Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {0.0, 0.0, 1.0}, 6, 13, 1 });    // Sector 2
    level->walls.push_back(Wall{ {0.0, 0.0, 1.0}, 7, 14, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {0.0, 0.0, 1.0}, 11, 15, 4 });
    level->walls.push_back(Wall{ {0.0, 0.0, 1.0}, 10, 12, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {1.0, 0.0, 1.0}, 8, 17, 1 });    // Sector 3
    level->walls.push_back(Wall{ {1.0, 0.0, 1.0}, 9, 18, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 0.0, 1.0}, 13, 19, 4 });
    level->walls.push_back(Wall{ {1.0, 0.0, 1.0}, 12, 16, Wall::NO_BEHIND });

    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 11, 21, Wall::NO_BEHIND });    // Sector 4
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 12, 22, 3 });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 13, 23, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 15, 24, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 14, 25, Wall::NO_BEHIND });
    level->walls.push_back(Wall{ {1.0, 1.0, 1.0}, 10, 20, 2 });

// Add all the sectors
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},  0.0,    64.0,   0,    4 }); 
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 16.0,    80.0,   4,    8 });
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 32.0,    96.0,   12,   4 });
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 32.0,    96.0,   16,   4 });
    level->sectors.push_back(Sector{ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 48.0,   116.0,   20,   6 });
#endif 
    return std::move(level);
}

// Haha, get it?
// This is a simple 2-sector test level that has one sector completely inside another. This is intended
// to test the triangulation code and make sure it can handle holes in the sectors. In Doom Style games, 
// it is extremely common to have sectors inside of sectors (polys with holes). We want to automatically
// handle this case so that level editor don't need to subdivide their sectors to avoid holes. 
std::unique_ptr<Level> buildHolyGeometry()
{
    auto level = std::make_unique<Level>();

    // Add all the verticies for the level
    level->vertices.push_back(Vertex{ -256, -256 });
    level->vertices.push_back(Vertex{  256, -256 });
    level->vertices.push_back(Vertex{  -64, -64 });
    level->vertices.push_back(Vertex{   64, -64 });
    level->vertices.push_back(Vertex{  -64,  64 });
    level->vertices.push_back(Vertex{   64,  64 });
    level->vertices.push_back(Vertex{ -256,  256 });
    level->vertices.push_back(Vertex{  256,  256 });

    // Add all the LineDefs for the level
    level->lineDefs.push_back(LineDef{ 0, 1, 0, LineDef::NO_WALL });
    level->lineDefs.push_back(LineDef{ 3, 2, 4, 8 });
    level->lineDefs.push_back(LineDef{ 6, 0, 3, LineDef::NO_WALL });
    level->lineDefs.push_back(LineDef{ 2, 4, 5, 11 });
    level->lineDefs.push_back(LineDef{ 5, 3, 7, 9 });
    level->lineDefs.push_back(LineDef{ 1, 7, 1, LineDef::NO_WALL });
    level->lineDefs.push_back(LineDef{ 4, 5, 6, 10});
    level->lineDefs.push_back(LineDef{ 7, 6, 2, LineDef::NO_WALL });

    // Add all the walls, going in CCW winding order for defining the inside of the sectors
    level->walls.push_back(Wall{ 0, 0, false, {1.0, 0.0, 0.0} });  // Sector 0, outside wall
    level->walls.push_back(Wall{ 5, 0, false, {1.0, 0.0, 0.0} });
    level->walls.push_back(Wall{ 7, 0, false, {1.0, 0.0, 0.0} });
    level->walls.push_back(Wall{ 2, 0, true, {1.0, 0.0, 0.0} });

    level->walls.push_back(Wall{ 1, 0, false, {1.0, 0.0, 0.0} });  // Sector 0, inside wall
    level->walls.push_back(Wall{ 3, 0, false, {1.0, 0.0, 0.0} });  
    level->walls.push_back(Wall{ 6, 0, false, {1.0, 0.0, 0.0} });
    level->walls.push_back(Wall{ 4, 0, true, {1.0, 0.0, 0.0} });

    level->walls.push_back(Wall{ 1, 1, false, {1.0, 0.0, 0.0} });  // Sector 1
    level->walls.push_back(Wall{ 4, 1, false, {1.0, 0.0, 0.0} });
    level->walls.push_back(Wall{ 6, 1, false, {1.0, 0.0, 0.0} });
    level->walls.push_back(Wall{ 3, 1, true, {1.0, 0.0, 0.0} });

    level->sectors.push_back(Sector{ 0, 8, 0, 64, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} });
    level->sectors.push_back(Sector{ 8, 4, -32, 48, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} });
    
    return std::move(level);
}

std::unique_ptr<Level> buildDynamicLevelMovingFlat()
{
    auto level = buildHolyGeometry();
    
    level->onUpdate = moveSectorUpAndDown;

    return std::move(level);
}

std::unique_ptr<Level> buildDynamicLevelMovingSector()
{
    auto level = buildHolyGeometry();
    
    level->onUpdate = moveSectorInCircle;

    return std::move(level);
}

std::vector<std::string> getFilesystemLevels()
{
    using namespace std::filesystem;

    path path("./Maps/");

    std::vector<std::string> files;
    for (const auto& entry : directory_iterator(path))
    {
        bool isFile = is_regular_file(entry);
        bool isWad = entry.path().extension().string() == ".wad";

        if (isFile && isWad) {
            files.push_back(entry.path().string());
        }
    }

    return files;
}

std::unique_ptr<Level> getLevelSelection()
{
    std::cout << "Select the level you want to play\n";
    std::cout << "  0 - Initial Test Level\n";
    std::cout << "  1 - Floors moving up & down\n";
    std::cout << "  2 - Sectors Moving Around\n";

    //std::vector<std::string> levels = getFilesystemLevels();
    //for (size_t i = 2; i <= levels.size(); i++) {
    //    std::cout << std::format("  {} - {}", i, levels[i - 2]) << "\n";
    //}

    std::cout << std::endl << "Type the number of the level: ";

    unsigned int input = 0;
    std::cin >> input;

    switch (input) 
    {
    case 0: return std::move(buildTestLevel());
    case 1: return std::move(buildDynamicLevelMovingFlat());
    case 2: return std::move(buildDynamicLevelMovingSector());
    default: return std::move(buildTestLevel());
    }
}

void handleInput(float deltaTime)
{
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    const float moveSpeed = 32.0f;                  // Units Per Second
    const float climbSpeed = 32.0f;                 // Units Per Second
    const float turnSpeed = glm::radians(90.0f + 45.0f);    // Radians per second

    if (keys[SDL_SCANCODE_SPACE]) {
        player.pos.z += climbSpeed * deltaTime;
    }
    if (keys[SDL_SCANCODE_LALT]) {
        player.pos.z -= climbSpeed * deltaTime;
    }

    if (keys[SDL_SCANCODE_LEFT]) {
        player.angle += turnSpeed * deltaTime;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        player.angle -= turnSpeed * deltaTime;
    }

    if (keys[SDL_SCANCODE_UP]) {
        player.yaw += turnSpeed * deltaTime;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        player.yaw -= turnSpeed * deltaTime;
    }

    // Clamp the yaw to up/down. We use a value slightly less than 90 degrees because
    // otherwise, we would mess up the lookat() call in the renderer, since we assume
    // that the up vector is always directly up
    constexpr float maxYaw = glm::radians(89.999f);
    player.yaw = glm::clamp(player.yaw, -maxYaw, maxYaw);

    glm::vec3 forwards { glm::cos(player.angle), glm::sin(player.angle), 0.0f };
    glm::vec3 right { forwards.y, -forwards.x, 0.0f };

    if (keys[SDL_SCANCODE_W]) {
        player.pos += forwards * moveSpeed * deltaTime;
    }
    if (keys[SDL_SCANCODE_S]) {
        player.pos -= forwards * moveSpeed * deltaTime;
    }
    if (keys[SDL_SCANCODE_A]) {
        player.pos -= right * moveSpeed * deltaTime;
    }
    if (keys[SDL_SCANCODE_D]) {
        player.pos += right * moveSpeed * deltaTime;
    }
}

void RenderUi(const Renderer& renderer)
{
    if (ImGui::Begin("Performance", nullptr, 0))
    {
        ImGui::SeparatorText("Frame Timings");

        ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
        if (ImGui::BeginTable("Timers", 3, flags)) {
            ImGui::TableSetupColumn("Timer", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Average", ImGuiTableColumnFlags_NoHide);

            ImGui::TableHeadersRow(); 
            ImGui::TableNextRow();

            ImGui::TableNextColumn();           ImGui::Text("Renderer");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextRow();

            ImGui::TableNextColumn();           ImGui::Text("    Level");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextRow();

            ImGui::TableNextColumn();           ImGui::Text("        Meshing");
            ImGui::TableNextColumn();           ImGui::Text("%f", renderer.meshTimer.milleseconds());
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextRow();

            ImGui::TableNextColumn();           ImGui::Text("        OpenGL");
            ImGui::TableNextColumn();           ImGui::Text("%f", renderer.glTimer.milleseconds());
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextRow();

            ImGui::TableNextColumn();           ImGui::Text("Game Sim");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextRow();


            ImGui::TableNextColumn();           ImGui::Text("Total Frame Time");
            ImGui::TableNextColumn();           ImGui::Text("--");
            ImGui::TableNextColumn();           ImGui::Text("--");

            ImGui::EndTable();

        }

        ImGui::End();
    }
}

int main(int argc, char** argv)
{
    WadFile testFile("maps/TestMap1.wad");

    testFile.printDirectory(std::cout);

    std::unique_ptr<Level> level = getLevelSelection();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initalize SDL2! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("Sector Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "Could not create window! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "Could not create OpenGL contxt! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (SDL_GL_MakeCurrent(window, context) != 0) {
        std::cerr << "Could not make OpenGL context current! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Could not load OpenGL\n" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();

    const int timeStepMs = 10;
    const float deltaTime = 1.0f / 1000.0f * (float)timeStepMs;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        if (level->onUpdate != nullptr) 
            level->onUpdate(*level, deltaTime);

        handleInput(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        renderer->beginFrame(width, height);

        renderer->renderLevel(*level, player.pos, player.angle, player.yaw);

        renderer->endFrame();


        RenderUi(*renderer);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();

        SDL_GL_SwapWindow(window);

        SDL_Delay(timeStepMs);
    }

    // Manually release the renderer. 
    // We need to clear the OpenGL state before de-initializing opengl and whatnot. 
    renderer.release();
    
    glFinish();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}