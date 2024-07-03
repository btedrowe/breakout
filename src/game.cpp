#include "game.h"
#include "resource.h"
#include "sprite.h"


breakout::Sprite* renderer;
breakout::Sprite* background;
breakout::GameObject* player;
breakout::BallObject* ball;

const glm::vec2 BALL_INITIAL_VELOCITY(100.0f, -350.0f);
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

const float BALL_RADIUS = 12.5f;

breakout::Game::Game(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE) {}

breakout::Game::~Game() {
  delete renderer;
  delete player;
}

void breakout::Game::init() {
  breakout::Resource::loadShader("assets/shaders/sprite.vs", "assets/shaders/sprite.fs", nullptr, "sprite");

  glm::mat4 projection = glm::ortho(0.0f,
                                    static_cast<float>(width),
                                    static_cast<float>(height),
                                    0.0f, -1.0f, 1.0f);
  breakout::Resource::getShader("sprite").use().setInteger("image", 0);
  breakout::Resource::getShader("sprite").setMatrix4("projection", projection);

  background = new breakout::Sprite(Resource::getShader("sprite"), 4.0f);
  renderer = new breakout::Sprite(Resource::getShader("sprite"));

  breakout::Resource::loadTexture("assets/textures/background.jpg", false, "background");
  breakout::Resource::loadTexture("assets/textures/ball.png", true, "face");
  breakout::Resource::loadTexture("assets/textures/block.png", true, "block");
  breakout::Resource::loadTexture("assets/textures/block_solid.png", true, "block_solid");
  breakout::Resource::loadTexture("assets/textures/paddle.png", true, "paddle");

  stages = std::vector<breakout::GameStage>(4);
  stages[0].load("assets/levels/1.lvl", width, height/2);
  stages[1].load("assets/levels/2.lvl", width, height/2);
  stages[2].load("assets/levels/3.lvl", width, height/2);
  stages[3].load("assets/levels/4.lvl", width, height/2);

  stage = 0;
  glm::vec2 playerPos((width - PLAYER_SIZE.x)/2.0f, height - PLAYER_SIZE.y);
  glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x/2.0f - BALL_RADIUS, -BALL_RADIUS*2.0f);

  player = new GameObject(playerPos, PLAYER_SIZE, breakout::Resource::getTexture("paddle"));
  ball = new BallObject(ballPos, BALL_RADIUS, BALL_INITIAL_VELOCITY, breakout::Resource::getTexture("face"));
}

bool breakout::Game::checkCollision(BallObject &c, GameObject &r) {
  std::vector<glm::vec2> verts = r.getVertices();
  std::vector<glm::vec2> norms = r.getEdgeNormals();

  glm::vec2 cCenter = c.position + c.radius;

  for (const auto& norm : norms) {
    float minPolyProjection = INFINITY;
    float maxPolyProjection = -INFINITY;

    for (const auto& vert : verts) {
      float proj = glm::dot(vert, norm);
      minPolyProjection = std::min(minPolyProjection, proj);
      maxPolyProjection = std::max(maxPolyProjection, proj);
    }

    float cProjection = glm::dot(cCenter, norm);

    float minCProjection = cProjection - c.radius;
    float maxCProjection = cProjection + c.radius;

    if (!(maxPolyProjection >= minCProjection && maxCProjection >= minPolyProjection)) {
      return false;
    }
  }
  return true;
}

std::pair<float, float> breakout::Game::projectPolyGon(const std::vector<glm::vec2> &vertices,
                                                       const glm::vec2 &axis) {
  float min = glm::dot(vertices[0], axis);
  float max = min;

  for (const auto& vertex : vertices) {
    float proj = glm::dot(vertex, axis);
    if (proj < min) min = proj;
    if (proj > max) max = proj;
  }

  return std::make_pair(min, max);
}

void breakout::Game::calculateCollisions() {
  for (auto& box : stages[stage].bricks) {
    if (!box.isDestroyed) {
      if (checkCollision(*ball, box)) {
        if (!box.isSolid) {
          box.isDestroyed = true;
        }
        glm::vec2 sizeBox = 0.5f*box.size;
        glm::vec2 posBox = box.position + sizeBox;
        glm::vec2 BB = (ball->position+ball->radius)-posBox;
        glm::vec2 BBClamped = glm::clamp(BB, -sizeBox, sizeBox);

        if (std::abs(BB.x) > std::abs(BB.y)) {
            ball->velocity.x *= -1.0f;
            ball->position.x = posBox.x + BBClamped.x + (BB.x > 0 ? 0.0f : -2.0f*ball->radius);
        } else {
            ball->velocity.y *= -1.0f;
            ball->position.y = posBox.y + BBClamped.y + (BB.y > 0 ? 0.0f : -2.0f*ball->radius);
        }
        break;
      }
    }
  }

  if (!ball->stuck && checkCollision(*ball, *player)) {
    ball->velocity.y = -1.0f * abs(ball->velocity.y);
    glm::vec2 P2B = (ball->position + glm::vec2(ball->radius)) - (player->position + 0.5f*player->size);
    ball->velocity.x = P2B.x * BALL_INITIAL_VELOCITY.y / P2B.y;
    ball->velocity.x = std::max(-800.0f, std::min(ball->velocity.x, 800.0f));
  }
}

void breakout::Game::processInput(SDL_Event& event, bool& running, float dt) {
  const float playerVelocity = 500.0f;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
    } else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          inputs.left = true;
          break;
        case SDLK_RIGHT:
          inputs.right = true;
          break;
        case SDLK_SPACE:
          inputs.space = true;
          break;
        default:
          break;
      }
    } else if (event.type == SDL_KEYUP) {
      switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          inputs.left = false;
          break;
        case SDLK_RIGHT:
          inputs.right = false;
          break;
        case SDLK_SPACE:
          inputs.space = false;
          break;
        default:
          break;
      }
    }
  }
   
  if (state == GAME_ACTIVE) {
    float dx = playerVelocity * dt;
    if (inputs.left && player->position.x >= 0.0f) {
      player->position.x -= dx;
      if (ball->stuck) {
          ball->position.x -= dx;
      }
    }
    if (inputs.right && player->position.x <= width - player->size.x) {
      player->position.x += dx;
      if (ball->stuck) {
          ball->position.x += dx;
      }
    }
    if (inputs.space) {
      ball->stuck = false;
    }
  }
}

void breakout::Game::update(float dt) {
  ball->move(dt, width);
  calculateCollisions();
}

void breakout::Game::render() {
  if (state == GAME_ACTIVE) {
    background->drawSprite(breakout::Resource::getTexture("background"),
                         glm::vec2(0.0f, 0.0f),
                         glm::vec2(width, height), 0.0f);
    stages[stage].draw(*renderer);
    player->draw(*renderer);
    ball->draw(*renderer);
  }
}
