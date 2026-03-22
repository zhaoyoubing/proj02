#ifndef __RIGID_OBJ_H__
#define __RIGID_OBJ_H__


#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


class RigidObj
{
    glm::vec3 pos;

    float mass;
    glm::vec3 force;
    glm::vec3 velocity;
    glm::vec3 acceler;
};

class RigidSphere : public RigidObj 
{
public:
    // using pos as the sphere center
    float r;

    bool ifIntersect(RigidSphere & sphere);
};

#endif