#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include <memory>
#include "RigidObj.h"
#include "SphereMesh.h"

class RigidSphere : public RigidObj {

public:
    float r = 1.0f;
    //std::shared_ptr<SphereMesh> mesh;
    RigidSphere() { r = 1.0f; }
    RigidSphere(float ar) { r = ar; }

    CollisionInfo testCollisionWith(std::shared_ptr<RigidObj>  obj2);
    CollisionInfo testCollisionWith(std::shared_ptr<RigidSphere> obj2);
};

#endif