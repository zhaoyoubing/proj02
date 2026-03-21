#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "shader.h"
#include "Mesh.h"
//#include "Node.h"

//#include "ArcballCamera.h"
#include "Interaction.h"
#include "MeshFactory.h"

static Shader shader;

glm::mat4 matModelRoot = glm::mat4(1.0);
// glm::mat4 matView = glm::mat4(1.0);
// glm::mat4 matProj = glm::ortho(-2.0f,2.0f,-2.0f,2.0f, -2.0f,2.0f);

glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 10.0f);
glm::vec3 viewPos_default = glm::vec3(0.0f, 6.0f, 6.0f);
// glm::vec3 viewPos = viewPos_default;

// We are using mesh list instead of scenegraph to demo our picking and collision detection
std::vector< std::shared_ptr <Mesh> > meshList;
std::vector< glm::mat4 > meshMatList;

// GLuint flatShader;
GLuint blinnShader;
GLuint phongShader;
GLuint texblinnShader;

// viewport width and height
int width = 800;
int height = 800;

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
    GLFWwindow *window;

    // GLFW init
    if (!glfwInit())
    {
        std::cout << "glfw failed" << std::endl;
        return -1;
    }

    // create a GLFW window
    window = glfwCreateWindow(width, height, "Hello OpenGL 11", NULL, NULL);
    glfwMakeContextCurrent(window);


    camera = std::make_shared<ArcballCamera>(
        viewPos_default,
        glm::vec3(0,0,0), // target
        5.0f,             // distance
        60.0f,            // FOV
        float(width) / float(height),
        0.5f,
        20.0f
    );

    // register the key event callback function
    glfwSetKeyCallback(window, key_callback);
    
    // register the mouse button event callback function
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    blinnShader = initShader( "shaders/blinn.vert", "shaders/blinn.frag");
    setLightPosition(lightPos);
    setViewPosition(camera->eye);
    // added for LabA07
    texblinnShader = initShader("shaders/texblinn.vert", "shaders/texblinn.frag");
    setLightPosition(lightPos);
    setViewPosition(camera->eye);

    //----------------------------------------------------
    // Meshes
    std::shared_ptr<Mesh> cube = MeshFactory::importAssimp("models/cube.obj");
    cube->setShaderId(blinnShader);

    glm::mat4 mat = glm::mat4(1.0);

    std::shared_ptr<Mesh> teapot = MeshFactory::importAssimp("models/teapot.obj");
    teapot->setShaderId(blinnShader);
    meshList.push_back(teapot);
    mat = glm::translate(glm::vec3(-2.0f, 1.0f, 0.0f));
    meshMatList.push_back(mat); // TRS
    teapot->initSpatial(true, mat);
    
    std::shared_ptr<Mesh> bunny = MeshFactory::importAssimp("models/bunny_normal.obj");
    bunny->setShaderId(texblinnShader);
    meshList.push_back(bunny);
    mat = glm::translate(glm::vec3(1.5f, 1.5f, 0.0f)) *
          glm::scale(glm::vec3(0.005f, 0.005f, 0.005f));
    meshMatList.push_back( mat ); // TRS
    bunny->initSpatial(true, mat);

    std::shared_ptr<PlaneMesh> cloth = MeshFactory::createPlane(PlaneMesh::XZ, 5, 5, 10, 10, "models/carpet.png");
    cloth->setShaderId(texblinnShader);
  

    //----------------------------------------------------
    // Nodes
    // std::shared_ptr<Node> scene = std::make_shared<Node>();
    // std::shared_ptr<Node> teapotNode = std::make_shared<Node>();
    // std::shared_ptr<Node> cubeNode = std::make_shared<Node>();
    // std::shared_ptr<Node> bunnyNode = std::make_shared<Node>();
    
    //----------------------------------------------------
    // Build the tree
    //teapotNode->addMesh(teapot);
    //cubeNode->addMesh(cube, glm::mat4(1.0), glm::mat4(1.0), glm::scale(glm::vec3(2.0f, 0.25f, 1.5f)));
    //bunnyNode->addMesh(bunny, glm::mat4(1.0), glm::mat4(1.0), glm::scale(glm::vec3(0.005f, 0.005f, 0.005f)));


    // cubeNode->addChild(teapotNode, glm::translate(glm::vec3(-1.5f, 0.5f, 0.0f)));
    // cubeNode->addChild(bunnyNode, glm::translate(glm::vec3(1.0f, 1.5f, 0.0f)));
    // cubeNode->addChild(teapotNode, glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)), glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    
    //----------------------------------------------------
    // Add the tree to the world space
    //scene->addChild(cubeNode);
    //scene->addChild(bunnyNode);
    // scene->addChild(cubeNode, glm::translate(glm::vec3(1.0f, 0.0f, 0.0f)), glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //scene->draw(matModelRoot, matView, matProj);
        // bunny->draw(glm::scale(glm::vec3(0.005f, 0.005f, 0.005f)), matView, matProj);

        /*
        for (int i = 0; i < meshList.size(); i++ ) {
            std::shared_ptr<Mesh> pMesh = meshList[i];
            pMesh->draw(matModelRoot * meshMatList[i], camera->matView, camera->matProj);
        }
        */

        cloth->draw(matModelRoot, camera->matView, camera->matProj);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}


