#ifndef GAME_H
#define GAME_H

#include "world.hpp"
#include "player.hpp"

enum GameState {
    Menu,
    Play
};

// Class that handles all game logic
class Game {
  public:
    Game();

    void run();

  private:
    GameState game_state;
    Player player;
    World world;
};

#endif