#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <vector> 
#include <numeric>
#include <functional>

#include <glm/glm.hpp>

// Brief explanation of the level format:
//		Vertex	- 2D point used to define LineDefs
//		LineDef - Physical line defined by two vertices, defining boundaries in a level
//		Wall	- One side of a Line, defining one edge of a sector and it's renderable traits
//		Sector	- A 2D polygon, defined by walls, defining an area in the level with a floor and ceiling
// Wall Loops:
//		Walls for a given sector should be stored together in memory, defining walls in counter-clockwise
//		winding. 

/**
 * @brief A 2D vertex used in defining a level
 */
using Vertex = glm::vec2;

/**
 * @brief A polygonal shape defining a single room in the engine, with a floor, ceiling, and walls
 */
struct Sector
{
	uint32_t	firstWallId;	// Index of the first wall defining the edges of this sector
	uint32_t	wallCount;		// The number of walls that this sector has

	float floorZ;				// The Z-Height of this sector's floor
	float ceilingZ;				// The Z-Height of this sector's ceiling

	glm::vec3	floorColor;		// The color that the floor should be rendered as
	glm::vec3	ceilingColor;	// The color that the ceiling should be rendered as
};

/**
 * @brief A physical line used to define physical boundaries in a level, with one or two sides (walls).
 */
struct LineDef
{
	static constexpr uint32_t NO_WALL = std::numeric_limits<uint32_t>::max();

	uint32_t	startVertexId;
	uint32_t	endVertexId;
	uint32_t	frontWallId;
	uint32_t	backWallId;
};

/**
 * @brief A single edge defining the boundary of a sector
 * 
 * @details Walls for a given sector should be defined in counter-clockwise order, with the outside
 *			boundaries being counter-clockwise, and holes in the sector being defined with a clockwise
 *			winding. 
 */
struct Wall
{
	uint32_t	lineDefId;	// The index of the line this wall lies on
	uint32_t	sectorId;	// The index of the sector this wall is for
	bool		endOfLoop;	//
	glm::vec3	color;		// The color that this wall should be rendered as
};

/**
 * @brief Structure representing a level within the engine.
 */
struct Level
{
	std::vector<Vertex>		vertices;	
	std::vector<LineDef>	lineDefs;
	std::vector<Wall>		walls;
	std::vector<Sector>		sectors;

	// Function the engine calls each frame to update the level. 
	// TODO: This is not flexible, and needs replaced with a better system like Doom's
	//       thinker system
	std::function<void(Level&, float)> onUpdate = nullptr;
};

#endif//LEVEL_HPP_INCWED