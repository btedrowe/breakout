#pragma once
#include <png.h>

namespace breakout {
  void user_error_fn(png_structp, png_const_charp error_msg);
  void user_warning_fn(png_structp, png_const_charp warning_msg);
  bool loadPNG(const char* filename, unsigned char** imageData, int* width, int* height, int* channels);
};
