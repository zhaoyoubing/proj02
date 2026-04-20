#include "ParticleSys.h"

#include <GLFW/glfw3.h>
#include <memory>
#include "ShaderProgram.h"

ParticleSystem::ParticleSystem(std::shared_ptr<Texture> texture, bool drawPoints)
{
    bDrawPoints = drawPoints;

    // Setup the compute shader material for the particlue simulation

    std::shared_ptr<ShaderProgram> simProgram = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> simShader 
            = std::make_shared<ShaderSingle>("shaders/particle/fire.comp", GL_COMPUTE_SHADER);
    simProgram->AttachShader(simShader);
    partSimMat = std::make_shared<Material>(simProgram);

    std::string fVert = "shaders/particle/point.vert";
    std::string fFrag = "shaders/particle/point.frag";

    if (! bDrawPoints) {
        fVert = "shaders/particle/quad_ssbo.vert";
        fFrag = "shaders/particle/quad.frag";
    }

    // Setup shaders and shader program.
    std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> vertexShader 
            = std::make_shared<ShaderSingle>(fVert.c_str(), GL_VERTEX_SHADER);
    program->AttachShader(vertexShader);
    std::shared_ptr<ShaderSingle> fragShader 
        = std::make_shared<ShaderSingle>(fFrag.c_str(), GL_FRAGMENT_SHADER);
    program->AttachShader(fragShader);

    partDrawMat = std::make_shared<Material>(program);
    partDrawMat->SetTexture((char*)"tex", texture);

    partDrawMat->Bind();
    
    // sprites 8 x 6
    //glm::vec2 uvScale  = glm::vec2(1 / 8.0f, 1/6.0f);

    // Create particle data.
    for (int i = 0; i < NUM_POINTS; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle& p = particles[i];
        // p.life = (float)i / NUM_POINTS;

        // random position for point rendering
        //float x = ((rand() % 100) / 5.0f) - 10.0f;
        //float y = ((rand() % 100) / 5.0f) - 10.0f;
        //p.pos = glm::vec4(x, y, 0, 1.0);

        //p.pos = glm::vec4(0, 0, 0, 1.0);
        p.pos = glm::vec4(pos, 1.0);
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
    glDeleteVertexArrays(1, &vao);
    
    glDeleteBuffers(1, & quadVertBuf);
    glDeleteBuffers(1, & partBuf);
}

void ParticleSystem::initBufPoints()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Particle/Instance VBO
    glGenBuffers(1, & partBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partBuf);
    glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

    // positions of particles' centers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);

    // colour
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(vao);
}


void ParticleSystem::initBufQuads()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Quad VBO for instancing
    glGenBuffers(1, &quadVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, quadVertBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertQuad), vertQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    /*
    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));

    glVertexAttribDivisor(2, 1);
    */

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenBuffers(1, &partBuf);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, partBuf);

    // Particle/Instance VBO
    //glGenBuffers(1, & partVertBuf);
    //glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleSystem::initBuffers()
{
    if (bDrawPoints) 
        initBufPoints();
    else
        initBufQuads();
}

std::shared_ptr<Material> ParticleSystem::getMaterial()
{
    return partDrawMat;
}

void ParticleSystem::tick(float dt)
{
    if (! bPlaySim ) return;

    // We are binding the vertex buffer from our square.
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partBuf);

    // Same as with drawing, but we bind a compute shader program instead.
    // Set a bunch of values in the compute shader to use.
    partSimMat->SetFloat((char*)"dt", dt);
    float time = (float) glfwGetTime();
    partSimMat->SetFloat((char*)"time", time);
    partSimMat->SetVec3((char*)"basePos", pos);
    
	// bind, execute the compute program, and unbind
	partSimMat->Bind();
    glDispatchCompute(NUM_POINTS, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    partSimMat->Unbind();
	
	// unbind vertex buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

void ParticleSystem::drawPoints()
{
    partDrawMat->Bind();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, partBuf);

    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, NUM_POINTS);

    partDrawMat->Unbind();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::drawQuads()
{
    // Enable blending when rendering particles
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    partDrawMat->Bind();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partBuf);
    //glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);
    
    glBindVertexArray(vao);

    // 🔥 One draw call for ALL particles
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, NUM_POINTS);

    partDrawMat->Unbind();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void ParticleSystem::draw()
{

    if (bDrawPoints)
        drawPoints();
    else
        drawQuads();
    
}