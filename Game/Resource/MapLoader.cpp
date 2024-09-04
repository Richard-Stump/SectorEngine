#include "MapLoader.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <format>

const size_t WAD_MAGIC_STRING_LEN = 4;
const size_t WAD_LUMP_NAME_LEN = 8; 

std::unique_ptr<Level> DoomMapLoader::loadLevel(std::string&& fileName, std::string&& mapName)
{
	_file = std::ifstream(fileName, std::ifstream::binary | std::ifstream::in);
	if (!_file.good()) {
		std::string message = std::format("Could not open wad file \"{}\" to load map \"{}\".", fileName, mapName);
		throw std::exception(message.c_str());
	}

	char magic[WAD_MAGIC_STRING_LEN + 1] = { 0, 0, 0, 0, 0};

	_file.read(magic, WAD_MAGIC_STRING_LEN);	

	std::string magicString(magic);
	if (magicString != "IWAD" && magicString != "PWAD") {
		std::string message = std::format("\"{}\" is not a valid wad file! Header contains the invalid magic string of \"{}\"", fileName, magicString);
		throw std::exception(message.c_str());
	}
	
	uint32_t lumpCount = read<uint32_t>();
	uint32_t directoryOffset = read<uint32_t>();
	
	std::cout << std::format("Lump Count: {}", lumpCount) << std::endl;
	std::cout << std::format("Directory Offset: {}", directoryOffset);

	loadDirectory(directoryOffset, lumpCount);

	std::unique_ptr<Level> level = std::make_unique<Level>();

	return std::move(level);
}

void DoomMapLoader::loadDirectory(uint32_t directoryOffset, uint32_t lumpCount)
{
	_file.seekg(directoryOffset, std::fstream::beg);

	std::cout << "Reading Wad Directory:" << std::endl;

	for (uint32_t i = 0; i < lumpCount; i++) {
		char lumpName[WAD_LUMP_NAME_LEN + 1];
		lumpName[WAD_LUMP_NAME_LEN] = 0;

		_file.read(lumpName, WAD_LUMP_NAME_LEN);
		uint32_t offset = read<uint32_t>();
		uint32_t size = read<uint32_t>();
		  
		std::cout << std::format("    {} {} {}", lumpName, offset, size) << std::endl;

		DirectoryEntry entry;
		entry.index = i;
		entry.size = size;
		entry.offset = offset;
		entry.name = std::string(lumpName);

		_directory.push_back(entry);
	}
}