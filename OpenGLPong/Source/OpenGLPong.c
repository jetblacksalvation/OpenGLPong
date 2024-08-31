// OpenGLPong.cpp : Defines the entry point for the application.
//

#include "OpenGLPong.h"
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
static GLuint compile_glsl_string(GLenum type, GLchar* const source)
{
    GLuint shader;
    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}
void get_glsl_error(struct ShaderPipelineStruct* shaderStruct,GLuint shaderid)
{
    int success;
    char infoLog[512];
    memset(shaderStruct->_shaderInfo, 0, 255);
    //std::stringstream ret;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderid, 512, NULL, infoLog);
        char infoToWrite[255] = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n\0";
        int endBaseStr = strlen(infoToWrite);
        for (int x = endBaseStr; x < 255; x++)
        {
            infoToWrite[x] = infoLog[x];
        }
        shaderStruct->_shaderInfo;
        memcpy(shaderStruct->_shaderInfo, infoToWrite, 255);
        //ret << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void InitScreen(struct ShaderPipelineStruct * this) 
{
    // Initialize GLFW
    if (!glfwInit()) {
        puts("Failed to initialize GLFW");
        return;
    }
    // Create a windowed mode window and its OpenGL context
    this->_window = glfwCreateWindow(800, 600, "Shin Megami Tensei", NULL, NULL);
    if (!this->_window) {
        puts("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(this->_window);
    int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0) {
        puts("Failed to initialize GLAD");
        return;
    }
    puts("GLAD initialized, version: ");

    // Set the viewport
    glViewport(0, 0, 800, 600);

    

    struct ShaderCompileSources shaders;
    shaders.vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 vertexColor;\n"
"varying highp vec3 color;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"color = vertexColor;\n"
"}\n";
    shaders.fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "varying lowp vec3 color;\n"
        "void main()\n"
        "{\n"
        "\n"
        "    FragColor = vec4(color, 1.0f); // Change to red for testing\n"
        "}\n";
    // Build and compile the shaders
    // 
    shaders.vertices = malloc(sizeof(float) * (3 * 6));
    float vertices[] = {
        // Positions        // Colors
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top-right corner
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right corner
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-left corner
    };
    memcpy_s(shaders.vertices, 3 * 6, vertices, 3 * 6);

    CompileShaders(this, shaders);
    

    while (!glfwWindowShouldClose(this->_window)) {
        // Input
        if (glfwGetKey(this->_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(this->_window, 1);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(this->_shaderProgram);

        // Bind the VAO
        glBindVertexArray(this->_VAO);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 6);  // or glDrawElements if using indices
        // Swap front and back buffers
        glfwSwapBuffers(this->_window);

        // Poll for and process events
        glfwPollEvents();  // Ensure this is called to handle window events

        // Optional: Limit the frame rate
        
        //std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    // Cleanup
    glDeleteVertexArrays(1, &this->_VAO);
    glDeleteBuffers(1, &this->_VBO);
    glDeleteProgram(this->_shaderProgram);

    glfwDestroyWindow(this->_window);
    glfwTerminate();

}
void CompileShaders(struct ShaderPipelineStruct* this, struct ShaderCompileSources sources)
{
    // Build and compile the shaders
    unsigned int vertexShader = compile_glsl_string(GL_VERTEX_SHADER, (GLchar*)sources.vertexShaderSource);

    glBindAttribLocation(vertexShader, 0, "aPos");
    glBindAttribLocation(vertexShader, 1, "vertexColor");

    get_glsl_error(this, vertexShader);
    puts(this->_shaderInfo);

    unsigned int fragmentShader = compile_glsl_string(GL_FRAGMENT_SHADER, (GLchar*)sources.fragmentShaderSource);
    get_glsl_error(this, fragmentShader);
    puts(this->_shaderInfo);

    // Link the shaders into a program
    this->_shaderProgram = glCreateProgram();
    glAttachShader(this->_shaderProgram, vertexShader);
    glAttachShader(this->_shaderProgram, fragmentShader);

    glLinkProgram(this->_shaderProgram);

    get_glsl_error(this, this->_shaderProgram);
    puts(this->_shaderInfo);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and configure vertex attributes
    float vertices[] = {
        // Positions        // Colors
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top-right corner
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right corner
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-left corner
    };


    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);

    glBindVertexArray(this->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    uint64_t offset = (sizeof(vertices) / sizeof(vertices[0]));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}