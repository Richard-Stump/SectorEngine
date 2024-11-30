#ifndef RESOURCE_UTILITY_HPP_INCLUDED
#define RESOURCE_UTILITY_HPP_INCLUDED

#include <istream>

/**
 * @brief Helper class for reading from a binary streams.
 *
 * @details Provides methods for reading specific data types from a binary stream,
 *          making the code for reading shorter and simpler.
 */
class BinaryStreamReader
{
public:
	BinaryStreamReader() : stream(nullptr)
	{

	}

	BinaryStreamReader(std::istream& _stream) : stream(&_stream)
	{
	}

	BinaryStreamReader(BinaryStreamReader& other) : stream(other.stream)
	{
	}

	/** @brief Reads a null terminated string from the stream */
	std::string readString()
	{
		std::string string;

		while (int c = stream->get())
			string.push_back(c);

		return string;
	}

	/** @brief Reads a fixed-length string from the stream */
	std::string readString(size_t length)
	{
		char* buffer = new char[length + 1];
		buffer[length] = '\0';
		stream->read(buffer, length);

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

private:
	std::istream* stream;

	template<typename T>
	T readType() {
		T value;
		stream->read(reinterpret_cast<char*>(&value), sizeof(T));
		return value;
	}
};

#endif//RESOURCE_UTILITY_HPP_INCLUDED
