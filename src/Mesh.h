#include <iostream>
#include <vector>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Mesh {

protected:
    std::vector< glm::vec3 > vertices; // array of vertices
    std::vector< glm::vec3 > normals;  // // array of normals

    // triangle vertex indices
    std::vector< unsigned int > vertex_indices;
    // triangle normal indices
    std::vector< unsigned int > normal_indices;
    
    std::vector<GLuint> buffers;

public:
    void loadModel(std::string path);
    void draw();
};