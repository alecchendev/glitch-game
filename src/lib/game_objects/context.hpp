
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.hpp"
#include "graphics_engine.hpp"

// Overarching game context that connects input processing to game logic to rendering graphics
class Context {
  public:
    struct ContextInit {
        int screen_width;
        int screen_height;
        std::string window_name;
    };

    Context(ContextInit& init);
    void run();
    void stop();

  private:
    GraphicsEngine graphics_engine_;
    Game game_;
    GLFWwindow* window_;

    GLFWwindow* createWindow(const int screen_width, const int screen_height, const std::string& window_name);
    void checkGlad();
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    void processInput();
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

Context::Context(ContextInit& init) {

    graphics_engine_ = GraphicsEngine();
    game_ = Game();

    // Setup window
	window_ = createWindow(init.screen_width, init.screen_height, init.window_name);
	glfwMakeContextCurrent(window_);
	glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
}

void Context::run() {


    // Setup cursor input
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window_, mouseCallback);

	// Check glad location
	checkGlad();

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

    // Actually run
	while (!glfwWindowShouldClose(window_)) {
        // Process input
        processInput();
        // Step game
        // Render graphics
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window_);
		glfwPollEvents();
    }

    // Delete graphics objects

    glfwTerminate();

}

void Context::stop() {
    exit(1);
}

GLFWwindow* Context::createWindow(const int screen_width, const int screen_height, const std::string& window_name) {

	// init/config glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create window object
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, window_name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        stop();
	}
    return window;
 
}

void Context::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Context::processInput()
{
	if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_, true);
}

void Context::mouseCallback(GLFWwindow* window, double xpos, double ypos) {

}

void Context::checkGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
        stop();
	}
}
