#pragma once

#include <map>
#include <string>

#include "shader.h"
#include "texture.h"

namespace breakout {
  class Resource {
    public:
      Resource() = delete;

      static std::map<std::string, Shader> shaders;
      static std::map<std::string, Texture> textures;

      static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
      static Shader getShader(std::string name);
      
      static Texture loadTexture(const char* file, bool alpha, std::string name);
      static Texture getTexture(std::string name);

      static void clear();
    private:
      static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
      static Texture loadTextureFromFile(const char* file, bool alpha);
  };
};