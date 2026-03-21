#ifndef __MESH_FACTORY_H__
#define __MESH_FACTORY_H__

#include <memory>
#include <assimp/material.h>

#include "Mesh.h"
#include "Material.h"


//#include "ClothSimMesh"

class MeshFactory {
public:
    static std::shared_ptr<Mesh> importAssimp(std::string path); 
    //static std::shared_ptr<ClothSimMesh> genClothSimMesh();
private:
    static Material MeshFactory::loadAssimpMaterial(aiMaterial* mat);
    static void loadAssimpMaterialTextures(std::shared_ptr<Mesh> pMesh, aiMaterial *mat, aiTextureType type, std::string typeName, std::string dir);

};

#endif