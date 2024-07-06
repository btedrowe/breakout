#include "image.h"
#include <iostream>
#include <cstdio>
#include <png.h>
#include <pngconf.h>

void breakout::user_error_fn(png_structp, png_const_charp error_msg) {
  std::cerr << "PNG error: " << error_msg << std::endl;
  exit(1);
}

void breakout::user_warning_fn(png_structp, png_const_charp warning_msg) {
  std::cerr << "PNG warning: " << warning_msg << std::endl;
}

bool breakout::loadPNG(const char* filename, unsigned char** imageData, int* width, int* height, int* channels) {
  FILE* pngFile = fopen(filename, "rb");
  if (pngFile == nullptr) {
    std::cerr << "Error: Failed to open file: " << filename << std::endl;
    return false;
  }

  png_byte sig[8];

  if (fread(sig, 1, 8, pngFile)) {
    if (png_sig_cmp(sig, 0, 8) != 0) {
      std::cerr << "Error: Wrong PNG signature in file: " << filename << std::endl;
    }
  } else {
    std::cerr << "Error: failed to read PNG signature in file: " << filename << std::endl;
    fclose(pngFile);
    pngFile = nullptr;
    return false;
  }

  png_structp pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, user_error_fn, nullptr);
  if (pngStruct == nullptr) {
    std::cerr << "Error: failed to create PNG struct for file: " << filename << std::endl;
    return false;
  }

  png_infop pngInfo = png_create_info_struct(pngStruct);
  if (pngInfo == nullptr) {
    std::cerr << "Error: failed to create PNG info struct for file" << filename << std::endl;
    png_destroy_read_struct(&pngStruct, nullptr, nullptr);
    pngStruct = nullptr;
    return false;
  }

  png_infop pngEndInfo = png_create_info_struct(pngStruct);
  if (pngEndInfo == nullptr) {
    std::cerr << "Error: failed to create PNG end info struct for file" << filename << std::endl;
    png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
    pngStruct = nullptr;
    pngInfo = nullptr;
    return false;
  }

  try {
    png_init_io(pngStruct, pngFile);
    png_set_sig_bytes(pngStruct, 8);
    png_set_keep_unknown_chunks(pngStruct, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0);

    png_read_info(pngStruct, pngInfo);

    *width = png_get_image_width(pngStruct, pngInfo);
    *height = png_get_image_height(pngStruct, pngInfo);
    *channels = png_get_channels(pngStruct, pngInfo);

    int rowBytes = png_get_rowbytes(pngStruct, pngInfo);
    *imageData = new png_byte[rowBytes * (*height)];

    png_bytep* rowPointers = new png_bytep[*height];

    for (int i = *height-1; i >= 0; i--) {
      rowPointers[i] = *imageData + i * rowBytes;
    }

    png_read_image(pngStruct, rowPointers);
    png_read_end(pngStruct, pngEndInfo);

    fclose(pngFile);
    pngFile = nullptr;

    delete[] rowPointers;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  png_destroy_read_struct(&pngStruct, &pngInfo, &pngEndInfo);
  pngStruct = nullptr;
  pngInfo = nullptr;
  pngEndInfo = nullptr;

  return true;
}

