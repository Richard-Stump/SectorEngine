#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <vector> 
#include <numeric>

#include <glm/glm.hpp>

/**
 * @brief Represents a single vertex inside a level.
 */
using Vertex = glm::vec2;

/**
 * @brief Structure representing a single sector (AKA room) in a level. 
 */
struct Sector
{
	glm::vec3	floorColor;
	glm::vec3	ceilingColor;

	float floorHeight;
	float ceilingHeight;

	uint32_t	firstWall;
	uint32_t	wallCount;
};

/**
 * @brief Structure represetning a single wall defining a sector.
 */
struct Wall
{
	static constexpr uint32_t NO_BEHIND = std::numeric_limits<uint32_t>::max();

	glm::vec3	color;

	uint32_t	startVertex;
	uint32_t	nextWall;
	uint32_t	behindSector;
};

/**
 * @brief Structure representing a level within the engine.
 */
struct Level
{
	std::vector<Vertex>		vertices;
	std::vector<Sector>		sectors;
	std::vector<Wall>		walls;
};

#endif//LEVEL_HPP_INCLUDED