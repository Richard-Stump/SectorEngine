#ifndef MAP_LOADER_HPP_INCLUDED
#define MAP_LOADER_HPP_INCLUDED

#include <vector>
#include <string> 
#include <memory>
#include <fstream>
#include <map>
#include <queue>

#include <glm/glm.hpp>

#include <Resource/WadFile.hpp>
#include <Level.hpp>

/**
 * @brief Interface for map loaders, allowing multiple map formats to be supported. 
 * 
 * @details This provides an interface that makes it easier for new map formats to be added
 *			without having to change too many parts of the engine. Implementing classes can
 *          worry about the details of various map formats (Doom Wads, build maps, Yaml, etc),
 *			and return the map in the internal engine format. 
 * 
 * @remarks Map loaders may not return maps with complete functionality, as some formats (Doom Wads)
 *			will inherently lack the capability of storing those features. This can still be useful
 *			for loading formats from other games, and then saving them in the format that the engine
 *			utilizes. 
 */
class MapLoader
{
public:
	virtual std::unique_ptr<Level> loadLevel() = 0;
};

/**
 * @brief Implementation for loading Doom Maps into the engine
 * 
 * @remarks Currently, this is intented to be used for testing the engine with more complex
 *          geometry, without needing to create a dedicated level editor. As the engine gains more
 *			features, it will likely need it's own map format since the Doom map format is very
 *			limited. 
 */
class DoomMapLoader : public MapLoader
{
public:
	DoomMapLoader(const std::string&& fileName, const std::string&& mapName = "MAP01");

	std::unique_ptr<Level> loadLevel() override;

private:
	struct DoomLinedef
	{
		static const uint16_t NO_SIDEDEF = 0xFFFF;

		uint16_t id;
		uint16_t startVertexId;
		uint16_t endVertexId;
		uint16_t frontSidedefId;
		uint16_t backSidedefId;
	};

	struct DoomSidedef
	{
		uint16_t	id;
		uint16_t	sectorId;
		uint16_t	linedefId;
		std::string upperTexture;
		std::string middleTexture;
		std::string lowerTexture;
	};

	struct DoomSector
	{
		int16_t		floorZ;
		int16_t		ceilingZ;

		std::string	floorTexture;
		std::string ceilingTexture;
	};

	// Names for the different map lumps that are used by Doom
	const std::string LUMP_VERTICES = "VERTEXES";
	const std::string LUMP_LINEDEFS	= "LINEDEFS";
	const std::string LUMP_SIDEDEFS = "SIDEDEFS";
	const std::string LUMP_SECTORS	= "SECTORS";

	// Sizes of individual object entries in the map files.
	const uint32_t VERTEX_ENTRY_SIZE	= 4;
	const uint32_t SIDEDEF_ENTRY_SIZE	= 30;
	const uint32_t LINEDEF_ENTRY_SIZE	= 14;
	const uint32_t SECTOR_ENTRY_SIZE	= 26;

	// Max number of characters in a texture name
	const size_t TEX_NAME_SIZE = 8;

	// String that corresponds to no texture
	const std::string TEX_NONE = "-";

	WadFile			wadFile;
	std::string		mapName;

	void loadVertices();
	void loadSidedefs();
	void loadLinedefs();
	void loadSectors();

	std::vector<glm::vec2>		doomVertices;
	std::vector<DoomSidedef>	doomSidedefs;
	std::vector<DoomLinedef>	doomLinedefs;
	std::vector<DoomSector>		doomSectors;
};

#endif//MAP_LOADER_HPP_INCLUDED