#include "RigidSim.h"
#include "AppMain.h"

extern App app;

// simulation update workflow for each time step
void RigidSim::tick(float dt)
{
    // dynamics
    for (auto& obj : objList) {
        // f = ma
        if (obj->useGravity) {
            obj->applyLinearForce(GRAVITY);
            obj->integrateForces(dt);
        }
    }

    // collision detection and collision response
    for (int i = 0; i < objList.size(); i++)
        for (int j = i + 1; j < objList.size(); j++)
    {
        std::shared_ptr<RigidObj> obj1 = objList[i];
        std::shared_ptr<RigidObj> obj2 = objList[j];

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
    glm::vec3 normal = collisionInfo.normal;
    float peneDepth = collisionInfo.peneDepth;
    glm::vec3 offsetPos = normal * peneDepth * 0.5f;

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

    float denom = (1.0f / a->mass) + (1.0f / b->mass);

    if (! b->dynamic) {
        denom = (1.0f / a->mass);
        offsetPos = offsetPos * 2.0f;
    }
    else {
        b->pos -= offsetPos;
    }

    a->pos += offsetPos;

    //if (a->pos.y < b->pos.y + 1)
    //    std::cout << "below the plane" << std::endl;

    float J = -(1 + a->elasity) * glm::dot(relativeVel, normal) / denom;

    // Impulse is given by j * n 
    glm::vec3 impulse = J * normal;

    a->applyLinearImpulse(impulse);
    b->applyLinearImpulse(-impulse);

    // code for rotation
    //a->applyAngularImpulse(impulse, rA);
    //b->applyAngularImpulse(-impulse, rB);
}
