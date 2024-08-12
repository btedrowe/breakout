#pragma once

#include "game.h"

namespace breakout {
  class App {
    public:
      App();
      ~App();

      bool init();
      void run();
    private:
      unsigned int width, height;
      bool running;
      SDL_Window *window;
      SDL_GLContext context;
      breakout::Game breakout;
  };
};
