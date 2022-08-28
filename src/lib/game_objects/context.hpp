
#include <GLFW/glfw3.h>
#include "game.hpp"
#include "graphics_engine.hpp"

// Overarching game context that connects input processing to game logic to rendering graphics
class Context {
  public:
    Context();
    void run();

  private:
    GraphicEngine graphics_engine;
    Game game;
    GLFWwindow* window;
};