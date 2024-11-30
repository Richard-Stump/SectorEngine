#ifndef MAP_LOADER_HPP_INCLUDED
#define MAP_LOADER_HPP_INCLUDED

#include <vector>
#include <string> 
#include <memory>
#include <fstream>

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
	DoomMapLoader(const std::string&& fileName, const std::string&& mapName = "map01");

	std::unique_ptr<Level> loadLevel() override;

private:
	const std::string LUMP_VERTICES = "VERTICES";
	const std::string LUMP_LINEDEFS	= "LINEDEFS";
	const std::string LUMP_SIDEDEFS = "SIDEDEFS";
	const std::string LUMP_SECTORS = "SECTORS";

	WadFile			wadFile;
	std::string		mapName;
};

#endif//MAP_LOADER_HPP_INCLUDED