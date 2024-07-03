#include "game_object.h"

breakout::GameObject::GameObject()
  : position(0.0f, 0.0f),
    size(1.0f, 1.0f),
    velocity(0.0f),
    color(1.0f),
    rotation(0.0f),
    sprite(),
    isSolid(false),
    isDestroyed(false) {}

breakout::GameObject::GameObject(glm::vec2 p,
                                 glm::vec2 s, 
                                 Texture spr, 
                                 glm::vec3 c, 
                                 glm::vec2 v)
  : position(p),
    size(s),
    velocity(v),
    color(c),
    rotation(0.0f),
    sprite(spr),
    isSolid(false),
    isDestroyed(false) {}

void breakout::GameObject::draw(Sprite &renderer) {
  renderer.drawSprite(sprite, position, size, rotation, color);
}

std::vector<glm::vec2> breakout::GameObject::getVertices() const {
  std::vector<glm::vec2> verts(4);

  verts[0] = position;
  verts[1] = position+glm::vec2(0.0f, size.y);
  verts[2] = position+size;
  verts[3] = position+glm::vec2(size.x, 0.0f);

  return verts;
}

std::vector<glm::vec2> breakout::GameObject::getEdgeNormals() const {
  std::vector<glm::vec2> verts = getVertices();
  std::vector<glm::vec2> norms(4);
  
  for (int i = 0; i < 4; i++) {
    glm::vec2 edge = verts[(i + 1) % 4] - verts[i];
    norms[i] = glm::normalize(glm::vec2(-edge.y, edge.x));
  }

  return norms;
}

breakout::BallObject::BallObject() : GameObject(), radius(12.5f), stuck(true) {}

breakout::BallObject::BallObject(glm::vec2 p, float r, glm::vec2 v, Texture s)
  : GameObject(p, glm::vec2(r * 2.0f, r * 2.0f), s, glm::vec3(1.0f), v),
    radius(r),
    stuck(true) {}

glm::vec2 breakout::BallObject::move(float dt, unsigned int windowWidth) {
  if (!stuck) {
    position += velocity * dt;
    if (position.y <= 0.0f) {
      velocity.y = -velocity.y;
      position.y = 0.0f;
    }

    if (position.x <= 0.0f) {
      velocity.x = -velocity.x;
      position.x = 0.0f;
    } else if (position.x + size.x >= windowWidth) {
      velocity.x = -velocity.x;
      position.x = windowWidth - size.x;
    }
  }
  return position;
}

void breakout::BallObject::reset(glm::vec2 p, glm::vec2 v) {
  position = p;
  velocity = v;
  stuck = true;
}
