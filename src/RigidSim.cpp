#include "RigidSim.h"
#include "AppMain.h"

extern App app;

// simulation update workflow for each time step
void RigidSim::tick(float dt)
{
    // dynamics
    for (auto& obj : objList) {
        // f = ma
        obj->applyLinearForce(GRAVITY);
        obj->integrateForces(dt);
    }

    // collision detection and collision response
    for (auto& obj1 : objList) 
        for (auto& obj2 : objList) {

            if (obj1 == obj2)
                continue;

            CollisionInfo info = obj1->testCollisionWith(obj2);

            if (info.isColliding) {
                collisionResponse(obj1, obj2, info);
            }
    }

    // integrate velocity and draw
    for (auto& obj : objList) {
        obj->integrateVelocity(dt);
        //obj->draw(app.camera->matView, app.camera->matProj);
    }

}

void RigidSim::draw()
{
    for (auto& obj : objList) {
        obj->draw(app.camera->matView, app.camera->matProj);
    }
}

// calculate impulse and modify object velocity
void RigidSim::collisionResponse(std::shared_ptr<RigidObj> a, std::shared_ptr<RigidObj> b, CollisionInfo collisionInfo) 
{
    glm::vec3 peneAxis = collisionInfo.peneAxis;
    float penetrationDepth = collisionInfo.peneDepth;

    /*
    // code for rotation
    glm::vec3 contactPointA = findContactPoint(a, peneAxis, penetrationDepth);
    glm::vec3 contactPointB = findContactPoint(b, -peneAxis, penetrationDepth);

    glm::vec3 rA = contactPointA - a->pos;
    glm::vec3 rB = contactPointB - b->pos;

    // Velocity of rigid body A
    glm::vec3 vA = a->linearVel + (glm::cross(a.angularVel, rA));

    // Velocity of rigid body B
    glm::vec3 vB = b->linearVel + (glm::cross(b.angularVel, rB));

    glm::mat3 invertedInertiaTensorA = glm::inverse(a.inertiaTensor);
    glm::mat3 invertedInertiaTensorB = glm::inverse(b.inertiaTensor);
    
    float j = -(1 + RESTITUTION_CO) * glm::dot(relativeVel, peneAxis) /
    (
        (1.0f / a->mass) +
        (1.0f / b->mass) +
        glm::dot(glm::cross(rA, mpa), invertedInertiaTensorA * glm::cross(rA, peneAxis)) +
        glm::dot(glm::cross(rB, mpa), invertedInertiaTensorB * glm::cross(rB, peneAxis))
    );

    */

    // We use relative velocity of both bodies to find the impulse needed
    // to push them apart. It's the reaction of the collision.
    glm::vec3 relativeVel = a->linearVel - b->linearVel;

    float J = -(1 + COEF_ELASITY) * glm::dot(relativeVel, peneAxis) /
        ( (1.0f / a->mass) + (1.0f / b->mass) );

    // Impulse is given by j * n (Minimum Penetration Axis)
    glm::vec3 impulse = J * peneAxis;

    a->applyLinearImpulse(impulse);
    b->applyLinearImpulse(-impulse);

    // code for rotation
    // a->applyImpulse(impulse, rA);
    // b->applyImpulse(-impulse, rB);
}