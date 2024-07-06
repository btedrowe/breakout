#include <GL/glew.h>
#include "resource.h"
#include "image.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, breakout::Texture> breakout::Resource::textures;
std::map<std::string, breakout::Shader> breakout::Resource::shaders;

breakout::Shader breakout::Resource::loadShader(const char *vShaderFile,
                                                const char *fShaderFile, 
                                                const char *gShaderFile, 
                                                std::string name) {
  shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  return shaders[name];
}

breakout::Shader breakout::Resource::getShader(std::string name) {
  return shaders[name];
}

breakout::Texture breakout::Resource::loadTexture(const char *file, bool alpha, std::string name) {
  textures[name] = loadTextureFromFile(file, alpha);
  return textures[name];
}

breakout::Texture breakout::Resource::getTexture(std::string name) {
  return textures[name];
}

void breakout::Resource::clear() {
  for (const auto& shader : shaders) {
    glDeleteProgram(shader.second.ID);
  }
  for (const auto& texture : textures) {
    glDeleteTextures(1, &texture.second.ID);
  }
}

breakout::Shader breakout::Resource::loadShaderFromFile(const char *vShaderFile,
                                              const char *fShaderFile,
                                              const char *gShaderFile) {
  std::string vertexCode, fragmentCode, geometryCode;

  try {
    std::ifstream vertexShaderFile(vShaderFile), fragmentShaderFile(fShaderFile);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    if (gShaderFile != nullptr) {
      std::ifstream geometryShaderFile(gShaderFile);
      std::stringstream gShaderStream;
      gShaderStream << geometryShaderFile.rdbuf();
      geometryCode = gShaderStream.str();
    }
  } catch (const std::ifstream::failure& e) {
    throw std::runtime_error("Failed to read shader file: " + std::string(e.what()));
  }

  breakout::Shader shader;
  shader.compile(vertexCode.c_str(), fragmentCode.c_str(),
                 gShaderFile != nullptr ? geometryCode.c_str() : nullptr);

  return shader;
}

breakout::Texture breakout::Resource::loadTextureFromFile(const char *file, bool alpha) {
  breakout::Texture texture;

  if (alpha) {
    texture.internalFormat = GL_RGBA;
    texture.imageFormat = GL_RGBA;
  }

  int width, height, nChannels;
  unsigned char* data;

  if (!loadPNG(file, &data, &width, &height, &nChannels)) {
    std::cerr << "Failed to load file: " << file << std::endl;
  }

  texture.generate(width, height, data);

  delete[] data;

  return texture;
}
