#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "sprite.h"
#include "texture.h"

namespace breakout {
  class GameObject {
    public:
      GameObject();
      GameObject(glm::vec2 p, glm::vec2 s, Texture spr,
                 glm::vec3 c = glm::vec3(1.0f),
                 glm::vec2 v = glm::vec2(0.0f, 0.0f));
      glm::vec2 position, size, velocity;
      glm::vec3 color;
      float rotation;
      bool isSolid;
      bool isDestroyed;

      Texture sprite;

      virtual void draw(Sprite& renderer);
      std::vector<glm::vec2> getVertices() const;
      std::vector<glm::vec2> getEdgeNormals() const;
  };

  class BallObject : public GameObject {
    public:
      BallObject();
      BallObject(glm::vec2 p, float r, glm::vec2 v, Texture s);

      float radius;
      bool stuck;

      glm::vec2 move(float dt, unsigned int windowWidth);
      void reset(glm::vec2 p, glm::vec2 v);
  };
};