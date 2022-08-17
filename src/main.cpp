// External dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard dependencies
#include <stdio.h>
#include <iostream>

// Src dependencies
#include "errors.cpp"
#include "window.cpp"

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void checkCompileShader(unsigned int shader) {
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::SHADER_NUM=" << shader << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		throw CompileShaderFailed();
	}
}

const char* vertex_shader_src =
"#version 330 core \
layout (location = 0) in vec3 aPos; \
void main() \
{ \
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \
}";

const char* fragment_shader_src =
"";

// ENTRYPOINT

int main() {

	// Setup window
	GLFWwindow* window = createWindow();
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// check glad location
	checkGlad();


	// building shader programs
	// ------------------------
	// create vertex shader
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	// compile the vertex shader
	unsigned int n_vshader_src = 1;
	glShaderSource(vertex_shader, n_vshader_src, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	checkCompileShader(vertex_shader);


	// fragment shader
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	checkCompileShader(fragment_shader);

	// vertex data
	// -----------
	float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
	}; 

	// generate vertex buffer object (VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// bind VBO to one buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy vertext data in buffer memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Running
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminate
	glfwTerminate();

	return 0;
}
