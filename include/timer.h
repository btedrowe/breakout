#pragma once

#include <functional>

namespace breakout {
  class Timer {
    public:
      Timer();

      void start(int d, std::function<void()> c);
      void update(float dt);
      void reset();

      bool isRunning();
      bool isExpired();

    private:
      float duration;
      float elapsed;
      std::function<void()> callback;
      bool running;
      bool expired;
  };
};
