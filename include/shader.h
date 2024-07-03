#pragma once

#include <string>

#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace breakout {
  class Shader {
    public:
      Shader();
      Shader& use();

      void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
      void setFloat(const char* name, float value, bool useShader = false);
      void setInteger(const char* name, int value, bool useShader = false);
      void setVector2f(const char* name, float x, float y, bool useShader = false);
      void setVector2f(const char* name, const glm::vec2& value, bool useShader = false);
      void setVector3f(const char* name, float x, float y, float z, bool useShader = false);
      void setVector3f(const char* name, const glm::vec3& value, bool useShader = false);
      void setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
      void setVector4f(const char* name, const glm::vec4& value, bool useShader = false);
      void setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

      unsigned int ID;
    private:
      void checkCompileErrors(unsigned int object, std::string type);
  };
};