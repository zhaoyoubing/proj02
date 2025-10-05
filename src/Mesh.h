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

public:
    void loadModel(std::string path);
    void draw();
};