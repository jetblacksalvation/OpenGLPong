#include "OpenGLPong.hpp"
#include "stdio.h"
#include "stdlib.h"

int main()
{
	srand(time(NULL));
	puts("running!\n");
	struct ShaderPipelineStruct shaderPipeline;

	InitScreen(&shaderPipeline);
	return 0;
}

