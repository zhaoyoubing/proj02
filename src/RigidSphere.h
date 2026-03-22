#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include <memory>
#include "RigidObj.h"
#include "SphereMesh.h"

class RigidSphere : public RigidObj {

public:
    std::shared_ptr<SphereMesh> mesh;
    RigidSphere() { }
};

#endif