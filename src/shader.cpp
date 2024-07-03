#include <GL/glew.h>
#include "shader.h"

#include <iostream>
#include <SDL2/SDL.h>


breakout::Shader::Shader() {}

breakout::Shader &breakout::Shader::use()
{
  glUseProgram(ID);
  return *this;
}

void breakout::Shader::compile(const char *vertexSource,
                               const char *fragmentSource,
                               const char *geometrySource) {
  unsigned int sVertex, sFragment, gShader;

  sVertex = glCreateShader(GL_VERTEX_SHADER);  //vertex
  glShaderSource(sVertex, 1, &vertexSource, NULL);
  glCompileShader(sVertex);
  checkCompileErrors(sVertex, "VERTEX");

  sFragment = glCreateShader(GL_FRAGMENT_SHADER);  //fragment
  glShaderSource(sFragment, 1, &fragmentSource, NULL);
  glCompileShader(sFragment);
  checkCompileErrors(sFragment, "FRAGMENT");

  if (geometrySource != nullptr) {  //geometry
    gShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gShader, 1, &geometrySource, NULL);
    glCompileShader(gShader);
    checkCompileErrors(gShader, "GEOMETRY");
  }

  ID = glCreateProgram();  //compile program
  glAttachShader(ID, sVertex);
  glAttachShader(ID, sFragment);
  if (geometrySource != nullptr) {
    glAttachShader(ID, gShader);
  }
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  glDeleteShader(sVertex);  //cleanup
  glDeleteShader(sFragment);
  if (geometrySource != nullptr) {
    glDeleteShader(gShader);
  }
}

void breakout::Shader::setFloat(const char* name, float value, bool useShader) {
  if (useShader)
    use();
  glUniform1f(glGetUniformLocation(ID, name), value);
}

void breakout::Shader::setInteger(const char* name, int value, bool useShader) {
  if (useShader)
    use();
  glUniform1i(glGetUniformLocation(ID, name), value);
}

void breakout::Shader::setVector2f(const char* name, float x, float y, bool useShader) {
  if (useShader)
    use();
  glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void breakout::Shader::setVector2f(const char* name, const glm::vec2& value, bool useShader) {
  if (useShader)
    use();
  glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}

void breakout::Shader::setVector3f(const char* name, float x, float y, float z,
                         bool useShader) {
  if (useShader)
    use();
  glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void breakout::Shader::setVector3f(const char* name, const glm::vec3& value,
                         bool useShader) {
  if (useShader)
    use();
  glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void breakout::Shader::setVector4f(const char* name, float x, float y, float z, float w,
                         bool useShader) {
  if (useShader)
    use();
  glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}

void breakout::Shader::setVector4f(const char* name, const glm::vec4& value,
                         bool useShader) {
  if (useShader)
    use();
  glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z,
              value.w);
}

void breakout::Shader::setMatrix4(const char* name, const glm::mat4& matrix, bool useShader) {
  if (useShader)
    use();
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}

void breakout::Shader::checkCompileErrors(unsigned int object, std::string type) {
  int success;
  char infoLog[1024];

  if (type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      std::cout
        << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n" \
        << infoLog \
        << "\n  -- --------------------------------------------------- -- " \
        << std::endl;
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      std::cout
        << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n" \
        << infoLog \
        << "\n  -- --------------------------------------------------- -- " \
        << std::endl;
    }
  }
}
