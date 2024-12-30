#include "WadFile.hpp"

#include <format>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

#include <Resource/Utilities.hpp>

WadFile::WadFile(const std::string&& fileName)
	: WadFile(fileName)
{
}

WadFile::WadFile(const std::string& fileName) 
	: fileName(fileName)
{
	std::ifstream stream(fileName, std::ifstream::binary);
	BinaryStreamReader reader = BinaryStreamReader(stream);

	loadHeader(reader);
	loadDirectory(reader);
}

void WadFile::printDirectory(std::ostream& ostream) const
{
	ostream << "Name\t\tOffset\tSize" << std::endl;

	for (const DirectoryEntry& entry : directory)
	{
		ostream << entry.name << "\t\t"
			<< entry.offset << "\t"
			<< entry.size << std::endl;
	}
}

uint32_t WadFile::lumpSize(uint32_t index) const
{
	return directory[index].size;
}

uint32_t WadFile::indexOfLump(const std::string& lumpName, uint32_t afterIndex) const
{
	// Find the list of lumps with the name we are looking for. 
	if (nameMap.contains(lumpName)) {
		const std::vector<uint32_t>& entryList = nameMap.at(lumpName);

		// Do a linear search of through the list to find the lump after the specified index. 
		// A linear search should be fine because the list will usually be one entry long. 
		// The worst case will be map lumps such as "sectors", which will have 32-36 or so entries
		// at most. Could be optimised, but not that important right now. 
		for (uint32_t i : entryList)
			if (i >= afterIndex)
				return i;

		std::string msg = std::format("Lump with name '{}' after index {} not found", lumpName, afterIndex);
		throw std::exception(msg.c_str());
	}
	else {
		std::string msg = std::format("Lump with name '{}' after index {} not found", lumpName, afterIndex);
		throw std::exception(msg.c_str());
	}
}

uint32_t WadFile::indexOfMapLump(const std::string& mapName, const std::string& lumpName) const
{
	uint32_t mapMarkerIndex = indexOfLump(mapName);
	
	return indexOfLump(lumpName, mapMarkerIndex);
}

FileSubsetStream WadFile::lumpStream(uint32_t index)
{
	const DirectoryEntry& entry = directory[index];
		
	FileSubsetStream stream(fileName, entry.offset, entry.size);

 	return stream;
}

FileSubsetStream WadFile::lumpStream(std::string& lumpName, uint32_t afterIndex)
{
	uint32_t index = indexOfLump(lumpName, afterIndex);

	return lumpStream(index);
}

FileSubsetStream WadFile::mapLumpStream(std::string& mapName, std::string& lumpName)
{
	uint32_t index = indexOfMapLump(mapName, lumpName);

	return lumpStream(index);
}

void WadFile::loadHeader(BinaryStreamReader& reader)
{
	magicString = reader.readString(MAGIC_LENGTH);
	lumpCount = reader.readUint32();
	directoryOffset = reader.readUint32();

	if (magicString != "PWAD" && magicString != "IWAD") {
		throw std::exception("Not a valid wad file");
	}
}

void WadFile::loadDirectory(BinaryStreamReader& reader)
{
	reader.stream().seekg(directoryOffset, std::istream::beg);

	for (uint32_t i = 0; i < lumpCount; i++) {
		DirectoryEntry newEntry;
		newEntry.index = i;
		newEntry.offset = reader.readUint32();
		newEntry.size = reader.readUint32();
		newEntry.name = reader.readString(LUMP_NAME_LENGTH);

		directory.push_back(newEntry);

		// Add the lump to our lump list. 
		std::vector<uint32_t>& list = nameMap[newEntry.name];
		list.push_back(i);
	}
}

