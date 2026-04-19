#include "shaderProgram.h"

#include <glm/gtc/type_ptr.hpp> 

ShaderProgram::ShaderProgram()
{
    m_shaderProgram = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_shaderProgram);
}

GLuint ShaderProgram::GetGLShaderProgram()
{
    return m_shaderProgram;
}

void ShaderProgram::AttachShader(std::shared_ptr<ShaderSingle> shader)
{
    // This will point to the pointer in this shaderprogram that is the type of our passed in shader.
    
    std::shared_ptr<ShaderSingle> currentShader = std::make_shared<ShaderSingle>();

    switch (shader->getShaderType())
    {
        case GL_COMPUTE_SHADER:
            computeShader = shader;
            break;
        case GL_VERTEX_SHADER:
            vertexShader = shader;
            break;
        //case GL_GEOMETRY_SHADER:
        //    currentShader = m_geometryShader;
        //    break;
        case GL_FRAGMENT_SHADER:
            fragShader = shader;
            break;
        default:
            return;
    }
    

    // Attach the gl shader to the shader program.
    if (shader->getShaderId() != 0)
    {
       glAttachShader(m_shaderProgram, shader->getShaderId());

       // ShaderProgram must be rebuilt
       bLinked = false;
    }
    else
    {
        // Print an error if trying to attach an uninitialized shader.
        std::cout << "Failed to attach shader: Shader not initialized." << std::endl;
    }
}

void ShaderProgram::Link()
{
    if (!bLinked)
    {
        // if the program hasn't been built, build it and get uniform data
        glLinkProgram(m_shaderProgram);

        GLint linked;
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linked);

        if (linked) {
            if (nullptr != vertexShader) {
                glDetachShader(m_shaderProgram, vertexShader->getShaderId());
                glDeleteShader(vertexShader->getShaderId());
            }

            if (nullptr != fragShader) {
                glDetachShader(m_shaderProgram, fragShader->getShaderId());
                glDeleteShader(fragShader->getShaderId());
            }

            if (nullptr != computeShader) {
                glDetachShader(m_shaderProgram, computeShader->getShaderId());
                glDeleteShader(computeShader->getShaderId());
            }

        }
        else {
            // programerrors(program_id) ;
            std::cout << "Program link error" << std::endl;
            throw 4;
        }

        bLinked = true;
    }
}


// --- Uniform setters ---
void ShaderProgram::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void ShaderProgram::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void ShaderProgram::SetVec2(const std::string& name, glm::vec2 value) const
{
    glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, glm::value_ptr(value) );
}

void ShaderProgram::SetVec4(const std::string& name, glm::vec4 value) const
{
    glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, glm::value_ptr(value) );
}

void ShaderProgram::SetMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()),
                       1, GL_FALSE,  glm::value_ptr(value));
}