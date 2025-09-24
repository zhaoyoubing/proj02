#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

void initTriangle()
{
    // triangle data
    GLfloat verts[] = {
        0.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f
    };

    // create vertex buffer
    GLuint bufID;
    glGenBuffers(1, &bufID);
    glBindBuffer(GL_ARRAY_BUFFER, bufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void drawTriangle()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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