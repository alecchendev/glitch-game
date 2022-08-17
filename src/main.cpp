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

void checkLinkShaderProgram(unsigned int shader_program) {
	int success;
	char info_log[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER_PROGRAM::PROGRAM_NUM=" << shader_program << "::LINKING_FAILED\n" << info_log << std::endl;
		throw LinkShaderProgramFailed();
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
"#version 330 core \
out vec4 FragColor; \
void main() \
{ \
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \
}";

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
	// vertex shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int n_vshader_src = 1;
	glShaderSource(vertex_shader, n_vshader_src, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	checkCompileShader(vertex_shader);

	// fragment shader
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int n_fshader_src = 1;
	glShaderSource(fragment_shader, n_fshader_src, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	checkCompileShader(fragment_shader);

	// create shader program
	unsigned int shader_program = glCreateProgram();

	// attach shaders
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	// delete shaders
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// link program
	glLinkProgram(shader_program);
	checkLinkShaderProgram(shader_program);

	// use program
	glUseProgram(shader_program);



	// vertex data
	// -----------
	float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
	}; 

	// Vertex Array Objects (VAO)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object (VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindVertexArray(0);

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

		// render triangle
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

	// terminate
	glfwTerminate();

	return 0;
}
