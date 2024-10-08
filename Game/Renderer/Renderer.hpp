#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Resource/MapLoader.hpp>
#include "OpenGL.hpp"
#include "Shader.hpp"
#include <Utility/Timer.hpp>

extern struct Level;

/**
 * This class is in charge of rendering to the screen
 */
class Renderer
{
public:

	Renderer();
	~Renderer();

	void beginFrame(int width, int height);

	void renderLevel(const Level& level, glm::vec3 camPos, float angle, float yaw);

	void endFrame();

	Timer meshTimer;
	Timer glTimer;

private:

	int buildMesh(const Level& level, std::vector<glm::vec3>& mesh);
	int buildWallMesh(const Level& level, std::vector<glm::vec3>& mesh);
	int buildFlatMesh(const Level& level, std::vector<glm::vec3>& mesh);

	ShaderProgram shader;

	unsigned int _vertexBufferId	= 0;
	unsigned int _vertexArrayId		= 0;

	int _width, _height;
};

#endif//RENDERER_HPP