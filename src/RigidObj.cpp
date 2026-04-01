#include "RigidObj.h"

#define LINEAR_DAMPING 0.9995
#define ANGULAR_DAMPING 0.999 

void RigidObj::applyLinearForce(glm::vec3 force)
{
    linearAcc += force / mass; 
}

void RigidObj::applyLinearImpulse(glm::vec3 impulse)
{
    linearVel += impulse / mass;
}

void RigidObj::integrateForces(float dt)
{
    // Integrate linear velocity.
    linearVel += linearAcc * dt;
    //std::cout << rb.linearVel.x << " " << rb.linearVel.y  << std::endl;
    
    // Integrate angular velocity
    // angularVel += angularAcc * dt;

    // Reset linear and angular acceleration.
    linearAcc = glm::vec3(0.0f, 0.0f, 0.0f);
    //angularAcc = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidObj::integrateVelocity(float dt)
{
    pos += linearVel * dt;

    if (pos.y < -20.0)
        pos.y = -20.0;

    // glm::quat velQuat(0.0f, rb.angularVel.x, rb.angularVel.y, rb.angularVel.z);
    // glm::quat dq = 0.5f * rb.trans.rotation * velQuat * dt;

    // rotation += dq; 
    // rotation = glm::normalize(rotation);

    linearVel *= LINEAR_DAMPING;
    //angularVel *= ANGULAR_DAMPING;
}
