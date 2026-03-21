#ifndef __GL_WIN_H__
#define __GL_WIN_H__

#include <memory>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Interaction.h"

class GLWin {
public:
    int width;
    int height;

    static std::unique_ptr<GLWin> createWin(int width = 640, int height = 480) {
        std::unique_ptr<GLWin> win = std::make_unique<GLWin>(width, height);
        win->init();

        return win;
    }

    GLFWwindow * getGLFWwin() { return window; }

    // should make this constructor private, but need special handling of smart pointers
    GLWin(int w, int h) {
        width = w;
        height = h;
        window = NULL;
    }

private:
    GLFWwindow * window;



    int init() 
    {
        // GLFW init
        if (!glfwInit())
        {
            std::cout << "glfw failed" << std::endl;
            return -1;
        }

        // create a GLFW window
        window = glfwCreateWindow(width, height, "Hello Mass-Spring", NULL, NULL);
        glfwMakeContextCurrent(window);

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

        return 0;

    }
};

#endif