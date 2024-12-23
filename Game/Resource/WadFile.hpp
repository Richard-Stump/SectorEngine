#ifndef WAD_FILE_HPP_INCLUDED
#define WAD_FILE_HPP_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <fstream>

#include <Resource/Utilities.hpp>

/**
 * @brief Class that abstracts the reading of Doom WAD files
 */
class WadFile
{
public:
	WadFile(const std::string&& fileName);
	WadFile(const std::string& fileName);

	void printDirectory(std::ostream& stream) const;

	uint32_t lumpSize(uint32_t index) const;

	/** @brief Returns the index of the first intance of a lump name, after the specified index */
	uint32_t indexOfLump(const std::string& lumpName, uint32_t afterIndex = 0) const;
	
	/** @brief Returns the index of the specified lump for a given map */
	uint32_t indexOfMapLump(const std::string& mapName, const std::string& lumpName) const;

	/** @brief Returns a stream for reading a lump specified by the given index */
	FileSubsetStream lumpStream(uint32_t index);

	/** @brief Returns a stream for reading the first insteance of a lump name, after the specified index */
	FileSubsetStream lumpStream(std::string& lumpName, uint32_t afterIndex = 0);
	
	/** @brief Returns a stream for reading the specified map lump for a given map name */
	FileSubsetStream mapLumpStream(std::string& mapName, std::string& lumpName);

private:
	const size_t MAGIC_LENGTH = 4;		
	const size_t LUMP_NAME_LENGTH = 8;

	/**
	 * @brief A directory entry for a lump in the wad file
	 * 
	 * @details Contains information on the index, size, offset, and name of a file. 
	 */
	struct DirectoryEntry {
		uint32_t		index;	/// Not stored in the wad directory itself, but handy during lookups.
		uint32_t		offset;	
		uint32_t		size;
		std::string		name;
	};

	/** @brief The entire directory of lumps in the wad */
	std::vector<DirectoryEntry> directory;

	std::string					fileName;

	// Maps lump names to their directory entry(s). Needs to be a one-to-many
	// associate because doom uses duplicate lump names across maps and other lumps (Especially for map files)
	std::map<std::string, std::vector<uint32_t>> nameMap;
	
	std::string					magicString;
	uint32_t					directoryOffset;
	uint32_t					lumpCount;

	void loadHeader(BinaryStreamReader& reader);
	void loadDirectory(BinaryStreamReader& reader);
};

#endif//WAD_FILE_HPP_INCLUDED
