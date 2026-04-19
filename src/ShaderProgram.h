#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include "ShaderSingle.h"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Wraps opengl shader program functionality
class ShaderProgram
{

private:
    // These shader objects wrap the functionality of loading and compiling shaders from files.
    std::shared_ptr<ShaderSingle> computeShader = nullptr;
    std::shared_ptr<ShaderSingle> vertexShader = nullptr;
    //Shader* m_geometryShader = nullptr;
    std::shared_ptr<ShaderSingle> fragShader = nullptr;

    // GL index for shader program
    GLuint m_shaderProgram;

    // Keep track of if the program has been built and only build when needed
    // bool m_programBuilt = false;

    bool bLinked = false;


public:
    ShaderProgram();
    ~ShaderProgram();
    GLuint GetGLShaderProgram();
    void AttachShader(std::shared_ptr<ShaderSingle> shader);
    
    void Link();

    void Use() const {   glUseProgram(m_shaderProgram); }
    void UnUse() { glUseProgram(0); }

    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    void SetVec2(const std::string& name, glm::vec2 value) const;
    void SetVec3(const std::string& name, glm::vec3 value) const;
    void SetVec4(const std::string& name, glm::vec4 value) const;
    void SetMat4(const std::string& name, glm::mat4 value) const;
};

#endif