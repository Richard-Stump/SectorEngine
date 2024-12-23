#include "MapLoader.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <format>

#include <Resource/WadFile.hpp>

DoomMapLoader::DoomMapLoader(const std::string&& fileName, const std::string&& mapName)
	: wadFile(fileName), mapName(mapName)
{
	/* We don't want to load the map in the constructor */
}

std::unique_ptr<Level> DoomMapLoader::loadLevel()
{
	std::unique_ptr<Level> level = std::make_unique<Level>();

	loadVertices(*level);

	// TODO: Load Sectors
	// TODO: Load Sidedefs
	// TODO: Load Linedefs, mapping them to their front/back sectors using the sidedef info
	// TODO: Split linedefs into sidedefs

	return std::move(level);
}

void DoomMapLoader::loadVertices(Level& level)
{
	uint32_t vertexLumpIndex = wadFile.indexOfMapLump(mapName, LUMP_VERTICES);
	uint32_t vertexCount = wadFile.lumpSize(vertexLumpIndex) / 4;

	FileSubsetStream vertexStream = wadFile.lumpStream(vertexLumpIndex);

	BinaryStreamReader reader(vertexStream);

	// TODO: Remove debugging lines
	std::cout << "Loading map Vertices:" << std::endl;

	for (uint32_t i = 0; i < vertexCount; i++) {
		int16_t x = reader.readInt16();
		int16_t y = reader.readInt16();

		std::cout << std::format("\t{}\t\t{}\t{}\n", i, x, y);

		level.vertices.push_back({ x, y });
	}
}
