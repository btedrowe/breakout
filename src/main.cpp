#include "app.h"

int main() {
  breakout::App breakout(800, 600);

  if (!breakout.init()) {
    return -1;
  }

  breakout.run();
  
  return 0;
}