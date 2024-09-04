#include "OpenGL.hpp"

#include <iostream>

// This function checks for OpenGL errors, and if one occurs, it prints
// the error information and the file/line it occured on.
//
// More info can be found here: 
//	* https://www.khronos.org/opengl/wiki/OpenGL_Error
void checkForGlErrors(const char* file, int line)
{
	const char* errorStr = "";

	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		switch (error) {
		case GL_INVALID_ENUM:
			errorStr = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errorStr = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errorStr = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errorStr = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errorStr = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errorStr = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			errorStr = "Unknown OpenGL Error";
			break;
		}

		std::cerr << "OpenGL Error (" << file << ":" << line << "): " << error
			<< ": " << errorStr << std::endl;
	}
}


void checkForGlErrors2(const char* file, int line, const char* func, const char* glFunc)
{
	const char* errorStr = "";

	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		switch (error) {
		case GL_INVALID_ENUM:
			errorStr = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errorStr = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errorStr = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errorStr = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errorStr = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errorStr = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			errorStr = "Unknown OpenGL Error";
			break;
		}

		std::cerr << "OpenGL Error (" << file << ":" << line << "  " << glFunc << "): " << error
			<< ": " << errorStr << std::endl;
	}
}
