#include "texture.h"

#include <SDL2/SDL_opengl.h>

breakout::Texture::Texture()
  : width(0),
    height(0),
    internalFormat(GL_RGB),
    imageFormat(GL_RGB),
    wrapS(GL_REPEAT),
    wrapT(GL_REPEAT),
    filterMin(GL_LINEAR),
    filterMax(GL_LINEAR) {
  glGenTextures(1, &ID);
}

void breakout::Texture::generate(unsigned int w, unsigned int h, unsigned char *data) {
  width = w;
  height = h;

  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void breakout::Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, ID);
}
