#ifndef __RIGID_SIM_H__
#define __RIGID_SIM_H__

#include <vector>
#include <memory>

#include "RigidObj.h"

class RigidSim 
{
public:
    std::vector<std::shared_ptr<RigidObj> > objList;

    RigidSim() {}

    void add(std::shared_ptr<RigidObj> obj) { objList.push_back(obj); }
    void tick(float dt);
};

#endif 
