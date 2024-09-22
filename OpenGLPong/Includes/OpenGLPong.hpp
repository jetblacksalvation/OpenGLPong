// OpenGLPong.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "ShaderStuff.hpp"

struct GlslErrorsStruct {
	std::vector<unsigned int> shader_ids; 
	std::vector<std::string> error_messages;

};
// TODO: Reference additional headers your program requires here.

GLuint compile_glsl_string(GLenum type, GLchar* const source);
