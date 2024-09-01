#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <memory>
#include <string>

#include "OpenGL.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>Z

/** 
 * This is my wrapper for OpenGL's shaders/programs. This class automatically
 * handles the loading of shaders and abstracts the interface for uniforms.
 * The constructor takes the name of a "program group" and loads the shaders
 * with the file names:
 *		<name>.vert
 *		<name>.frag
 *		<name>.geom
 * and links them together. The way this class is implemented is not the best
 * since it loads the file each time, and creates a new shader each time a file
 * is requested, but it is simple to use. 
 * 
 * The uniform functions for this class are the set*() methods of the class,
 * and they take the name of the uniform, a value, and set the uniform
 * accordingly. This implementation is not the most efficient but works well
 * smaller programs where ease of use is more important. 
 * 
 * Instances of this class cannot be initialzied globally since they require
 * OpenGL to be loaded. Global classes are initialized before the main
 * function, but the OpenGL is loaded in the main function. 
 * 
 * There are many different ways you could do this abstraction, and I change
 * mine every few OpenGL programs I write since I learn a bit more about C++
 * and how to handle things. Initial versions of my class had an issue where
 * trying to return a shader instance and pass them would cause the 
 * deconstructor to free the program even though it was still in use. This 
 * version uses a smart pointer to reference count the program id and only 
 * delete the program when all references to a program id are destroyed
 * 
 * Initially inspired by the implementation at LearnOpenGL:
 *		https://learnopengl.com/Getting-started/Shaders
 */
class ShaderProgram
{
public:
	ShaderProgram();

	ShaderProgram(std::string&& name)
	{
		load(name);
	}

	ShaderProgram(std::string& name)
	{
		load(name);
	}

	void load(std::string&& name)
	{
		load(name);
	}

	void load(std::string& name);
		
	// Call this to use this program
	void use() { 
		glUseProgram(*id);
		checkGl();
	}

	// Below are the various uniform functions provided by my abstraction
	void setUint(const std::string& name, GLuint value) {
		glUniform1ui(uniformLoc(name), value);
	}

	void setInt(const std::string& name, GLint value) {
		glUniform1i(uniformLoc(name), value);
	}

	void setFloat(const std::string& name, GLfloat value) {
		glUniform1f(uniformLoc(name), value);
	}

	void setBool(const std::string& name, GLboolean value) {
		glUniform1ui(uniformLoc(name), value);
	}

	void setVec4(const std::string& name, glm::vec4 value) {
		glUniform4f(uniformLoc(name), value.x, value.y, value.z, value.w);
	}

	void setVec3(const std::string& name, glm::vec3 value) {
		glUniform3f(uniformLoc(name), value.x, value.y, value.z);
	}

	void setVec2(const std::string& name, glm::vec2 value) {
		glUniform2f(uniformLoc(name), value.x, value.y);
	}

	void setMat4(const std::string& name, glm::mat4& value) {
		glUniformMatrix4fv(uniformLoc(name), 1, GL_FALSE, glm::value_ptr(value));
	}

private:
	int loadShader(std::string&& fileName, GLenum type);

	GLuint uniformLoc(const std::string& name) {
		return glGetUniformLocation(*id, name.c_str());
	}

	// This shared pointer instance manages the program id for the shader program.
	// When this shader program is coppied, returned, etc, the shared pointer will
	// automatically reference count the number of times this pointer is used, and
	// when the last object is destroyed, will deallocate the shader program.
	std::shared_ptr<unsigned int> id;
};

#endif//SHADER_HPP_INCLUDED