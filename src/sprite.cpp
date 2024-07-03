#include <GL/glew.h>
#include "sprite.h"
#include <SDL2/SDL_opengl.h>

breakout::Sprite::Sprite(breakout::Shader shader) : shader(shader) {
  initRenderData(1.0f);
}

breakout::Sprite::Sprite(breakout::Shader shader, const float& scale) : shader(shader) {
  initRenderData(scale);
}

breakout::Sprite::~Sprite() {
  glDeleteVertexArrays(1, &quadVAO);
}

void breakout::Sprite::drawSprite(breakout::Texture texture, 
                                  glm::vec2 position, 
                                  glm::vec2 size, 
                                  float rotate, 
                                  glm::vec3 color) {
  shader.use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 1.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  shader.setMatrix4("model", model);

  shader.setVector3f("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture.bind();

  glBindVertexArray(quadVAO);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void breakout::Sprite::initRenderData(const float& coordScale) {
  unsigned int VBO;
  float vertices[] = {
    0.0f, 1.0f, 0.0f, coordScale,
    1.0f, 0.0f, coordScale, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, coordScale,
    1.0f, 1.0f, coordScale, coordScale,
    1.0f, 0.0f, coordScale, 0.0f
  };
  
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(quadVAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
