#include "RigidSphere.h"

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidObj> obj2) {
    std::shared_ptr<RigidSphere> sphere =
               std::dynamic_pointer_cast<RigidSphere> (obj2);
    if (sphere != std::shared_ptr<RigidSphere>()) {
        return testCollisionWith(sphere);
    }
    else {
        CollisionInfo info;
        info.isColliding = false;
        return info;
    }
}

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidSphere> obj2) {
    
    CollisionInfo info;
    
    glm::vec3 vecVel = linearVel - obj2->linearVel;
    
    info.peneAxis = vecVel; // normalisation

    glm::vec3 vecPos = pos -  obj2->pos;
    float dist = glm::length(vecPos);
    float depth = r + obj2->r - dist;
    info.peneDepth = depth;
    if (depth > 0) {
        // Impulse-based response:
        info.isColliding = true;
    }
    else {
        info.isColliding = false;
    }
    
    return info; 

}