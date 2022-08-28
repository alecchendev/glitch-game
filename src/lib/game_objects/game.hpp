#ifndef GAME_H
#define GAME_H

#include "world.h"
#include "player.h"

enum GameState {
    Menu,
    Play
};

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