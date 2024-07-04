#include "timer.h"

breakout::Timer::Timer() : duration(0.0f), elapsed(0.0f), running(false), expired(false) {}

void breakout::Timer::start(int d, std::function<void()> c) {
  duration = d;
  elapsed = 0.0f;
  callback = c;
  running = true;
  expired = false;
}

void breakout::Timer::update(float dt) {
  if (running) {
    elapsed += dt;
    if (elapsed >= duration) {
      running = false;
      expired = true;
      callback();
    }
  }
}

void breakout::Timer::reset() {
  running = false;
  expired = false;
  elapsed = 0.0f;
}

bool breakout::Timer::isRunning() {
  return running;
}

bool breakout::Timer::isExpired() {
  return expired;
}
