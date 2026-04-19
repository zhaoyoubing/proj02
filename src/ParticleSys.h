#ifndef __PARTICLE_SYS_H__
#define __PARTICLE_SYS_H__

#include "glm/gtc/matrix_transform.hpp"

#include "Material.h"

struct Particle
{
    glm::vec4 pos;
    glm::vec4 velocity;
    glm::vec4 color;

    float life;
    float maxLife;

    //float rotation;
    //float angularVelocity;

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
    glm::vec3 pos;

    // Time in seconds until particles are recycled.
    float maxLife = 1.f;
    
    // global acceleration applied to all particles, defaults to 0
    glm::vec3 acc = glm::vec3(0, 0, 0);

    // size of particles
    glm::vec2 partSize = glm::vec2(100, 100);

private:
    // The particle system will work with a predefined pool of particles, this makes things way faster than having a dynamic list.
    // You may be able to increase this number depending on your hardware.
    // I was able to run it smoothly with 65536 particles on an NVIDIA GTX 680
    // 16348
    static const int NUM_POINTS = 64;
    Particle particles[NUM_POINTS];
    float internalTimer = 0;

    // particle vertex and fragment shaders
    std::shared_ptr<Material> partDrawMat;
    // simulation shaders
    std::shared_ptr<Material> partSimMat;

    // quad for drawing particles
    inline static const GLfloat vertQuad[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    bool bPoints = true;

    // for drawing particles
    GLuint vao;  // vertex array object
    GLuint quadVertBuf; // the quad proxy for particle textures
    GLuint partVertBuf; // vertex buffer for particles

    void initBuffers();
    void initBufPoints();
    void initBufQuads();


    void drawPoints();
    void drawQuads();

};

#endif