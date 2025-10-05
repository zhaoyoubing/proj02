#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void Mesh::loadModel(std::string path) 
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate);
    if (NULL != scene) {
        std::cout << "load model successful" << std::endl;
    } else {
        std::cout << "load model failed" << std::endl;
    }

    // mNumMeshes should > 0
    //std::cout << scene->mNumMeshes << std::endl;

    // at the moment we only handle one mesh
    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        
        // read vertex position and normals
        int nVertex = mesh->mNumVertices;
        for (int j = 0; j < nVertex; j++)
        {
            glm::vec3 pos; 
            pos.x = mesh->mVertices[i].x;
            pos.y = mesh->mVertices[i].y;
            pos.z = mesh->mVertices[i].z; 
            vertices.push_back(pos);

            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertices.push_back(normal);
        }

		int nFaces = mesh->mNumFaces;
        for (int j = 0; j < nFaces; j++ )
        {
            const aiFace& face = mesh->mFaces[j];
            for (int k = 0; k < 3; k++)
            {
                indices.push_back(face.mIndices[j]);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

}

void Mesh::draw()
{

}