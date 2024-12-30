#include "MapLoader.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <format>

#include <Resource/WadFile.hpp>

struct DoomLineAndSideDef
{
	uint16_t startVertId;
	uint16_t endVertId;

	uint16_t frontSector; 
	uint16_t backSector;
};

// Doom wad loading algorithm:
//	1. Load vertices
//	2. Load sectors, leaving the first wall and wall count blank. 
//	3. Load sidedefs
//  4. Load linedefs, mapping each linedef to the sector(s) it belongs to.

DoomMapLoader::DoomMapLoader(const std::string&& fileName, const std::string&& mapName)
	: wadFile(fileName), mapName(mapName)
{
	/* We don't want to load the map in the constructor */
}

std::unique_ptr<Level> DoomMapLoader::loadLevel()
{
	std::unique_ptr<Level> level = std::make_unique<Level>();

	loadVertices();
	loadSidedefs();
	loadLinedefs();
	loadSectors();

	return std::move(level);
}

void DoomMapLoader::loadVertices()
{
	uint32_t vertexLumpIndex = wadFile.indexOfMapLump(mapName, LUMP_VERTICES);
	uint32_t vertexCount = wadFile.lumpSize(vertexLumpIndex) / VERTEX_ENTRY_SIZE;

	FileSubsetStream vertexStream = wadFile.lumpStream(vertexLumpIndex);

	BinaryStreamReader reader(vertexStream);

	// TODO: Remove debugging lines
	std::cout << "Loading map Vertices:" << std::endl;
	std::cout << "\tidx\tx\ty\n";

	for (uint32_t i = 0; i < vertexCount; i++) {
		int16_t x = reader.readInt16();
		int16_t y = reader.readInt16();

		std::cout << std::format("\t{}\t\t{}\t{}\n", i, x, y);

		doomVertices.push_back({ x, y });
	}
}

void DoomMapLoader::loadSidedefs()
{
	uint32_t sidedefLumpIndex = wadFile.indexOfMapLump(mapName, LUMP_SIDEDEFS);
	uint32_t sidedefCount = wadFile.lumpSize(sidedefLumpIndex) / SIDEDEF_ENTRY_SIZE;

	FileSubsetStream sidedefStream = wadFile.lumpStream(sidedefLumpIndex);
	BinaryStreamReader reader(sidedefStream);

	std::cout << "Loading map sidedefs" << std::endl;
	std::cout << "\tidx\tSecId\ttex u\t tex l\ttex m\tline id" << std::endl;

	for (uint32_t i = 0; i < sidedefCount; i++) {
		reader.skip(sizeof(uint16_t) * 2);

		DoomSidedef sidedef;
		sidedef.upperTexture = reader.readString(TEX_NAME_SIZE);
		sidedef.lowerTexture = reader.readString(TEX_NAME_SIZE);
		sidedef.middleTexture = reader.readString(TEX_NAME_SIZE);

		sidedef.sectorId = reader.readUint16();
		sidedef.linedefId = 0xFFFF;

		std::cout << std::format(
			"\t{}\t{}\t{}\t{}\t{}\t{}\n",
			i,
			sidedef.sectorId,
			sidedef.upperTexture,
			sidedef.lowerTexture,
			sidedef.middleTexture,
			sidedef.linedefId
		);

		doomSidedefs.push_back(sidedef);
	}
}

void DoomMapLoader::loadLinedefs()
{
	uint32_t linedefLumpIndex = wadFile.indexOfMapLump(mapName, LUMP_LINEDEFS);
	uint32_t linedefCount = wadFile.lumpSize(linedefLumpIndex) / LINEDEF_ENTRY_SIZE;

	FileSubsetStream linedefStream = wadFile.lumpStream(linedefLumpIndex);
	BinaryStreamReader reader(linedefStream);

	std::cout << "Loading map linedefs" << std::endl;
	std::cout << "\tidx\tstart\tend\tfront\tback" << std::endl;

	for (uint16_t i = 0; i < linedefCount; i++)
	{
		DoomLinedef linedef;
		linedef.startVertexId = reader.readUint16();
		linedef.endVertexId = reader.readUint16();

		reader.skip(3 * sizeof(uint16_t));

		linedef.frontSidedefId = reader.readUint16();
		linedef.backSidedefId = reader.readUint16();

		doomLinedefs.push_back(linedef);

		if (linedef.frontSidedefId != DoomLinedef::NO_SIDEDEF)
			doomSidedefs[linedef.frontSidedefId].linedefId = i;
		if (linedef.backSidedefId != DoomLinedef::NO_SIDEDEF)
			doomSidedefs[linedef.backSidedefId].linedefId = i;

		std::cout << std::format(
			"\t{}\t{}\t{}\t{}\t{}\n",
			i,
			linedef.startVertexId, 
			linedef.endVertexId, 
			linedef.frontSidedefId,
			linedef.backSidedefId
		);
	}
}

void DoomMapLoader::loadSectors()
{
	uint32_t sectorLumpIndex = wadFile.indexOfMapLump(mapName, LUMP_SECTORS);
	uint32_t sectorCount = wadFile.lumpSize(sectorLumpIndex) / SECTOR_ENTRY_SIZE;

	FileSubsetStream sectorStream = wadFile.lumpStream(sectorLumpIndex);
	BinaryStreamReader reader(sectorStream);

	std::cout << "Loading map sectors" << std::endl;
	std::cout << "\tidx\tfz\tcz\tft\tct" << std::endl;

	for (uint16_t i = 0; i < sectorCount; i++)
	{
		DoomSector sector;
		sector.floorZ = reader.readInt16();
		sector.ceilingZ = reader.readInt16();
		sector.floorTexture = reader.readString(TEX_NAME_SIZE);
		sector.ceilingTexture = reader.readString(TEX_NAME_SIZE);

		reader.skip(3 * sizeof(int16_t));

		std::cout << std::format(
			"\t{}\t{}\t{}\t{}\t{}\n",
			i,
			sector.floorZ,
			sector.ceilingZ,
			sector.floorTexture,
			sector.ceilingTexture
		);

		doomSectors.push_back(sector);
	}
}
