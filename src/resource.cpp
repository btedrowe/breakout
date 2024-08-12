#include <GL/glew.h>
#include "resource.h"
#include "image.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, breakout::Texture> breakout::Resource::textures;
std::map<std::string, breakout::Shader> breakout::Resource::shaders;
std::map<std::string, float> breakout::Resource::configs;

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

void breakout::Resource::loadConf(const char *file) {
  std::ifstream confFile;
  confFile.open(file);

  if (!confFile.is_open()) {
    std::cerr << "Could not open file: " << file << '\n';
    return;
  }

  std::string line;
  while (std::getline(confFile, line)) {
    std::istringstream iss(line);
    std::string key;
    std::string valueStr;

    if (!(iss >> key >> valueStr)) {
      std::cerr << "Invalid line format in file " << file << ": " << line << '\n';
      continue;
    }

    if (!isValidString(key)) {
      std::cerr << "Invalid string format in file " << file << ": " << line << '\n';
      continue;
    }

    if (!isValidValue(valueStr)) {
      std::cerr << "Invalid value format in file " << file << ": " << line << '\n';
      continue;
    }

    configs[key] = std::stof(valueStr);
  }
  confFile.close();
}

void breakout::Resource::setConf(std::string name, unsigned int &value) {
  if (configs.count(name)) {
    if (configs[name] >= 0.0f) {
      value = static_cast<unsigned int>(configs[name]);
    }
  }
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

bool breakout::Resource::isValidString(const std::string &str) {
  return !str.empty() && \
        std::all_of(str.begin(), str.end(), [](char c) {
          return std::isalnum(c) || c == '_';
        });
}

bool breakout::Resource::isValidValue(const std::string &str) {
  std::istringstream iss(str);
  float value;

  return (iss >> value) && (iss.eof());
}
