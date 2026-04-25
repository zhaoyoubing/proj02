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

            // TODO : apply gravity force
            // using RigidBody::applyLinearForce() and the vector RigidSim::GRAVITY
            obj->applyLinearForce(obj->mass * GRAVITY);
        }

        // TODO: Integrate acceleration 
        // to update object velocity
        obj->integrateAcc(dt);
    }

    // collision detection and collision response
    for (int i = 0; i < objList.size(); i++)
        for (int j = i + 1; j < objList.size(); j++)
    {
        std::shared_ptr<RigidBody> obj1 = objList[i];
        std::shared_ptr<RigidBody> obj2 = objList[j];

        // TODO: call RigidBody::testCollisionWith()
        // to check if two rigid body objects collides
        // and returns CollisionInfo
        CollisionInfo info = obj1->testCollisionWith(obj2);

        if (info.bColliding) {
            // TODO: call collisionResponse(RigidBody, RigidBody, CollisionInfo)
            // to achieve impulse based collision effects
            collisionResponse(obj1, obj2, info);
        }
    }

    // integrate velocity and draw
    for (auto& obj : objList) {
        // call RigidBody::integrateVelocity() to 
        // update object position
        obj->integrateVelocity(dt);
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

    // half of the penetration vector
    glm::vec3 offsetPos = normal * peneDepth * 0.5f;

    // magnitute of the impulse
    float J = 0.0;

    // for angular effects
    float angularA = 0.0f;
    float angularB = 0.0f;
    glm::vec3 rA = glm::vec3(0.0f);
    glm::vec3 rB = glm::vec3(0.0f);

    // the denominator of the impulse formula
    // to avoid divide by zero
    float denom = 0.000001f;

    // angular effects
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

        // the full formula with angular rotations
        /*
        float j = -(1 + e) * glm::dot(relativeVel, peneAxis) /
        (
            (1.0f / a->mass) +
            (1.0f / b->mass)

            // the following are for anular effects
            + glm::dot(glm::cross(rA, mpa), invInertiaA * glm::cross(rA, normal))
            + glm::dot(glm::cross(rB, mpa), invInertiaB * glm::cross(rB, normal))
        );
        */

        // always think A is dynamic
        if (b->bDynamic) {
            denom = (1.0f / a->mass) + (1.0f / b->mass) + angularA + angularB;
            a->pos += offsetPos;
            b->pos -= offsetPos;
        } else {
            // ignore B if it is static
            denom = (1.0f / a->mass) + angularA;
            a->pos += offsetPos * 2.0f;
        }

    } else {
        // the full formula without angular rotations
        /*
        float j = -(1 + e) * glm::dot(relativeVel, peneAxis) /
        (
            (1.0f / a->mass) + (1.0f / b->mass)
        );
        */
        if (b->bDynamic) {
            // TODO: calculate the full dominator as
            // (1.0f / a->mass) + (1.0f / b->mass)
            denom = (1.0f / a->mass) + (1.0f / b->mass);
            // denom = ???;


            // push back objects outside, simple but not accurate 
            a->pos += offsetPos;
            b->pos -= offsetPos;
        }
        else {
            // ignore B if it is static, so only include A
            denom = 1.0f / a->mass;
            // push back objects outside, simple but not accurate 
            a->pos += offsetPos * 2.0f;
        }

    }

    // Use relative velocity of both bodies to find the impulse needed
    // to push them apart. It's the reaction of the collision.
    glm::vec3 relativeVel = a->linearVel - b->linearVel;

    // TODO: calculate magnitude of the impulse J
    // using a->elastiy, relativeVel, normal and denom
    J = -(1 + a->elasity) * glm::dot(relativeVel, normal) / denom;
    // J = ???
    
    // the impulse vector has the direction of the normal 
    glm::vec3 impulse = J * normal;

    // apply linear impulse
    a->applyLinearImpulse(impulse);
    b->applyLinearImpulse(-impulse);

    // code for angular effects
    if (bUseAngular) {
        a->applyAngularImpulse(impulse, rA);
        b->applyAngularImpulse(-impulse, rB);
    }
}
