#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <vector>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Mesh {

protected:
    // array of vertices and normals
    std::vector< glm::vec3 > vertices; 

    // triangle vertex indices
    std::vector< unsigned int > indices;
    
    std::vector<GLuint> buffers;

    void initBuffer();

    // this will be Material in the future
    GLuint shaderId;

public:
    Mesh();
    ~Mesh();

    void init(std::string path, GLuint shaderId);
    void loadModel(std::string path);

    void setShaderId(GLuint sid);
    
    void draw(glm::mat4 matModel, glm::mat4 matView, glm::mat4 matProj);
};

#endif