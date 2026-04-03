#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include <memory>
#include "RigidBody.h"
#include "SphereMesh.h"
#include "RigidPlane.h"

class RigidSphere : public RigidBody {

public:
    float r = 1.0f;
    //std::shared_ptr<SphereMesh> mesh;
    RigidSphere() { init(1.0f); }
    RigidSphere(float ar) { init(ar); }

    void init(float ar) {
        r = ar;
        matInertia = calcInertia();
    }

    glm::mat3 calcInertia() override;
    CollisionInfo testCollisionWith(std::shared_ptr<RigidBody>  obj2);
    
    CollisionInfo testCollisionWith(std::shared_ptr<RigidSphere> obj2);
    CollisionInfo testCollisionWith(std::shared_ptr<RigidPlane> obj2);
};

#endif