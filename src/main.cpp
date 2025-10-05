#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"
#include "Mesh.h"

static Shader shader;


void testMesh()
{
    Mesh * pMesh = new Mesh();
    pMesh->loadModel("D:/_course/_graphics/_labs/lab03/proj_cg/models/teapot.obj");
}

void initTriangle()
{
    /*
    // raw triangle data with duplicated vertices
    GLfloat verts[] = {
        -1.0f, 1.0f,  // v0
        -1.0f, -1.0f, // v1
        1.0f, -1.0f,  // v2

        1.0f, -1.0f,  // v2
        1.0f, 1.0f,   // v3
        -1.0f, 1.0f,  // v0
    };
    */

    /*
    // vertex data with RGB colour components
    GLfloat verts[] = {
        -1.0f, 1.0f,  0.0f, // v0
        0.0f, 1.0f,  0.0f,  // v0 colour green
        -1.0f, -1.0f, 0.0f, // v1
        0.0f, 0.0f, 0.0f,   // v1 colour black
        1.0f, -1.0f,  0.0f, // v2
        1.0f, 0.0f,  0.0f,  // v2 colour red
        1.0f, 1.0f,   0.0f, // v3
        1.0f, 1.0f,   0.0f, // v3 colour yellow
    };

    // indices of two triangles
    GLuint indices[] = { 0, 1, 2, 2, 3, 0};
    */

    // cube vertex data with RGB colour components
    GLfloat verts[] = {
        -1.0f, 1.0f,  1.0f, // v0
        0.0f, 1.0f,  0.0f,  // v0 colour green
        -1.0f, -1.0f, 1.0f, // v1
        0.0f, 0.0f, 0.0f,   // v1 colour black
        1.0f, -1.0f,  1.0f, // v2
        1.0f, 0.0f,  0.0f,  // v2 colour red
        1.0f, 1.0f,   1.0f, // v3
        1.0f, 1.0f,   0.0f, // v3 colour yellow

        -1.0f, 1.0f, -1.0f, // v4
        0.0f, 1.0f,  1.0f,  // v4 colour cyan
        -1.0f, -1.0f, -1.0f, // v5
        0.0f, 0.0f, 1.0f,   // v5 colour blue
        1.0f, -1.0f,  -1.0f, // v6
        1.0f, 0.0f,  1.0f,  // v6 colour magenta
        1.0f, 1.0f,  -1.0f, // v7
        1.0f, 1.0f,  1.0f, // v7 colour white
    };


    // indices of 12 triangles of a cube
    GLuint indices[] = { 
        0, 1, 2,  0, 2, 3,
        1, 5, 2,  5, 6, 2,
        0, 3, 4,  4, 3, 7,
        3, 2, 7,  2, 6, 7,
        1, 0, 4,  1, 4, 5,
        7, 6, 4,  4, 6, 5
    
    };


    // create vertex buffer
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

    // set colour attributes
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (sizeof(float) * 3));

    // create index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Initialize shader
    shader.read_source( "../shaders/colour.vert", "../shaders/colour.frag");
    // use the following if you are using Visual Studio
    // shader.read_source( "shaders/colour.vert", "shaders/colour.frag");
    shader.compile();
    glUseProgram(shader.program);

    /*
    glm::mat4 mat_scale = glm::scale(  // Scale first
        glm::mat4(1.0),              // identity matrix
        glm::vec3( 0.5f, 0.5f, 0.5f )
    );
    */

    glm::mat4 mat_scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 mat_trans = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 mat_rot = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mat_rot2 = glm::rotate(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // the order matters
    // glm::mat4 mat_modelview = mat_rot * mat_trans * mat_scale;
    // glm::mat4 mat_modelview = mat_trans * mat_rot * mat_scale;
    // glm::mat4 mat_modelview =  mat_rot2 * mat_rot *  mat_scale;
    glm::mat4 mat_modelview =   mat_rot2 * mat_rot * mat_scale;

    glm::mat4 mat_projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    GLuint modelview_loc = glGetUniformLocation( shader.program, "modelview" );
    glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, &mat_modelview[0][0]);

    // you must set the orthographic projection to get correct rendering with depth
    GLuint projection_loc = glGetUniformLocation( shader.program, "projection" );
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &mat_projection[0][0]);

}

void drawTriangle()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       
    // draw triangle using indices
    glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);

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
    window = glfwCreateWindow(640, 640, "Hello OpenGL 2", NULL, NULL);
    glfwMakeContextCurrent(window);


    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couuldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    // test our Mesh class
    testMesh();


    initTriangle();

    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_GREATER);  

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawTriangle();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}