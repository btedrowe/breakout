#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"

namespace breakout {
  class Sprite {
    public:
      Sprite(breakout::Shader shader);
      Sprite(breakout::Shader shader, const float& scale);
      ~Sprite();

      void drawSprite(breakout::Texture texture,
                      glm::vec2 position,
                      glm::vec2 size = glm::vec2(10.0f, 10.0f),
                      float rotate = 0.0f, 
                      glm::vec3 color = glm::vec3(1.0f));
    private:
      breakout::Shader shader;
      unsigned int quadVAO;
      void initRenderData(const float& coordScale);
  };
};
