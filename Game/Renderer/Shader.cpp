#include "Shader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

#include "OpenGL.hpp"

ShaderProgram::ShaderProgram() 
{
	id = std::shared_ptr<unsigned int>(new unsigned int);
	*id = -1;
}

void ShaderProgram::load(std::string& name)
{
	std::cout << "Loading program group \"" << name << "\"" << std::endl;

	// This std::shared_ptr handles reference counting for the program id.
	// Here, we allocate a new integer to store it and define a custom deleter
	// that deletes the OpenGL program before freeing the pointer.
	id = std::shared_ptr<unsigned int>(
		new unsigned int,
		[](unsigned int* ptr) {
			glDeleteProgram(*ptr);
			delete ptr;
		}
	);
	*id = -1; // Set the program to invalid.

	// Load each of the individual shaders in the program group.
	int vertexShader = loadShader(name + ".vert", GL_VERTEX_SHADER);
	int fragmentShader = loadShader(name + ".frag", GL_FRAGMENT_SHADER);
	int geometryShader = loadShader(name + ".geom", GL_GEOMETRY_SHADER);

	bool valid = true;

	// We must have a vertex shader and a fragment shader. If either is missing,
	// mark that we have an error and print a message.
	if (vertexShader == -1) {
		std::cout << "    Vertex shader not loaded! "
			"Cannot continue loading program" << std::endl;
		valid = false;
	}
	if (fragmentShader == -1) {
		std::cout << "    Fragment shader not loaded! "
			"Cannot continue loading program" << std::endl;
		valid = false;
	}

	// Only continue to link if we have both a vertex and fragment shader
	if (valid) {
		*id = glCreateProgram();
		checkGl();

		// The vertex shader and fragment shader are required, so we always 
		// link them
		glAttachShader(*id, vertexShader);
		checkGl();
		glAttachShader(*id, fragmentShader);
		checkGl();

		// The geometry shader is not required, so we only link it if it is 
		// provided
		if (geometryShader != -1) {
			glAttachShader(*id, geometryShader);
			checkGl();
		}

		glLinkProgram(*id);

		// Check that the program linked properly
		int linkSucceded;
		glGetProgramiv(*id, GL_LINK_STATUS, &linkSucceded);
		if (!linkSucceded) {
			// Fetch the length of the info log so that we can create a buffer
			// to hold the entire log
			int logLength;
			glGetProgramiv(*id, GL_INFO_LOG_LENGTH, &logLength);

			char* logBuffer = new char[logLength + 1];
			glGetProgramInfoLog(*id, logLength, nullptr, logBuffer);
			logBuffer[logLength] = '\0';

			std::cout << "Program failed to link. OpenGL Info Log:\n"
				<< logBuffer << std::endl;
		}
		else {
			std::cout << "    Program linked successfully!" << std::endl;
		}
	}

	// Free any shaders that were created. 
	if (vertexShader != -1) glDeleteShader(vertexShader);
	if (fragmentShader != -1) glDeleteShader(fragmentShader);
	if (geometryShader != -1) glDeleteShader(geometryShader);
}

int ShaderProgram::loadShader(std::string&& fileName, GLenum type)
{
	std::cout << "    Loading shader \"" << fileName << "\":    ";

	// Try opening the source file for this shader. If it does not open,
	// then we return -1 to indicate that there was a failure to load this
	// shader.
	std::ifstream sourceFile(fileName);
	if (sourceFile.is_open()) {
		std::cout << "Found:    ";

		// read the file into memory so that we can pass it along to OpenGL
		std::stringstream sourceStream;
		sourceStream << sourceFile.rdbuf();
		std::string sourceString = sourceStream.str();
		const char* cString = sourceString.c_str();

		// Create the shader and compile it
		int shaderId = glCreateShader(type);
		checkGl();
		glShaderSource(shaderId, 1, &cString, nullptr);
		checkGl();

		glCompileShader(shaderId);

		// Check that the shader compiled successfully. If it did not, print a
		// message to the programmer and log the errors that OpenGL shows
		int compileSucceded;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSucceded);
		if (compileSucceded == GL_FALSE) {
			// Fetch the length of the log so that we can read in the entire
			// log, rather than just the first N characters of it. 
			int logLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
			checkGl();

			char* logBuffer = new char[logLength + 1];
			glGetShaderInfoLog(shaderId, logLength, nullptr, logBuffer);
			checkGl();
			logBuffer[logLength] = '\0';

			std::cout << "Failed to compile" << std::endl;
			std::cout << "Shader \"" << fileName 
				<< "\" failed to compile. OpenGL Info Log:\n"
				<< logBuffer << std::endl;

			// Cleanup what we allocated
			delete[] logBuffer;
			glDeleteShader(shaderId);
			return -1;
		}
		else {
			std::cout << "Compiled Successfully" << std::endl;
			return shaderId;
		}
	}
	else {
		std::cout << "Not found" << std::endl;
		return -1;
	}
}