#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <vector>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// added in LabA07
// ==============================================
#include <assimp/material.h>

// added in LabA07
struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

// added in LabA07
struct Texture {
    GLuint id;
    std::string type;
};

// added in LabA07
struct Material {
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;
};

// ==============================================


class Mesh {

protected:
    // changed in LabA07
    // array of vertices and normals
    //std::vector< glm::vec3 > vertices; 
    std::vector<Vertex> vertices;

    // triangle vertex indices
    std::vector< unsigned int > indices;

    // added in LabA07
    std::vector<Texture> textures;

    // Material material;
    
    std::vector<GLuint> buffers;

    // this will be Material in the future
    GLuint shaderId;

    void initBuffer();

    // added in LabA07
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::string dir);
    unsigned int loadTextureAndBind(const char* path, const std::string& directory);
    
    Material Mesh::loadMaterial(aiMaterial* mat);

public:
    Mesh();
    ~Mesh();

    void init(std::string path, GLuint shaderId);
    void loadModel(std::string path);

    void setShaderId(GLuint sid);
    
    void draw(glm::mat4 matModel, glm::mat4 matView, glm::mat4 matProj);
};

#endif