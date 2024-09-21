// OpenGLPong.cpp : Defines the entry point for the application.
//

#include "OpenGLPong.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <iostream>
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

void InitScreen(struct ShaderPipelineStruct * This) 
{
    // Initialize GLFW
    if (!glfwInit()) {
        puts("Failed to initialize GLFW");
        return;
    }
    // Create a windowed mode window and its OpenGL context
    This->_window = glfwCreateWindow(800, 600, "Shin Megami Tensei", NULL, NULL);
    if (!This->_window) {
        puts("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(This->_window);
    int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0) {
        puts("Failed to initialize GLAD");
        return;
    }
    puts("GLAD initialized, version: ");

    // Set the viewport
    glViewport(0, 0, 800, 600);

    

    struct ShaderCompileSources shaders;
    shaders.vertexShaderSource = (char* )R"(
#version 330 core
uniform vec3 aPos;        
uniform vec3 vertexColor; 
varying highp vec3 color;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = vertexColor;
}
)";
    shaders.fragmentShaderSource = (char*)R"(
#version 330 core
out vec4 FragColor;
varying lowp vec3 color;
void main()
{
    FragColor = vec4(color, 1.0f); // Change to red for testing
}
)";


    // Build and compile the shaders
    // 
    shaders.vertices = malloc(sizeof(float) * (3 * 6));
    float vertices[] = {
        // Positions        // Colors
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top-right corner
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right corner
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-left corner
    };


    CompileShaders(This, shaders);
    

    while (!glfwWindowShouldClose(This->_window)) {
        // Input
        if (glfwGetKey(This->_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(This->_window, 1);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(This->_shaderProgram);

        glPointSize(10.0f); // Increase the point size to 10.0

        GLuint pos_id = glGetUniformLocation(This->_shaderProgram, "aPos");
        GLuint vertexColor_id = glGetUniformLocation(This->_shaderProgram, "vertexColor");
        if(pos_id == GL_INVALID_VALUE)std::cout<<"fucked\n";
        if(vertexColor_id == GL_INVALID_VALUE)std::cout << "fucked\n";;
        // For Vertex 1 (Top-right corner)
        glUniform3f(glGetUniformLocation(This->_shaderProgram, "aPos"), 0.5f, 0.5f, 0.0f);
        glUniform3f(glGetUniformLocation(This->_shaderProgram, "vertexColor"), 1.0f, 0.0f, 0.0f);

        // Draw the first vertex

        // For Vertex 2 (Bottom-right corner)
        glUniform3f(glGetUniformLocation(This->_shaderProgram, "aPos"), 0.5f, -0.5f, 0.0f);
        glUniform3f(glGetUniformLocation(This->_shaderProgram, "vertexColor"), 0.0f, 1.0f, 0.0f);

        // Draw the second vertex

        // For Vertex 3 (Bottom-left corner)
        glUniform3f(glGetUniformLocation(This->_shaderProgram, "aPos"), -0.5f, -0.5f, 0.0f);
        glUniform3f(glGetUniformLocation(This->_shaderProgram, "vertexColor"), 0.0f, 0.0f, 1.0f);

        // Draw the third vertex
        glDrawArrays(GL_TRIANGLES, 0, 3);


        //// Draw the triangle
        //glDrawArrays(GL_TRIANGLES, 0, 6);  // or glDrawElements if using indices
        // Swap front and back buffers
        glfwSwapBuffers(This->_window);

        // Poll for and process events
        glfwPollEvents();  // Ensure This is called to handle window events

        // Optional: Limit the frame rate
        
        //std::This_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    // Cleanup
    glDeleteVertexArrays(1, &This->_VAO);
    glDeleteBuffers(1, &This->_VBO);
    glDeleteProgram(This->_shaderProgram);

    glfwDestroyWindow(This->_window);
    glfwTerminate();

}
void CompileShaders(struct ShaderPipelineStruct* This, struct ShaderCompileSources sources)
{
    // Build and compile the shaders
    unsigned int vertexShader = compile_glsl_string(GL_VERTEX_SHADER, (GLchar*)sources.vertexShaderSource);

    glBindAttribLocation(vertexShader, 0, "aPos");
    glBindAttribLocation(vertexShader, 1, "vertexColor");

    get_glsl_error(This, vertexShader);
    puts(This->_shaderInfo);

    unsigned int fragmentShader = compile_glsl_string(GL_FRAGMENT_SHADER, (GLchar*)sources.fragmentShaderSource);
    get_glsl_error(This, fragmentShader);
    puts(This->_shaderInfo);

    // Link the shaders into a program
    This->_shaderProgram = glCreateProgram();
    glAttachShader(This->_shaderProgram, vertexShader);
    glAttachShader(This->_shaderProgram, fragmentShader);

    glLinkProgram(This->_shaderProgram);

    get_glsl_error(This, This->_shaderProgram);
    puts(This->_shaderInfo);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Set up vertex data and configure vertex attributes
    float vertices[] = {
        // Positions        // Colors
         0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top-right corner
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right corner
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-left corner
    };
    
    
    glGenVertexArrays(1, &This->_VAO);
    glGenBuffers(1, &This->_VBO);

    glBindVertexArray(This->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, This->_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    uint64_t offset = (sizeof(vertices) / sizeof(vertices[0]));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}