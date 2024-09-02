#include "Renderer.hpp"

#include "OpenGL.hpp"

#include <Resource/MapLoader.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <exception>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

Renderer::Renderer()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	shader.load("Shaders/Main");

	glGenVertexArrays(1, &_vertexArrayId);
	checkGl();
	glGenBuffers(1, &_vertexBufferId);
	checkGl();

	glBindVertexArray(_vertexArrayId);
	checkGl();
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	checkGl();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));						// Position 
	glEnableVertexAttribArray(0);
	checkGl();

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));		// Color
	glEnableVertexAttribArray(1);
	checkGl();
}

Renderer::~Renderer()
{
	glDeleteBuffers(1, &_vertexBufferId);
	checkGl();
	_vertexBufferId = 0;

	glDeleteVertexArrays(1, &_vertexArrayId);
	checkGl();
	_vertexArrayId = 0;
}

void Renderer::beginFrame(int width, int height)
{
	glViewport(0, 0, width, height);
	checkGl();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	checkGl();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkGl();

	_width = width;
	_height = height;
}

void Renderer::renderLevel(const Level& level, glm::vec3 camPos, float angle)
{
	std::vector<glm::vec3> data;
	int vertexCount = buildMesh(level, data);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_DYNAMIC_DRAW);
	checkGl();

	glm::mat4 matProj = glm::perspective(
		glm::radians(90.0f),
		(float)_width / (float)_height,
		0.1f,
		1000.0f
	);

	glm::mat4 matView = glm::lookAt(
		camPos,
		camPos + glm::vec3{ -glm::cos(angle), 0, glm::sin(angle) },
		{ 0, 1.0f, 0.0 }
	);

	const float scale = 1.0 / 8.0;
	glm::mat4 matWorld = glm::scale(glm::vec3{ scale });

	glm::mat4 matTrans = matProj * matView * matWorld;

	shader.use();
	shader.setMat4("matTrans", matTrans);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	checkGl();
}

void Renderer::endFrame()
{

}

int Renderer::buildMesh(const Level& level, std::vector<glm::vec3>& mesh)
{
	int vertexCount = 0;

	// Build a mesh for all the walls. 
	for (const Sector& sector : level.sectors) {
		
		for (int i = 0; i < sector.wallCount; i++) {
			const Wall& wall = level.walls[sector.firstWall + i];

			glm::vec2 start = level.vertices[wall.startVertex];
			glm::vec2 end = level.vertices[level.walls[wall.nextWall].startVertex];

			if (wall.behindSector == Wall::NO_BEHIND) {
				glm::vec3 startBottom{ start, sector.floorHeight };
				glm::vec3 startTop{ start, sector.ceilingHeight };
				glm::vec3 endBottom{ end, sector.floorHeight };
				glm::vec3 endTop{ end, sector.ceilingHeight };

				mesh.push_back(startBottom);
				mesh.push_back(wall.color);
				mesh.push_back(startTop);
				mesh.push_back(wall.color);
				mesh.push_back(endTop);
				mesh.push_back(wall.color);

				mesh.push_back(startBottom);
				mesh.push_back(wall.color);
				mesh.push_back(endTop);
				mesh.push_back(wall.color);
				mesh.push_back(endBottom);
				mesh.push_back(wall.color);

				vertexCount += 6;
			}
			else {
				const Sector& behindSector = level.sectors[wall.behindSector];

				// If the sector behind this wall has a higher floor, render the ledge
				if (behindSector.floorHeight > sector.floorHeight) {
					glm::vec3 startBottom{ start, sector.floorHeight };
					glm::vec3 startTop{ start, behindSector.floorHeight };
					glm::vec3 endBottom{ end, sector.floorHeight };
					glm::vec3 endTop{ end, behindSector.floorHeight };

					mesh.push_back(startBottom);
					mesh.push_back(wall.color);
					mesh.push_back(startTop);
					mesh.push_back(wall.color);
					mesh.push_back(endTop);
					mesh.push_back(wall.color);

					mesh.push_back(startBottom);
					mesh.push_back(wall.color);
					mesh.push_back(endTop);
					mesh.push_back(wall.color);
					mesh.push_back(endBottom);
					mesh.push_back(wall.color);

					vertexCount += 6;
				}

				if (behindSector.ceilingHeight < sector.ceilingHeight) {
					glm::vec3 startBottom{ start, behindSector.ceilingHeight };
					glm::vec3 startTop{ start, sector.ceilingHeight };
					glm::vec3 endBottom{ end, behindSector.ceilingHeight };
					glm::vec3 endTop{ end, sector.ceilingHeight };

					mesh.push_back(startBottom);
					mesh.push_back(wall.color);
					mesh.push_back(startTop);
					mesh.push_back(wall.color);
					mesh.push_back(endTop);
					mesh.push_back(wall.color);

					mesh.push_back(startBottom);
					mesh.push_back(wall.color);
					mesh.push_back(endTop);
					mesh.push_back(wall.color);
					mesh.push_back(endBottom);
					mesh.push_back(wall.color);

					vertexCount += 6;
				}
			}
		}
	}
 
	return vertexCount;
}
