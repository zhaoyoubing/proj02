#include <glad/glad.h>

#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void Mesh::init(std::string path, GLuint id)
{
    shaderId = id;
    loadModel(path);
    initBuffer();
}

void Mesh::loadModel(std::string path) 
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices);
    if (NULL != scene) {
        std::cout << "load model successful" << std::endl;
    } else {
        std::cout << "load model failed" << std::endl;
    }

    // mNumMeshes should > 0
    // std::cout << scene->mNumMeshes << std::endl;

    // at the moment we only handle one mesh
    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        
        // read vertex position and normals
        int nVertex = mesh->mNumVertices;
        // std::cout << mesh->mNumVertices << std::endl;
        for (int j = 0; j < nVertex; j++)
        {
            glm::vec3 pos; 
            pos.x = mesh->mVertices[j].x;
            pos.y = mesh->mVertices[j].y;
            pos.z = mesh->mVertices[j].z; 
            vertices.push_back(pos);

            glm::vec3 normal;
            normal.x = mesh->mNormals[j].x;
            normal.y = mesh->mNormals[j].y;
            normal.z = mesh->mNormals[j].z;
            vertices.push_back(normal);

        }

		int nFaces = mesh->mNumFaces;
        for (int j = 0; j < nFaces; j++ )
        {
            const aiFace& face = mesh->mFaces[j];
            for (int k = 0; k < 3; k++)
            {
                indices.push_back(face.mIndices[k]);
            }
        }
    }

    // for debugging
    // std::cout << vertices.size() << std::endl;
    // std::cout << indices.size() << std::endl;
}

void Mesh::initBuffer()
{
   // create vertex buffer
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);
    
    buffers.push_back(vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

    // set normal attributes
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (sizeof(float) * 3));

    // create index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    buffers.push_back(idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// all drawings come here
void Mesh::draw(glm::mat4 mat)
{
    glUseProgram(shaderId);

    // set transforms
    // identity
    glm::mat4 mat_modelview =   glm::mat4(1.0);
    
    // default orthographic projection
    glm::mat4 mat_projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    GLuint modelview_loc = glGetUniformLocation(shaderId, "modelview" );
    glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, &mat_modelview[0][0]);

    // you must set the orthographic projection to get correct rendering with depth
    GLuint projection_loc = glGetUniformLocation( shaderId, "projection" );
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &mat_projection[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}