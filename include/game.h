#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "game_stage.h"

namespace breakout {
  enum GameState {GAME_ACTIVE, GAME_MENU, GAME_WIN};

  struct Input {
    Input() : left(false), right(false), space(false) {}
    bool left;
    bool right;
    bool space;
  };

  class Game {
    public:
      Game(unsigned int w, unsigned int h);
      ~Game();

      void init();

      void processInput(SDL_Event& event, bool& running, float dt);
      void update(float dt);
      void render();

      GameState state;
      Input inputs;
      unsigned int width, height;
    private:
      std::vector<breakout::GameStage> stages;
      unsigned int stage;
      bool win;

      bool checkCollision(BallObject& c, GameObject& r);
      void calculateCollisions();
  };
};
