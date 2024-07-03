#include "game_stage.h"
#include "resource.h"

#include <fstream>
#include <sstream>

void breakout::GameStage::load(const char *file,
                               unsigned int stageWidth,
                               unsigned int stageHeight) {
  bricks.clear();

  std::ifstream fstream(file);
  std::vector<std::vector<unsigned int>> tileData;

  if (!fstream) {
    std::runtime_error("Failed to open file: " + std::string(file));
  }

  std::string line;

  while (std::getline(fstream, line)) {
    std::istringstream sstream(line);
    std::vector<unsigned int> row;
    unsigned int tileCode;
    while (sstream >> tileCode) {
      row.push_back(tileCode);
    }
    tileData.push_back(row);
  }

  if (!tileData.empty()) {
    this->init(tileData, stageWidth, stageHeight);
  }
}

void breakout::GameStage::draw(Sprite &renderer) {
  for (auto& tile : bricks) {
    if (!tile.isDestroyed) {
      tile.draw(renderer);
    }
  }
}

bool breakout::GameStage::isComplete() {
  for (const auto& tile : bricks) {
    if (!tile.isSolid && !tile.isDestroyed) {
      return false;
    }
  }
  return true;
}

void breakout::GameStage::init(std::vector<std::vector<unsigned int>> tileData,
                               unsigned int stageWidth, 
                               unsigned int stageHeight) {
  int height = tileData.size();
  int width = tileData[0].size();

  float unitWidth = stageWidth / static_cast<float>(width),
        unitHeight = stageHeight / static_cast<float>(height);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      glm::vec2 pos(unitWidth * j, unitHeight * i);
      glm::vec2 size(unitWidth, unitHeight);
      switch (tileData[i][j]) {
        case 1:
          bricks.push_back(GameObject(pos, size,
                                      Resource::getTexture("block_solid"),
                                      glm::vec3(0.8f, 0.8f, 0.7f)));
          bricks.back().isSolid = true;
          break; 
        case 2:
          bricks.push_back(GameObject(pos, size,
                                      Resource::getTexture("block"),
                                      glm::vec3(0.2f, 0.6f, 1.0f)));
          break;
          
        case 3:
          bricks.push_back(GameObject(pos, size,
                                      Resource::getTexture("block"),
                                      glm::vec3(0.0f, 0.7f, 0.0f)));
          break;
        case 4:
          bricks.push_back(GameObject(pos, size,
                                      Resource::getTexture("block"),
                                      glm::vec3(0.8f, 0.8f, 0.4f)));
          break;
        case 5:
          bricks.push_back(GameObject(pos, size,
                                      Resource::getTexture("block"),
                                      glm::vec3(1.0f, 0.5f, 0.0f)));
          break;
      }
    }
  }
}
