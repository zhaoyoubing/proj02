#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "GLWin.h"

#include "shader.h"
#include "Mesh.h"

#include "Interaction.h"
#include "MeshFactory.h"
#include "ClothSim.h"
#include "AppMain.h"
#include "SphereMesh.h"
#include "RigidSphere.h"
#include "PlaneMesh.h"
#include "RigidPlane.h"
#include "ParticleSys.h"


App app;

static Shader shader;

glm::mat4 matModelRoot = glm::mat4(1.0);

glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 10.0f);
glm::vec3 viewPos_default = glm::vec3(0.0f, 0.0f, 300.0f);

GLuint blinnShader;
GLuint texblinnShader;

// viewport width and height
int width = 800;
int height = 800;

bool bWireframe = false;



// Initialize shader
GLuint initShader(std::string pathVert, std::string pathFrag) 
{
    shader.read_source( pathVert.c_str(), pathFrag.c_str());

    shader.compile();
    glUseProgram(shader.program);

    return shader.program;
}

void setLightPosition(glm::vec3 lightPos)
{
    GLuint lightpos_loc = glGetUniformLocation(shader.program, "lightPos" );
    glUniform3fv(lightpos_loc, 1, glm::value_ptr(lightPos));
}

void setViewPosition(glm::vec3 eyePos)
{
    GLuint viewpos_loc = glGetUniformLocation(shader.program, "viewPos" );
    glUniform3fv(viewpos_loc, 1, glm::value_ptr(eyePos));
}


void clearScene() 
{
    
    app.sim->clear();
}

void key_callback_sim(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << "simulation key callback " << std::endl;
    if (action == GLFW_PRESS)
    {
        // Controls
        if (GLFW_KEY_SPACE == key) {
           //app.sim->setPlaySim(true);
        } if (GLFW_KEY_R == key) {
           app.sim->clear();
        }
        
    }
}


int main()
{
    app.glWin = GLWin::createWin(width, height, "Hello Rigid Body");
    
    GLFWwindow *window = app.glWin->getGLFWwin();

    glViewport(0, 0, width, height);

    app.camera = std::make_shared<ArcballCamera>(
        viewPos_default,
        glm::vec3(0,0,0), // target
        20.0f,            // distance
        60.0f,            // FOV
        float(width) / float(height),
        0.1f,
        1000.0f
    );

    blinnShader = initShader("shaders/blinn.vert", "shaders/blinn.frag");
    //setLightPosition(lightPos);
    //setViewPosition(app.camera->eye);

    // ================================================
    // Particle System
    // Initialize the particle system class with a bunch of parameters:
    std::shared_ptr<Texture> tex = std::make_shared<Texture>("models/particle.png");
    std::shared_ptr<ParticleSystem> particleSystem = std::make_shared<ParticleSystem>(tex);

    particleSystem->pos = glm::vec3(0, 0, -0.5f);
    particleSystem->maxLife = 1.0f;
    particleSystem->acc = glm::vec3(0, 0, 0);
    particleSystem->partSize = glm::vec2(100, 100);

    app.sim = particleSystem;

    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    std::cout << "==================================" << std::endl;
    std::cout << "SPACE: Start simulation" << std::endl;
    //std::cout << "1: Single ball free falling " << std::endl;
    std::cout << "x: Toggle wireframe" << std::endl;
    std::cout << "r: Clear" << std::endl;
    std::cout << "==================================" << std::endl;

    // The view projection matrix will be used in the vertex shader to move the particle.
    app.sim->getMaterial()->SetMat4("view", app.camera->matView);
    app.sim->getMaterial()->SetMat4("proj", app.camera->matProj);
    // The viewport dimensions are needed in the geometry shader to make a correctly sized quad.
    // app.sim->getMaterial()->SetVec2((char*)"viewport", viewportDimensions);
    // app.sim->getMaterial()->Bind();

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        app.sim->getMaterial()->SetMat4("view", app.camera->matView);
        app.sim->getMaterial()->SetMat4("proj", app.camera->matProj);

        // update physics
        // time step of 0.005 second
        app.sim->tick(0.005);

        // draw the models
        app.sim->draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}


