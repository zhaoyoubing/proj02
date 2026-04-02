#include "RigidSphere.h"

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidObj> obj2) {
    std::shared_ptr<RigidSphere> sphere =
               std::dynamic_pointer_cast<RigidSphere> (obj2);
    if (sphere != std::shared_ptr<RigidSphere>()) {
        return testCollisionWith(sphere);
    }

    std::shared_ptr<RigidPlane> plane =
        std::dynamic_pointer_cast<RigidPlane> (obj2);
    if (plane != std::shared_ptr<RigidPlane>()) {
        return testCollisionWith(plane);
    }
    
    CollisionInfo info;
    info.isColliding = false;
    return info;

    //return RigidObj::testCollisionWith(obj2);
}

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidSphere> obj2) {
    
    CollisionInfo info;
    
    glm::vec3 vecVel = linearVel - obj2->linearVel;
    
    info.normal = glm::normalize(vecVel); // normalisation

    glm::vec3 vecPos = pos -  obj2->pos;
    float dist = glm::length(vecPos);

    // penetration depth
    float depth = r + obj2->r - dist;
    info.peneDepth = depth;

    if (depth > 0)  {
        // Impulse-based response:
        info.isColliding = true;
    }
    else {
        info.isColliding = false;
    }
    
    return info; 

}


// for a ground, the plane is static
CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidPlane> obj2)
{
    CollisionInfo info;
    
    // relative velocity
    //glm::vec3 vecVel = linearVel - obj2->linearVel;
    
    info.normal = obj2->normal; // normalisation

    glm::vec3 vecPos = pos -  obj2->pos;

    // origin of sphere to plane distance
    float dist = obj2->pointDist(pos);
    float depth = r - dist;
    info.peneDepth = depth;

    
    // fix the above ceiling bug temporarily
    if ((depth > 0) && (depth < r)) {
        // Impulse-based response:
        info.isColliding = true;
    }
    else {
        info.isColliding = false;
    }
    
    return info; 
}