#ifndef __PARTICLE_SYS_H__
#define __PARTICLE_SYS_H__

#include "glm/gtc/matrix_transform.hpp"

#include "Material.h"

struct Particle
{
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec4 m_color;
    float size;
    //float m_rotation;
    //float m_angularVelocity;
    float m_age;
    
    // This variable is required, can't input data that isn't a multiple of 4
    // IF you comment this out, undefined weird stuff will happen.
    //float buffer;
};


class ParticleSystem
{
public:
    //ParticleSystem() { }  // for dealing with std::shared_ptr

    ParticleSystem(std::shared_ptr<Texture> texture);
    ~ParticleSystem();

    std::shared_ptr<Material> getMaterial();
    // update
    void tick(float dt);
    void draw();
    // TODO
    void clear() { }

    // Position of the system.
    glm::vec3 m_position;

    // Time in seconds until particles are recycled.
    float m_lifeTime = 1.f;
    
    // global acceleration applied to all particles, defaults to 0
    glm::vec3 m_acceleration = glm::vec3(0, 0, 0);

    // size of particles
    glm::vec2 m_particleSize = glm::vec2(100, 100);

private:
    // The particle system will work with a predefined pool of particles, this makes things way faster than having a dynamic list.
    // You may be able to increase this number depending on your hardware.
    // I was able to run it smoothly with 65536 particles on an NVIDIA GTX 680
    // 16348
    static const int MAX_PARTICLES = 1024;
    Particle m_particles[MAX_PARTICLES];
    float m_internalTimer = 0;

    std::shared_ptr<Material> m_particleRenderMat;
    std::shared_ptr<Material> m_particleSimulateMat;

    // for drawing particles
    GLuint vao;
    GLuint partVertBuf; // vertex buffer for particles
    GLuint quadVertBuf;


    // quad for drawing particles
    inline static const GLfloat vertQuad[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    void initBuffers();

};

#endif