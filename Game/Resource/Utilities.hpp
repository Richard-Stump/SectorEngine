#ifndef RESOURCE_UTILITY_HPP_INCLUDED
#define RESOURCE_UTILITY_HPP_INCLUDED

#include <iostream>>
#include <istream>
#include <sstream>

/**
 * @brief Helper class for reading from a binary streams.
 *
 * @details Provides methods for reading specific data types from a binary stream,
 *          making the code for reading shorter and simpler.
 */
class BinaryStreamReader
{
public:
	BinaryStreamReader() : _stream(nullptr)
	{

	}

	BinaryStreamReader(std::istream& _stream) : _stream(&_stream)
	{
	}

	BinaryStreamReader(BinaryStreamReader& other) : _stream(other._stream)
	{
	}

	/** @brief Reads a null terminated string from the stream */
	std::string readString()
	{
		std::string string;

		while (int c = _stream->get())
			string.push_back(c);

		return string;
	}

	/** @brief Reads a fixed-length string from the stream */
	std::string readString(size_t length)
	{
		char* buffer = new char[length + 1];
		buffer[length] = '\0';
		_stream->read(buffer, length);

		std::string string(buffer);

		delete[] buffer;

		return string;
	}

	char readChar() { return readType<char>(); }
	unsigned char readUchar() { return readType<unsigned char>(); }

	uint8_t readUint8() { return readType<uint8_t>(); }
	uint16_t readUint16() { return readType<uint16_t>(); }
	uint32_t readUint32() { return readType<uint32_t>(); }
	uint64_t readUint64() { return readType<uint64_t>(); }

	int8_t readInt8() { return readType<int8_t>(); }
	int16_t readInt16() { return readType<int16_t>(); }
	int32_t readInt32() { return readType<int32_t>(); }
	int64_t readInt64() { return readType<int64_t>(); }

	void skip(size_t bytes) { _stream->seekg(bytes, std::ios::cur); }

	std::istream& stream() { return *_stream; }

private:
	std::istream* _stream;

	template<typename T>
	T readType() {
		T value;
		_stream->read(reinterpret_cast<char*>(&value), sizeof(T));
		return value;
	}
};

/**
 * @brief std::istream for reading a specified subset of a file. 
 * 
 * @details Stream for reading a specified subset of a file, decided by the 
 *          passed offset and size within the given file. This is useful for reading
 *          archive files from old games, such as Doom's WAD files, Duke Nukem's GPT files,
 *			etc. This allows the class managing the archive to return a stream for the
 *          individual files within the archive, without the calling code being able to 
 *          read before or after the ends of the sub-file.
 * 
 * @note This class was partially generated using ChatGPT
 */
class FileSubsetStream : public std::istringstream
{
public:

	FileSubsetStream(std::string fileName, uint32_t offset, uint32_t size)
	{
		std::streampos offsetPos(offset);

		// Open the file in binary mode
        std::ifstream file(fileName, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + fileName);
        }

        // Check the file size
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();

        if (offset < 0 || offset >= fileSize) {
            throw std::out_of_range("Offset is out of file bounds.");
        }

        // Calculate the actual size to read, ensuring it doesn't exceed the file bounds
        std::size_t bytesToRead = static_cast<std::size_t>(std::min<std::streampos>(size, fileSize - offsetPos));

        // Seek to the offset and read the specified range
        file.seekg(offset, std::ios::beg);
        buffer.resize(bytesToRead);
        file.read(buffer.data(), bytesToRead);

        // Ensure the file was read successfully
        if (!file) {
            throw std::runtime_error("Error reading file: " + fileName);
        }

        // Initialize the parent istringstream with the loaded data
        this->str(std::string(buffer.data(), buffer.size()));
	}

	FileSubsetStream(FileSubsetStream&& other)
		: std::istringstream(std::move(other.str())), buffer(std::move(other.buffer))
	{
		other.str("");
	}

	FileSubsetStream& operator= (FileSubsetStream&& other) 
	{
		if (this != &other) {
			this->str(std::move(other.str()));
			buffer = std::move(other.buffer);
			other.str("");
		}
		return *this;
	}

private:
	// There is probably a more efficient way to do this than loading the entire file into
	// memory, but this solution is more simple and shouldn't be a problem with the size of
	// files we are loading with it. 
	std::vector<char> buffer; 
};

#endif//RESOURCE_UTILITY_HPP_INCLUDED
