#ifndef MAP_LOADER_HPP_INCLUDED
#define MAP_LOADER_HPP_INCLUDED

#include <vector>
#include <string> 
#include <memory>
#include <fstream>

#include <glm/glm.hpp>

#include <Level.hpp>

class DoomMapLoader
{
public:
	std::unique_ptr<Level> loadLevel(std::string&& fileName, std::string&& mapName = "map01");

private:
	struct DirectoryEntry 
	{
		uint32_t		index;	// Index of the lump in the wad
		uint32_t		offset;	// Offset of the lump data in the wad
		uint32_t		size;	// Size of the lump, in bytes
		std::string		name;	// Name of the lump
	};

	std::ifstream	_file;
	std::vector<DirectoryEntry>		_directory;

	void loadDirectory(uint32_t directoryOffset, uint32_t lumpCount);

	template<typename T>
	T read() 
	{
		T value; 
		_file.read(reinterpret_cast<char*>(&value), sizeof(T));
		return value;
	}
};

#endif//MAP_LOADER_HPP_INCLUDED