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

void Mesh::draw()
{

}