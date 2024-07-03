#pragma once

namespace breakout {
  class Texture {
    public:
      Texture();

      void generate(unsigned int w, unsigned int h, unsigned char* data);
      void bind() const;

      unsigned int ID;
      
      unsigned int width, height;
      unsigned int internalFormat;
      unsigned int imageFormat;

      unsigned int wrapS;
      unsigned int wrapT;

      unsigned int filterMin;
      unsigned int filterMax;
  };
};