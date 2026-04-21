#include "ParticleSys.h"

#include <GLFW/glfw3.h>
#include <memory>
#include "ShaderProgram.h"

ParticleSystem::ParticleSystem()
{
    // set up the compute shader material for the particlue simulation
    std::shared_ptr<ShaderProgram> simProgram = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> simShader 
            = std::make_shared<ShaderSingle>("shaders/particle/fire.comp", GL_COMPUTE_SHADER);
    simProgram->attachShader(simShader);
    simMat = std::make_shared<Material>(simProgram);

    // Setup shaders and shader program.
    progPoint = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> vertexShader 
            = std::make_shared<ShaderSingle>("shaders/particle/point.vert", GL_VERTEX_SHADER);
    progPoint->attachShader(vertexShader);
    std::shared_ptr<ShaderSingle> fragShader 
        = std::make_shared<ShaderSingle>("shaders/particle/point.frag", GL_FRAGMENT_SHADER);
    progPoint->attachShader(fragShader);

    drawPointMat = std::make_shared<Material>();
    drawPointMat->setShaderProgram(progPoint);


    progQuadTex = std::make_shared<ShaderProgram>();
    vertexShader = std::make_shared<ShaderSingle>("shaders/particle/quad.vert", 
                                                    GL_VERTEX_SHADER);
    progQuadTex->attachShader(vertexShader);
    fragShader = std::make_shared<ShaderSingle>("shaders/particle/quad.frag", 
                                                    GL_FRAGMENT_SHADER);
    progQuadTex->attachShader(fragShader);

    drawTexMat = std::make_shared<Material>();
    drawTexMat->setShaderProgram(progQuadTex);

    progQuadSprite = std::make_shared<ShaderProgram>();
    vertexShader = std::make_shared<ShaderSingle>("shaders/particle/quad_sprite.vert", 
                                                    GL_VERTEX_SHADER);
    progQuadSprite->attachShader(vertexShader);
    fragShader = std::make_shared<ShaderSingle>("shaders/particle/quad_sprite.frag", 
                                                    GL_FRAGMENT_SHADER);
    progQuadSprite->attachShader(fragShader);

    drawSpriteMat = std::make_shared<Material>();
    drawSpriteMat->setShaderProgram(progQuadSprite);
    
    // Create particle data.
    // [TODO] set particle positions
    for (int i = 0; i < NUM_POINTS; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle & p = particles[i];

        // random position for point rendering
        float x = ((rand() % 100) / 5.0f) - 10.0f;
        float y = ((rand() % 100) / 5.0f) - 10.0f;

        // [TODO 1.1]
        // set the initial particle position 
        // to a random position (x, y, z = 0.0, w = 1.0)
        // p.pos = glm::vec4(x, y, 0, 1.0);

        p.pos = glm::vec4(basePos, 1.0);
        p.velocity = glm::vec4(0, 0, 0, 0);

        //p.m_angularVelocity = 0;
        //p.m_rotation = 0;
        //p.color = glm::vec4((rand() % 256) / 255.0f, 
        //        (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1);

        p.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        
        p.life = maxLife;
        p.maxLife = maxLife;
    }

    initBuffers();
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays(1, &vaoPoint);
    glDeleteVertexArrays(1, &vaoQuad);
    glDeleteBuffers(1, & quadVertBuf);
    glDeleteBuffers(1, & partBuf);
}

void ParticleSystem::initBufPoints()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, & vaoPoint);
    glBindVertexArray(vaoPoint);

    // Particle/Instance VBO
    // glGenBuffers(1, & partBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partBuf);
    //glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

    // [TODO] set the vertex attribute pointer for particle positions
    // positions of particles' centers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);

    // colour
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));

    glBindVertexArray(0);
}


void ParticleSystem::initBufQuads()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, & vaoQuad);
    glBindVertexArray(vaoQuad);

    // Quad VBO for instancing
    glGenBuffers(1, &quadVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, quadVertBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertQuad), vertQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

}

void ParticleSystem::initBuffers()
{
    // create the particle buffer
    glGenBuffers(1, & partBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partBuf);
    glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

    initBufPoints();
    initBufQuads();
}


// use the compute shader
void ParticleSystem::tick(float dt)
{
    if (! bPlaySim ) return;

    // Same as with drawing, but we bind a compute shader program instead.
    // Set a bunch of values in the compute shader to use.
    simMat->setFloat((char*)"dt", dt);
    float time = (float) glfwGetTime();
    simMat->setFloat((char*)"time", time);
    simMat->setVec3((char*)"basePos", basePos);
    simMat->setVec3((char*)"acc", acc);
    
	// use the compute shader
	simMat->bind();

    // We are binding the particle buffer from our square.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partBuf);
    glDispatchCompute(NUM_POINTS, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    simMat->unbind();
	
}

void ParticleSystem::drawPoints()
{
    glBindVertexArray(vaoPoint);

    glPointSize(50.0f * size);

    glDrawArrays(GL_POINTS, 0, NUM_POINTS);

    glBindVertexArray(0);
}

void ParticleSystem::drawQuad()
{
    glBindVertexArray(vaoQuad);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partBuf);
    
    // draw instanced quads for all particles
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, NUM_POINTS);

    glBindVertexArray(0);

}



void ParticleSystem::draw()
{
    std::shared_ptr<Material> currentMat;

    switch (drawMode)
    {
        case DrawMode::POINT:
            currentMat = drawPointMat;
            break;
        case DrawMode::TEXTURE:
            currentMat = drawTexMat;
            break;
        case DrawMode::SPRITE: 
            currentMat = drawSpriteMat;
            break;  
    }

    currentMat->setMat4("view", camera->matView);
    currentMat->setMat4("proj", camera->matProj);
    currentMat->setFloat("size", size);

    currentMat->bind();
 
    // Enable blending when rendering particles
    glEnable(GL_BLEND);
    // [TODO] specifiy the blend function 
    // for fire with black background
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // for smoke with white background
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (drawMode)
    {
        case DrawMode::POINT:
            drawPoints();
            break;
        case DrawMode::TEXTURE:
        case DrawMode::SPRITE: 
            drawQuad();
            break;
    }

    // unuse the shader programs
    currentMat->unbind();

    glDisable(GL_BLEND);
}