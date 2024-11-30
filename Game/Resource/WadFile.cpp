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
{
	this->stream = std::ifstream(fileName, std::ifstream::binary);
	this->reader = BinaryStreamReader(this->stream);

	loadHeader();
	loadDirectory();
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

uint32_t WadFile::indexOfLump(std::string& lumpName, uint32_t afterIndex) const
{
	// Find the list of lumps with the name we are looking for. 
	if (nameMap.contains(lumpName)) {
		const std::vector<DirectoryEntry*>& entryList = nameMap.at(lumpName);

		// Do a linear search of through the list to find the lump after the specified index. 
		// A linear search should be fine because the list will usually be one entry long. 
		// The worst case will be map lumps such as "sectors", which will have 32-36 or so entries
		// at most. Could be optimised, but not that important right now. 
		for (DirectoryEntry* i : entryList)
			if (i->index >= afterIndex)
				return i->index;

		std::string msg = std::format("Lump with name '{}' after index {} not found", lumpName, afterIndex);
		throw std::exception(msg.c_str());
	}
	else {
		std::string msg = std::format("Lump with name '{}' after index {} not found", lumpName, afterIndex);
		throw std::exception(msg.c_str());
	}
}

uint32_t WadFile::indexOfMapLump(std::string& mapName, std::string& lumpName) const
{
	uint32_t mapMarkerIndex = indexOfLump(mapName);
	
	return indexOfLump(lumpName, mapMarkerIndex);
}

std::istream& WadFile::lumpStream(uint32_t index)
{
	const DirectoryEntry& entry = directory[index];

	// TODO: Implement substream 

	return stream;
}

std::istream& WadFile::lumpStream(std::string& lumpName, uint32_t afterIndex)
{
	uint32_t index = indexOfLump(lumpName, afterIndex);

	return lumpStream(index);
}

std::istream& WadFile::mapLumpStream(std::string& mapName, std::string& lumpName)
{
	uint32_t index = indexOfMapLump(mapName, lumpName);

	return lumpStream(index);
}


void WadFile::loadHeader()
{
	magicString = reader.readString(MAGIC_LENGTH);
	lumpCount = reader.readUint32();
	directoryOffset = reader.readUint32();

	if (magicString != "PWAD" && magicString != "IWAD") {
		throw std::exception("Not a valid wad file");
	}
}

void WadFile::loadDirectory()
{
	stream.seekg(directoryOffset, std::istream::beg);

	for (uint32_t i = 0; i < lumpCount; i++) {
		DirectoryEntry newEntry;
		newEntry.index = i;
		newEntry.offset = reader.readUint32();
		newEntry.size = reader.readUint32();
		newEntry.name = reader.readString(LUMP_NAME_LENGTH);

		directory.push_back(newEntry);
		DirectoryEntry* entryPtr = &directory.back();

		// Add the lump to our lump list. 
		std::vector<DirectoryEntry*>& list = nameMap[newEntry.name];
		list.push_back(entryPtr);
	}
}

