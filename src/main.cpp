#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"
#include "Mesh.h"
#include "Node.h"

static Shader shader;

// Initialize shader
void initShader(std::string pathVert, std::string pathFrag) 
{
    shader.read_source( pathVert.c_str(), pathFrag.c_str());

    shader.compile();
    glUseProgram(shader.program);
}

float rot_x = 0;
float rot_y = 0;
glm::mat4 matRoot = glm::mat4(1.0);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT ) {
        rot_y -= 5.0;
    } else if (key == GLFW_KEY_RIGHT /*&& action == GLFW_PRESS*/) {
        rot_y += 5.0;
    } if (key == GLFW_KEY_DOWN ) {
        rot_x += 5.0;
    } else if (key == GLFW_KEY_UP) {
        rot_x -= 5.0;
    }

    glm::mat4 mat_rot_y = glm::rotate(glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mat_rot_x = glm::rotate(glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));

    matRoot =  mat_rot_x * mat_rot_y;
    
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
    window = glfwCreateWindow(640, 640, "Hello OpenGL 3", NULL, NULL);
    glfwMakeContextCurrent(window);

    // register the key event callback function
    glfwSetKeyCallback(window, key_callback);


    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

 
    // !!!!!!!!!! Use the following with Visual Studio
    initShader( "shaders/colour.vert", "shaders/colour.frag");

    //----------------------------------------------------
    // Meshes
    std::shared_ptr<Mesh> cube = std::make_shared<Mesh>();
    cube->init("models/cube.obj", shader.program);

    std::shared_ptr<Mesh> teapot = std::make_shared<Mesh>();
    teapot->init("models/teapot.obj", shader.program);
    
    //----------------------------------------------------
    // Nodes
    std::shared_ptr<Node> scene = std::make_shared<Node>();
    std::shared_ptr<Node> teapotNode = std::make_shared<Node>();
    std::shared_ptr<Node> cubeNode = std::make_shared<Node>();
    
    //----------------------------------------------------
    // Build the tree
    teapotNode->addMesh(teapot);
    cubeNode->addMesh(cube);
    cubeNode->addChild(teapotNode, glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    // cubeNode->addChild(teapotNode, glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)), glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    
    //----------------------------------------------------
    // Add the tree to the world space
    scene->addChild(cubeNode);
    // scene->addChild(cubeNode, glm::translate(glm::vec3(1.0f, 0.0f, 0.0f)), glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pMesh->draw();
        scene->draw(matRoot);
        
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}