#include "RigidSim.h"
#include "AppMain.h"

extern App app;

// simulation update workflow for each time step
void RigidSim::tick(float dt)
{
    if (!bPlay) return;

    // dynamics
    for (auto& obj : objList) {
        // f = ma
        if (obj->bUseGravity) {
            obj->applyLinearForce(GRAVITY);
        }
        obj->integrateAcc(dt);
    }

    // collision detection and collision response
    for (int i = 0; i < objList.size(); i++)
        for (int j = i + 1; j < objList.size(); j++)
    {
        std::shared_ptr<RigidBody> obj1 = objList[i];
        std::shared_ptr<RigidBody> obj2 = objList[j];

        CollisionInfo info = obj1->testCollisionWith(obj2);

        if (info.bColliding) {
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
void RigidSim::collisionResponse(std::shared_ptr<RigidBody> a, std::shared_ptr<RigidBody> b, CollisionInfo info) 
{
    glm::vec3 normal = info.normal;
    float peneDepth = info.peneDepth;
    glm::vec3 offsetPos = normal * peneDepth * 0.5f;

    float J = 0.0;

    float angularA = 0.0f;
    float angularB = 0.0f;
    glm::vec3 rA = glm::vec3(0.0f);
    glm::vec3 rB = glm::vec3(0.0f);

    if (bUseAngular)
    {
        // find the contact point
        glm::vec3 contactPointA = info.pos + peneDepth * normal * 0.5f;
        glm::vec3 contactPointB = info.pos - peneDepth * normal * 0.5f;

        rA = contactPointA - a->pos;
        rB = contactPointB - b->pos;

        // Velocity of rigid body A
        glm::vec3 vA = a->linearVel + (glm::cross(a->angularVel, rA));

        // Velocity of rigid body B
        glm::vec3 vB = b->linearVel + (glm::cross(b->angularVel, rB));

        glm::mat3 invInertiaA = glm::inverse(a->matInertia);
        glm::mat3 invInertiaB = glm::inverse(b->matInertia);

        angularA = glm::dot(glm::cross(rA, normal), invInertiaA * glm::cross(rA, normal));
        angularB = glm::dot(glm::cross(rB, normal), invInertiaA * glm::cross(rA, normal));

    }

    float denomA = (1.0f / a->mass) + angularA;
    float denomB = (1.0f / b->mass) + angularB;

    float denom = denomA;

    // the full formula with angular rotations
    /*
    float j = -(1 + e) * glm::dot(relativeVel, peneAxis) /
    (
        (1.0f / a->mass) +
        (1.0f / b->mass) +
        glm::dot(glm::cross(rA, mpa), invInertiaA * glm::cross(rA, normal)) +
        glm::dot(glm::cross(rB, mpa), invInertiaB * glm::cross(rB, normal))
    );
    */

    if (b->bDynamic) {
        denom += denomB;
        b->pos -= offsetPos;
    }
    else {
        offsetPos = offsetPos * 2.0f;
    }

    // simple but not accurate handling 
    a->pos += offsetPos;

    // We use relative velocity of both bodies to find the impulse needed
    // to push them apart. It's the reaction of the collision.
    glm::vec3 relativeVel = a->linearVel - b->linearVel;

    // magnitude of the impulse
    J = -(1 + a->elasity) * glm::dot(relativeVel, normal) / denom;

    // the impulse vector has the direction of the normal 
    glm::vec3 impulse = J * normal;

    a->applyLinearImpulse(impulse);
    b->applyLinearImpulse(-impulse);

    // code for rotation
    if (bUseAngular) {
        a->applyAngularImpulse(impulse, rA);
        b->applyAngularImpulse(-impulse, rB);
    }
}
