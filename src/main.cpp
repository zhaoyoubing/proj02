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


App app;

static Shader shader;

glm::mat4 matModelRoot = glm::mat4(1.0);

glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 10.0f);
glm::vec3 viewPos_default = glm::vec3(0.0f, 0.0f, 100.0f);

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

int main()
{
    app.glWin = GLWin::createWin(width, height, "Hello Mass-Spring");
    
    GLFWwindow *window = app.glWin->getGLFWwin();

    app.camera = std::make_shared<ArcballCamera>(
        viewPos_default,
        glm::vec3(0,0,0), // target
        100.0f,            // distance
        60.0f,            // FOV
        float(width) / float(height),
        0.1f,
        200.0f
    );

    blinnShader = initShader("shaders/blinn.vert", "shaders/blinn.frag");
    setLightPosition(lightPos);
    setViewPosition(app.camera->eye);

    //texblinnShader = initShader("shaders/texblinn.vert", "shaders/texblinn.frag");
    //setLightPosition(lightPos);
    //setViewPosition(app.camera->eye);

    //std::shared_ptr<PlaneMesh> cloth = MeshFactory::createPlane(PlaneMesh::XZ, 20, 22, 20, 20, 10.0f, "models/carpet.png");
    //cloth->setShaderId(texblinnShader);

    std::shared_ptr<SphereMesh> ball = std::make_shared<SphereMesh>(20, 20, 5.0f);
    ball->setShaderId(blinnShader);

    app.sim = std::make_shared<RigidSim>();

    std::shared_ptr<RigidSphere> ball1 = std::make_shared<RigidSphere>(1.0f);
    ball1->setMesh(ball);
    std::shared_ptr<RigidObj> ball1Obj = std::dynamic_pointer_cast<RigidObj>(ball1);
    ball1Obj->setPosition(glm::vec3(0, 50.0f, 0));
    ball1Obj->setVelocity(glm::vec3(-5.0, 0.0f, 0.0f));
    app.sim->add(ball1Obj);
  
    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    std::cout << "==================================" << std::endl;
    std::cout << "Space key: Start simulation" << std::endl;
    std::cout << "v: Enable/Disable wind" << std::endl;
    std::cout << "g: Inverse gravity" << std::endl;
    std::cout << "x: Toggle wireframe" << std::endl;
    std::cout << "r: Reset" << std::endl;
    std::cout << "==================================" << std::endl;

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // time step of 0.005 second
        app.sim->tick(0.001);

        app.sim->draw();

        //ball->draw(matModelRoot, app.camera->matView, app.camera->matProj);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}


