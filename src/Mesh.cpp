#include <iostream>

#include <glad/glad.h>

#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

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

            // std::cout << pos.x << pos.y << pos.z << std::endl;

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

                //std::cout << face.mIndices[k] << std::endl;
            }
        }
    }

    // for debugging
    std::cout << "numVertex: " <<  vertices.size() << std::endl;
    std::cout << "numIdex: " << indices.size() << std::endl;
}

void Mesh::initBuffer()
{
    // create vertex buffer
    GLuint vao;
    glGenVertexArrays(1, &vao);
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    
    // remember VAO
    glBindVertexArray(vao);
    buffers.push_back(vao);
    
    std::cout << "vertBufId: " << vertBufID << std::endl;

    buffers.push_back(vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

    // set normal attributes
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (sizeof(float) * 3));

    // bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    std::cout << "idxBufId: " << idxBufID << std::endl;
    buffers.push_back(idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::setShaderId(GLuint sid) {
    shaderId = sid;
}

// all drawings come here
void Mesh::draw(glm::mat4 matModel, glm::mat4 matView, glm::mat4 matProj)
{
    // 1. Bind the correct shader program
    glUseProgram(shaderId);

    //std::cout << "shader: " << shaderId << std::endl;

    // 2. Set the appropriate uniforms for each shader
    // set model view transforms
    //glm::mat4 mat_modelview = matView * matModel;
    
    GLuint model_loc = glGetUniformLocation(shaderId, "model" );
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, &matModel[0][0]);

    // set view matrix
    GLuint view_loc = glGetUniformLocation(shaderId, "view" );
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &matView[0][0]);

    // set projection transforms
    // glm::mat4 mat_projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
    glm::mat4 mat_projection = matProj;
    GLuint projection_loc = glGetUniformLocation( shaderId, "projection" );
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &mat_projection[0][0]);

    // 3. Bind the corresponding model's VAO
    glBindVertexArray(buffers[0]);

    // 4. Draw the model
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}