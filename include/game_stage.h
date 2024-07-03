#pragma once

#include <vector>

#include "game_object.h"
#include "sprite.h"


namespace breakout {
  class GameStage {
    public:
      GameStage() {}

      void load(const char* file, unsigned int stageWidth, unsigned int stageHeight);
      void draw(Sprite& renderer);
      bool isComplete();

      std::vector<GameObject> bricks;
    private:
      void init(std::vector<std::vector<unsigned int>> tileData, unsigned int stageWidth, unsigned int stageHeight);
  };
};