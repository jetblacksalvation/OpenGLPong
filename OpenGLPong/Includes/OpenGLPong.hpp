// OpenGLPong.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "time.h"
#include <string>
#include <vector>
struct GlslErrorsStruct {
	std::vector<unsigned int> shader_ids; 
	std::vector<std::string> error_messages;

};
// TODO: Reference additional headers your program requires here.
struct ShaderPipelineStruct {
	GLFWwindow* _window;
	// OpenGL-related members
	unsigned int _VAO;          // Vertex Array Object
	unsigned int _VBO;          // Vertex Buffer Object
	unsigned int _shaderProgram; // Shader Program
	char _shaderInfo[255];

};
/*
	pass by pointer.. This struct is larger than maximum
*/
struct ShaderCompileSources
{
	char* vertexShaderSource;
	char* fragmentShaderSource;
	void* vertices;
	int x, y; 
	uint64_t verticesSize; 
};
GLuint compile_glsl_string(GLenum type, GLchar* const source);
void get_glsl_error(struct ShaderPipelineStruct*, GLuint shaderid);

void InitScreen(struct ShaderPipelineStruct* This);
void CompileShaders(struct ShaderPipelineStruct* This, struct ShaderCompileSources);