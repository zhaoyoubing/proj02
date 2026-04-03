#ifndef __RIGID_SIM_H__
#define __RIGID_SIM_H__

#include <vector>
#include <memory>

#include "RigidBody.h"

class RigidSim 
{
public:
    const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.80665, 0.0f);

    const float LINEAR_DAMPING = 0.999;
    const float ANGULAR_DAMPING = 0.999;

    std::vector<std::shared_ptr<RigidBody> > objList;

    bool bPlay = false;
    bool bUseAngular = true;
    
    RigidSim() {}

    void add(std::shared_ptr<RigidBody> obj) { objList.push_back(obj); }
    
    void tick(float dt);

    void draw();

    void collisionResponse(std::shared_ptr<RigidBody> a, std::shared_ptr<RigidBody> b, CollisionInfo collisionInfo);

    void setUseAngular(bool b) {
        bUseAngular = b;
    }

    void setPlaySim(bool b) {
        bPlay = b;
    }

    void clear() {
        objList.clear();
        bPlay = false;
    }
};

#endif 
