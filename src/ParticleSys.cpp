#include "ParticleSys.h"

#include <GLFW/glfw3.h>
#include <memory>
#include "ShaderProgram.h"

ParticleSystem::ParticleSystem(std::shared_ptr<Texture> texture)
{
    // Setup the compute shader material for the particlue simulation

    std::shared_ptr<ShaderProgram> simProgram = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> simShader 
            = std::make_shared<ShaderSingle>("shaders/particle/fire.comp", GL_COMPUTE_SHADER);
    simProgram->AttachShader(simShader);
    partSimMat = std::make_shared<Material>(simProgram);

    // Setup shaders and shader program.
    std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> vertexShader 
            = std::make_shared<ShaderSingle>("shaders/particle/point.vert", GL_VERTEX_SHADER);
    program->AttachShader(vertexShader);
    std::shared_ptr<ShaderSingle> fragShader 
        = std::make_shared<ShaderSingle>("shaders/particle/point.frag", GL_FRAGMENT_SHADER);
    program->AttachShader(fragShader);

    partDrawMat = std::make_shared<Material>(program);
    partDrawMat->SetTexture((char*)"tex", texture);

    partDrawMat->Bind();
    

    // Create particle data.
    for (int i = 0; i < NUM_POINTS; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle& p = particles[i];
        p.life = (float)i / NUM_POINTS;
        
        float x = ((rand() % 100) / 5.0f) - 10.0f;
        float y = ((rand() % 100) / 5.0f) - 10.0f;

        p.pos = glm::vec4(x, y, 0, 1.0);

        p.pos = glm::vec4(0, 0, 0, 1.0);
        p.velocity = glm::vec4(0, 0, 0, 0);
        //p.m_angularVelocity = 0;
        //p.m_rotation = 0;
        p.color = glm::vec4((rand() % 256) / 255.0f, 
                (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1);
        p.maxLife = maxLife;
    }

    initBuffers();
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, & partVertBuf);
    glDeleteBuffers(1, & quadVertBuf);
}

void ParticleSystem::initBufPoints()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Particle/Instance VBO
    glGenBuffers(1, & partVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);
    glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // positions of particles' centers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
}


void ParticleSystem::initBufQuads()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Quad VBO for instancing
    /*
    glGenBuffers(1, &quadVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, quadVertBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertQuad), vertQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    */

    // Particle/Instance VBO
    glGenBuffers(1, & partVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);
    glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);


    //glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::initBuffers()
{
    if (bPoints) 
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
    // We are binding the vertex buffer from our square.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partVertBuf);

    // Same as with drawing, but we bind a compute shader program instead.
    // Set a bunch of values in the compute shader to use.
    partSimMat->SetFloat((char*)"dt", dt);
    float time = (float) glfwGetTime();
    partSimMat->SetFloat((char*)"time", time);
    partSimMat->SetVec3((char*)"basePos", pos);
    //partSimMat->SetFloat((char*)"burnRate", 1 / (float) maxLife);
    //partSimMat->SetVec3((char*)"acceleration", acc);
    
	// bind, execute the compute program, and unbind
	partSimMat->Bind();
    glDispatchCompute(NUM_POINTS, 1, 1);
    partSimMat->Unbind();
	
	// unbind vertex buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

void ParticleSystem::drawPoints()
{
    partDrawMat->Bind();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);

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
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);

    // 🔥 One draw call for ALL particles
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, NUM_POINTS);

    partDrawMat->Unbind();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void ParticleSystem::draw()
{

    if (bPoints)
        drawPoints();
    else
        drawQuads();


    /*
    // Bind the vertex buffer. and set up attributes
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(0));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 4));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 8));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 12));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 13));
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 14));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // use the vertex attributes we just declared
    for (int i = 0; i < 6; i++)
    {
        glEnableVertexAttribArray(i);
    }


    // The particle size is used in the geometry shader to create quads.
    m_particleRenderMat->SetVec2((char*)"particleSize", m_particleSize);

    // Bind material and draw
    m_particleRenderMat->Bind();

    // The geometry shader is expecting points, so we call draw with points, once for each particle.
    glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);


    // reset everything:
    m_particleRenderMat->Unbind();
    for (int i = 0; i < 6; i++)
    {
        glDisableVertexAttribArray(i);
    }
    */
    
    
}