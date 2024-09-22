#include <functional>
void set_gl_buffer(struct ShaderPipelineStruct* This, struct ShaderCompileSources*);
void InitScreen(struct ShaderPipelineStruct* This);
void CompileShaders(struct ShaderPipelineStruct* This, struct ShaderCompileSources*);

struct ShaderPipelineStruct {
	GLFWwindow* _window;
	// OpenGL-related members
	unsigned int _VAO;          // Vertex Array Object
	unsigned int _VBO;          // Vertex Buffer Object
	unsigned int _shaderProgram; // Shader Program
	char _shaderInfo[255];
	struct ShaderFunctionsStruct {
		/*
			two function pointers with interchangeable args, one is called to compile and link, but can be used for other things. this is called once when shader is loaded.
			the other one is called every game loop, its callled to set vector position/rendering.
		*/
		std::function<void(ShaderPipelineStruct*, ShaderCompileSources*)> every_loop_function = set_gl_buffer;
		std::function<void(ShaderPipelineStruct*, ShaderCompileSources*)> on_shader_load = CompileShaders;
		int hasRun = 0; 



	};
	ShaderFunctionsStruct _shader_functions;
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
