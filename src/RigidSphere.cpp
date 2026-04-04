#include "RigidSphere.h"

glm::mat3 RigidSphere::calcInertia()
{
    float d = (2.0f / 5.0f) * mass * r * r;

    return glm::mat3(
        d, 0.0f, 0.0f,
        0.0f, d, 0.0f,
        0.0f, 0.0f, d
    );
}

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidBody> obj2) {
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
    
    return RigidBody::testCollisionWith(obj2);
}

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidSphere> obj2) {
    
    CollisionInfo info;
    
    //glm::vec3 vecVel = linearVel - obj2->linearVel;
    
    glm::vec3 vPos = pos -  obj2->pos;
    float dist = glm::length(vPos);

    // contact surface normal
    info.normal = vPos / dist; // normalisation

    // penetration depth
    float depth = r + obj2->r - dist;
    info.peneDepth = depth;

    if (depth > 0)  {
        // Impulse-based response:
        info.bColliding = true;
        info.pos = (pos + obj2->pos) * 0.5f;
    }
    else {
        info.bColliding = false;
    }
    
    return info; 

}


// for a ground, the plane is static
CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidPlane> obj2)
{
    CollisionInfo info;
    
    info.normal = obj2->normal; // normalisation

    // origin of sphere to plane distance
    float dist = obj2->pointDist(pos);
    float depth = r - dist;
    info.peneDepth = depth;

    
    // fix the above ceiling bug temporarily
    if ((depth > 0) && (depth < r)) {
        // Impulse-based response:
        info.bColliding = true;

        // contact point 
        info.pos = pos - (dist + depth * 0.5f) * obj2->normal;
    }
    else {
        info.bColliding = false;
    }
    
    return info; 
}