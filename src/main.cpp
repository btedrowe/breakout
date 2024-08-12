#include "app.h"

int main() {
  breakout::App breakout;

  if (!breakout.init()) {
    return -1;
  }

  breakout.run();
  
  return 0;
}
