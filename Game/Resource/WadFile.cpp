	#include "WadFile.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

WadFile::WadFile(std::string&& fileName)
{
	this->stream = std::ifstream(fileName, std::ifstream::binary);
	this->reader = BinaryStreamReader(this->stream);

	loadHeader();
	loadDirectory();
}

void WadFile::printDirectory(std::ostream& const stream) const
{
	stream << "Name\t\tOffset\tSize" << std::endl;

	for (const DirectoryEntry& entry : directory)
	{
		stream << entry.name << "\t\t"
			<< entry.offset << "\t"
			<< entry.size << std::endl;
	}
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
	}
}

