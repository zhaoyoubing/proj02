#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <unordered_map>
#include <memory>
#include "ShaderProgram.h"
#include "Texture.h"


// try to move Material from traditional OpenGL
// to shader based
class Material {
public:
    // parameters for 
    // tradtional phong and blinn-phong models 
    // for compatibility with existing programs
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;

private:
    std::shared_ptr<ShaderProgram> m_shader;

    std::unordered_map<std::string, float> m_floatParams;
    std::unordered_map<std::string, glm::vec2 > m_vec2Params;
    std::unordered_map<std::string, glm::vec3 > m_vec3Params;
    std::unordered_map<std::string, glm::vec4 > m_vec4Params;
    std::unordered_map<std::string, glm::mat4 > m_mat4Params;

    std::unordered_map<std::string, std::shared_ptr<Texture> > m_textures;

public:
    // old style material for compatibility
    Material()  {}

    // future style material
    Material(std::shared_ptr<ShaderProgram> shader) : m_shader(shader) 
    { }

    // --- Shader ---
    void SetShader(std::shared_ptr<ShaderProgram> shader)
    {
        m_shader = shader;
    }

    std::shared_ptr<ShaderProgram> GetShader() const
    {
        return m_shader;
    }

    // --- Uniform Parameters ---
    void SetFloat(const std::string& name, float value)
    {
        m_floatParams[name] = value;
    }

    void SetVec2(const std::string& name, glm::vec2 v) // assume vec3[3]
    {
        m_vec2Params[name] = v;
    }

    void SetVec3(const std::string& name, glm::vec3 v) // assume vec3[3]
    {
        m_vec3Params[name] = v;
    }

    void SetVec4(const std::string& name, glm::vec4 v) // assume vec3[3]
    {
        m_vec4Params[name] = v;
    }

    void SetMat4(const std::string& name, glm::mat4 value) // assume 16 floats
    {
        //m_mat4Params[name] = std::vector<float>(value, value + 16);
    }

    // --- Textures ---
    void SetTexture(const std::string& name, std::shared_ptr<Texture> texture)
    {
        m_textures[name] = texture;
    }

    // --- Bind material before drawing ---
    void Bind()
    {
        if (!m_shader) return;

        // bind the shader
        m_shader->Use();

        // Upload floats
        for (auto& [name, value] : m_floatParams)
            m_shader->SetFloat(name, value);

        // Upload vec3s
        for (auto& [name, vec] : m_vec3Params)
            m_shader->SetVec3(name, vec);

        // Upload matrices
        for (auto& [name, mat] : m_mat4Params)
            m_shader->SetMat4(name, mat);

        // Bind textures
        int textureUnit = 0;
        for (auto& [name, tex] : m_textures)
        {
            tex->bindTexture(textureUnit);
            m_shader->SetInt(name, textureUnit);
            textureUnit++;
        }
    }


    void Unbind()
    {
        // Unbind textures
        // Unbind all owned objects.
        int textureUnit = 0;
        for (auto& [name, tex] : m_textures) 
        {
            tex->UnbindTexture(textureUnit);
            textureUnit++;
        }

        /*
        for (int i = 0; i < m_textureUniforms.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        */

        m_shader->UnUse();
    }
};


#endif



