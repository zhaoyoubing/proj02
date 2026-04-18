#include "ParticleSys.h"

#include <memory>
#include "ShaderProgram.h"

ParticleSystem::ParticleSystem(std::shared_ptr<Texture> texture)
{
    // Setup the compute shader material for the particlue simulation

    std::shared_ptr<ShaderProgram> simProgram = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> simShader 
            = std::make_shared<ShaderSingle>("shaders/particle/compute.glsl", GL_COMPUTE_SHADER);
    simProgram->AttachShader(simShader);
    m_particleSimulateMat = std::make_shared<Material>(simProgram);

    // Setup shaders and shader program.
    std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> vertexShader 
            = std::make_shared<ShaderSingle>("shaders/particle/vertex.glsl", GL_VERTEX_SHADER);
    program->AttachShader(vertexShader);
    std::shared_ptr<ShaderSingle> fragShader 
        = std::make_shared<ShaderSingle>("shaders/particle/fragment.glsl", GL_FRAGMENT_SHADER);
    program->AttachShader(fragShader);

    m_particleRenderMat = std::make_shared<Material>(program);
    m_particleRenderMat->Bind();
    m_particleRenderMat->SetTexture((char*)"tex", texture);

    // Create particle data.
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle& p = m_particles[i];
        p.m_age = (float)i / MAX_PARTICLES;
        
        float x = ((rand() % 100) / 50.0f) - 1.0f;
        float y = ((rand() % 100) / 50.0f) - 1.0f;

        p.m_position = glm::vec3(x, y, 0);
        p.m_velocity = glm::vec3(0, 0, 0);
        //p.m_angularVelocity = 0;
        //p.m_rotation = 0;
        p.m_color = glm::vec4(1, 0, 1, 1);
        p.size = 1.0f;
    }

    initBuffers();
}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, & partVertBuf);
    glDeleteBuffers(1, & quadVertBuf);
}

void ParticleSystem::initBuffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Quad VBO
    glGenBuffers(1, &quadVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, quadVertBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertQuad), vertQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Particle/Instance VBO
    glGenBuffers(1, & partVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partVertBuf);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle), m_particles, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);


    // 3rd attribute buffer : particles' colors
    /*
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(
    2, // attribute. No particular reason for 1, but must match the layout in the shader.
    4, // size : r + g + b + a => 4
    GL_UNSIGNED_BYTE, // type
    GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
    0, // stride
    (void*)0 // array buffer offset
    );
    */

    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);
}

std::shared_ptr<Material> ParticleSystem::getMaterial()
{
    return m_particleRenderMat;
}

void ParticleSystem::tick(float dt)
{
    // We are binding the vertex buffer from our square.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partVertBuf);

    // Same as with drawing, but we bind a compute shader program instead.
    // Set a bunch of values in the compute shader to use.
    m_particleSimulateMat->SetFloat((char*)"dt", dt);
    m_particleSimulateMat->SetFloat((char*)"burnRate", 1 / (float)m_lifeTime);
    m_particleSimulateMat->SetVec3((char*)"basePosition", m_position);
    m_particleSimulateMat->SetVec3((char*)"acceleration", m_acceleration);
    
	// bind, execute the compute program, and unbind
	m_particleSimulateMat->Bind();
    glDispatchCompute(MAX_PARTICLES, 1, 1);
    m_particleSimulateMat->Unbind();
	
	// unbind vertex buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

void ParticleSystem::draw()
{
    // Enable blending when rendering particles
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    //glUseProgram(program);
    m_particleRenderMat->Bind();
    glBindVertexArray(vao);

    // 🔥 One draw call for ALL particles
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MAX_PARTICLES);


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
    glDisable(GL_BLEND);
}