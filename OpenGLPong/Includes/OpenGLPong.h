// OpenGLPong.h : Include file for standard system include files,
// or project specific include files.

#pragma once


#include <glad/gl.h>
#include <GLFW/glfw3.h>



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
	pass by pointer.. this struct is larger than maximum
*/
struct ShaderCompileSources
{
	char* vertexShaderSource;
	char* fragmentShaderSource;
	float* vertices;
	int x, y; 
	uint64_t verticesSize; 
};
GLuint compile_glsl_string(GLenum type, GLchar* const source);
void get_glsl_error(struct ShaderPipelineStruct*, GLuint shaderid);

void InitScreen(struct ShaderPipelineStruct* this);
void CompileShaders(struct ShaderPipelineStruct* this, struct ShaderCompileSources);