#ifndef __RIGID_SIM_H__
#define __RIGID_SIM_H__

#include <vector>
#include <memory>

#include "RigidObj.h"

class RigidSim 
{
public:
    const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.80665, 0.0f);
    const float COEF_ELASITY = 0.78f;

    std::vector<std::shared_ptr<RigidObj> > objList;
    
    RigidSim() {}

    void add(std::shared_ptr<RigidObj> obj) { objList.push_back(obj); }
    
    void tick(float dt);

    void draw();

    void collisionResponse(std::shared_ptr<RigidObj> a, std::shared_ptr<RigidObj> b, CollisionInfo collisionInfo);
};

#endif 
