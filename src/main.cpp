#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

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
}

void drawTriangle()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    
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
    window = glfwCreateWindow(640, 480, "Hello OpenGL1", NULL, NULL);
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