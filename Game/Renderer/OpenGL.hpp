#ifndef OPENGL_HPP_INCLUDED
#define OPENGL_HPP_INCLUDED

// The glad file should be included before any other fiels that use OpenGL
// since it overrides the OpenGL definitions.
#include <glad/glad.h>

// This is a simple macro for checking opengl calls for errors. It expands the
// file name and line number that the macro is called on and passes them to
// a function that checks for OpenGL errors.
//
// The macro expands to nothing in release mode for increase performance.
#ifndef NDEBUG
	#define checkGl()	checkForGlErrors(__FILE__, __LINE__);
#else
	#define checkGl()	
#endif

#define callGl(name, ...)		\
	name ( __VA_ARGS__ ); checkForGlErrors2(__FILE__, __LINE__, __func__, #name );

void checkForGlErrors(const char* file, int line);

void checkForGlErrors2(const char* file, int line, const char* func, const char* glFunc);

#endif//OPENGL_HPP_INCLUDED