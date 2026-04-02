#ifndef __RIGID_OBJ_H__
#define __RIGID_OBJ_H__


#include <vector>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"

struct CollisionInfo {
    bool isColliding;
    glm::vec3 normal;          // Minimum Penetration Axis
    float peneDepth; // How deep a rigid body is inside another
};

class RigidObj
{
public:
    bool dynamic = true;
    bool useGravity = true;
    const float elasity = 0.98f;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // rotation
    glm::vec3 scale  = glm::vec3(1.0f, 1.0f, 1.0f);


    float mass = 1.0;
    glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearVel = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearAcc = glm::vec3(0.0f, 0.0f, 0.0f);

    // angular velocity and acceleration
    // To be introduced in 2027
    // glm::vec3 angularVel;
    // glm::vec3 angularAcc;

    std::shared_ptr<Mesh> mesh;

    void applyLinearForce(glm::vec3 force);
    void applyLinearImpulse(glm::vec3 impulse);
    void integrateForces(float dt);
    void integrateVelocity(float dt);


    void setDynamic(bool b) {
        dynamic = b;
    }

    void setPosition(glm::vec3 apos) { 
        pos = apos; 
    }

    void setVelocity(glm::vec3 vel) {
        linearVel = vel;
    }

    void setUseGravity(bool b) { useGravity = b;  }


    void setMesh(std::shared_ptr<Mesh> m) { mesh = m; }

    void setMass(float m) { mass = m; }

    void draw(glm::mat4 matView, glm::mat4 matProj) {
        glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), pos);
        //glm::mat4 matRot    = glm::mat4_cast(rot);
        glm::mat4 matScale  = glm::scale(glm::mat4(1.0f), scale);

        glm::mat4 modelTrans = matTrans * matScale;

        mesh->draw(modelTrans, matView, matProj);
    }

    virtual CollisionInfo testCollisionWith(std::shared_ptr<RigidObj> obj) 
    {
        CollisionInfo info;
        info.isColliding = false;
        return info;
    }

};

#endif