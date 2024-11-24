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

#include <CDT.h>

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

	meshTimer.reset();
	glTimer.reset();
}

void Renderer::renderLevel(const Level& level, glm::vec3 camPos, float angle, float yaw)
{
	std::vector<glm::vec3> data;
	int vertexCount = buildMesh(level, data);

	glTimer.start();

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
		camPos + glm::vec3{ glm::cos(angle) * glm::cos(yaw), glm::sin(angle) * glm::cos(yaw), sin(yaw)},
		{ 0, 0.0, 1.0 }
	);

	const float scale = 1.0 / 8.0;
	glm::mat4 matWorld = glm::scale(glm::vec3{ scale });

	glm::mat4 matTrans = matProj * matView * matWorld;

	shader.use();
	shader.setMat4("matTrans", matTrans);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	checkGl();

	glTimer.stop();
}

void Renderer::endFrame()
{

}

/**
 * Adds the walls of the level to the render mesh.
 * 
 * \param level	The level we are rendering
 * \param mesh  The mesh vector to add the data to
 * \return		The number of vertices added to the mesh
 */
int Renderer::buildWallMesh(const Level& level, std::vector<glm::vec3>& mesh)
{
	int vertexCount = 0;

	// Build a mesh for all the walls. 
	for (const Sector& sector : level.sectors) {

		for (int i = 0; i < sector.wallCount; i++) {
			const uint32_t wallId = sector.firstWallId + i;
			const Wall& wall = level.walls[wallId];
			const LineDef& lineDef = level.lineDefs[wall.lineDefId];

			const bool wallFollowsLine = wallId == lineDef.frontWallId;
			const uint32_t behindWallId = wallFollowsLine ? lineDef.backWallId : lineDef.frontWallId;

			glm::vec2 start = level.vertices[lineDef.startVertexId];
			glm::vec2 end = level.vertices[lineDef.endVertexId];

			// If there is no sector behind this wall, we can add just a single quad and move on
			// with our busy lives. 
			if (behindWallId == LineDef::NO_WALL) {
				glm::vec3 startBottom{ start, sector.floorZ };
				glm::vec3 startTop{ start, sector.ceilingZ };
				glm::vec3 endBottom{ end, sector.floorZ };
				glm::vec3 endTop{ end, sector.ceilingZ };

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
				// If we are on the back side of the linedef, we need to flip the vertex order to 
				// keep a correct vertex winding. 
				if (!wallFollowsLine) std::swap(start, end);

				// If there is a wall behind, we could have 0, 1, or 2 quads to add. depending on the heights of the two
				// connected sectors. We could have 3 if we had middle textures, but we don't in this demo. 
				const Wall& behindWall = level.walls[behindWallId];
				const Sector& behindSector = level.sectors[behindWall.sectorId];

				// We need to add a wall from our floor to the behind sector's floor if their floor is higher. 
				if (behindSector.floorZ > sector.floorZ) {
					glm::vec3 startBottom{ start, sector.floorZ };
					glm::vec3 startTop{ start, behindSector.floorZ };
					glm::vec3 endBottom{ end, sector.floorZ };
					glm::vec3 endTop{ end, behindSector.floorZ };

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

				// We need to add a wall from our ceiling to the behind sector's ceiling if their ceiling
				// is lower. 
				if (behindSector.ceilingZ < sector.ceilingZ) {
					glm::vec3 startBottom{ start, behindSector.ceilingZ };
					glm::vec3 startTop{ start, sector.ceilingZ };
					glm::vec3 endBottom{ end, behindSector.ceilingZ };
					glm::vec3 endTop{ end, sector.ceilingZ };

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

/**
 * Adds the flats (Floor, Ceilings) to the render mesh.
 *
 * \param level The level we are rendering
 * \param mesh  The mesh vector to add to
 * \return		The number of vertices added to the mesh
 */
int Renderer::buildFlatMesh(const Level& level, std::vector<glm::vec3>& mesh)
{
	int vertexCount = 0;

	for (const Sector& sector : level.sectors) {
		CDT::Triangulation<float> triangulation;

		std::vector<glm::vec2> sectorVerts;
		std::vector<std::pair<size_t, size_t>> sectorEdges;

		uint32_t loopStartWallIndex = sector.firstWallId;	// Which wall did this edge loop start on?
		size_t loopStartVertexIndex = 0;				// What vertex number did this edge loop start on?

		// Loop through all of the walls in the sector so we can add them to the triangulation.
		for (size_t i = 0; i < sector.wallCount; i++) {
			const uint32_t wallId = sector.firstWallId + i;
			const Wall& wall = level.walls[wallId];
			const LineDef& lineDef = level.lineDefs[wall.lineDefId];

			const bool wallFollowsLine = wallId == lineDef.frontWallId;
			const uint32_t startVertexId = wallFollowsLine ? lineDef.startVertexId : lineDef.endVertexId;
			glm::vec2 startVertex = level.vertices[startVertexId];
			sectorVerts.push_back(startVertex);

			// If we reach the end of the edge loop, mark the end vertex of the edge to be the start index
			// for this loop. Otherwise, mark it as the next vertex we are going to add. 
			size_t edgeEndVertexIndex = wall.endOfLoop ? loopStartVertexIndex : i + 1;
			sectorEdges.push_back(std::make_pair(i, edgeEndVertexIndex));
			
			// If we have reached the end of this edge loop, set the indices for the start of the
			// next one. 
			if (wall.endOfLoop) {
				loopStartWallIndex = sector.firstWallId + i + 1;
				loopStartVertexIndex = i + 1;
			}
		}

		triangulation.insertVertices(
			sectorVerts.begin(),
			sectorVerts.end(),
			[](const glm::vec2& v) { return v.x; },
			[](const glm::vec2& v) { return v.y; }
		);

		triangulation.insertEdges(
			sectorEdges.begin(),
			sectorEdges.end(),
			[](const std::pair<size_t, size_t>& e) { return e.first; },
			[](const std::pair<size_t, size_t>& e) { return e.second; }
		);

		triangulation.eraseOuterTrianglesAndHoles();

		// Since sector ceilings and floors are the same 2D-shape, we can triangulate once and then
		// build both the floor and ceiling from the same triangulation
		CDT::TriangleVec tris = triangulation.triangles;
		auto verts = triangulation.vertices;
		for (CDT::Triangle& tri : tris)
		{
			auto v1 = verts[tri.vertices[0]];
			auto v2 = verts[tri.vertices[1]];
			auto v3 = verts[tri.vertices[2]];

			// Add the floor triangles
			mesh.push_back(glm::vec3{ v1.x, v1.y, sector.floorZ });
			mesh.push_back(sector.floorColor);
			mesh.push_back(glm::vec3{ v2.x, v2.y, sector.floorZ });
			mesh.push_back(sector.floorColor);
			mesh.push_back(glm::vec3{ v3.x, v3.y, sector.floorZ });
			mesh.push_back(sector.floorColor);

			// Add the ceiling triangles. These have to have the opposite winding from the floor.
			mesh.push_back(glm::vec3{ v3.x, v3.y, sector.ceilingZ });
			mesh.push_back(sector.ceilingColor);
			mesh.push_back(glm::vec3{ v2.x, v2.y, sector.ceilingZ });
			mesh.push_back(sector.ceilingColor);
			mesh.push_back(glm::vec3{ v1.x, v1.y, sector.ceilingZ });
			mesh.push_back(sector.ceilingColor);

			vertexCount += 6;
		}
	}

	return vertexCount;
}

int Renderer::buildMesh(const Level& level, std::vector<glm::vec3>& mesh)
{
	meshTimer.start();

	int vertexCount = buildWallMesh(level, mesh);
	vertexCount += buildFlatMesh(level, mesh);

	meshTimer.stop();

	return vertexCount;
}
