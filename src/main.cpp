#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"

static Shader shader;

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
    glm::mat4 mat_trans = glm::translate(glm::vec3(0.3f, 0.2f, 0.0f));
    glm::mat4 mat_rot = glm::rotate(glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // the order matters
    // glm::mat4 mat_modelview = mat_rot * mat_trans * mat_scale;
    glm::mat4 mat_modelview = mat_trans * mat_rot * mat_scale;
    
    GLuint modelview_loc = glGetUniformLocation( shader.program, "modelview" );
    glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, &mat_modelview[0][0]);

}

void drawTriangle()
{
    //glColor3f(1.0f, 0.0f, 0.0f);
    
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // draw triangle using indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main()
{
    GLFWwindow *window;

    // GLFW init
    if (!glfwInit())
    {
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

    initTriangle();

    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        drawTriangle();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}