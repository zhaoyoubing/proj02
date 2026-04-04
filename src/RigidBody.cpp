#include "RigidBody.h"

#define LINEAR_DAMPING 0.9999
#define ANGULAR_DAMPING 0.999 

void RigidBody::applyLinearForce(glm::vec3 force)
{
    if (! bDynamic) return;

    linearAcc += force / mass; 
}

void RigidBody::applyLinearImpulse(glm::vec3 impulse)
{
    if (! bDynamic) return;
    linearVel += impulse / mass;
}


void RigidBody::applyAngularForce(glm::vec3 f, glm::vec3 r) {
    if (! bDynamic) return;

    glm::vec3 t = glm::cross(r, f);
    angularAcc += glm::inverse(matInertia) * t; 
}

void RigidBody::applyAngularImpulse(glm::vec3 i, glm::vec3 r) {
    if (! bDynamic) return;

    glm::vec3 torque = glm::cross(r, i);
    angularVel += glm::inverse(matInertia) * torque;
}

void RigidBody::integrateAcc(float dt)
{
    integrateLinearAcc(dt);
    integrateAngularAcc(dt);
}
    
void RigidBody::integrateLinearAcc(float dt)
{
    if (! bDynamic) return;

    // Integrate linear velocity.
    linearVel += linearAcc * dt;
    //std::cout << rb.linearVel.x << " " << rb.linearVel.y  << std::endl;
    
    // Integrate angular velocity
    // angularVel += angularAcc * dt;

    // Reset linear and angular acceleration.
    linearAcc = glm::vec3(0.0f, 0.0f, 0.0f);
    //angularAcc = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::integrateAngularAcc(float dt)
{
    // Integrate angular velocity
    angularVel += angularAcc * dt;

    // Reset linear and angular acceleration.
    angularAcc = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::integrateVelocity(float dt)
{
    if (!bDynamic) return;

    integrateLinearVelocity(dt);
    integrateAngularVelocity(dt);
}

void RigidBody::integrateLinearVelocity(float dt)
{
    if (! bDynamic) return;

    pos += linearVel * dt;

    linearVel *= LINEAR_DAMPING;
}

void RigidBody::integrateAngularVelocity(float dt)
{
    glm::quat velQuat(0.0f, angularVel.x, angularVel.y, angularVel.z);
    glm::quat dq = 0.5f * rotation * velQuat * dt;

    rotation += dq; 
    rotation = glm::normalize(rotation);

    angularVel *= ANGULAR_DAMPING;
}
