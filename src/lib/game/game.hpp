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
    Game(World first_world):
      game_state_(Play),
      player_(Player(first_world.startPosition(), first_world.startDirection())),
      world_(first_world) {}

    void run() {}

  private:
    GameState game_state_;
    Player player_;
    World world_;
};

#endif