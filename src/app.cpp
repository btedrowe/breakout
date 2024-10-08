#include "app.h"
#include "resource.h"

breakout::App::App() : width(800), height(600), breakout(Game(800, 600)), running(false) {}

breakout::App::~App() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool breakout::App::init() {
  Resource::loadConf("assets/config/video.cfg");

  Resource::setConf("width", width);
  Resource::setConf("height", height);

  window = SDL_CreateWindow("Breakout", 0, 0, width, height, SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    return false; //failed to create SDL window
  }

  context = SDL_GL_CreateContext(window);

  GLenum glewErr = glewInit();

  if (glewErr != GLEW_OK) {
    return false; //failed to initialize GLEW
  }

  glViewport(0, 0, width, height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return true;
}

void breakout::App::run() {
  running = true;

  breakout.init();

  float dTime = 0.0f;
  float pTime = SDL_GetTicks();

  while (running) {
    SDL_Event event;

    float cTime = SDL_GetTicks();
    dTime = (cTime - pTime)*0.001f;
    pTime = cTime;
    
    breakout.processInput(event, running, dTime);
    
    breakout.update(dTime);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    breakout.render();

    SDL_GL_SwapWindow(window);
  }
}
