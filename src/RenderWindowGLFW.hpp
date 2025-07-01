#pragma once

#include <glfw3.h>
#include <iostream>

class RenderWindowGLFW
{
    public:
        RenderWindowGLFW();
        
        bool Initialization();
        void LoopEvent();
        
        void Ready();
        void Input();
        void Update();
        void Render();
    
    
};

RenderWindowGLFW::RenderWindowGLFW()
{    
    Initialization();
}
bool RenderWindowGLFW::Initialization()
{
    if(!glfwInit())
    {
        std::cerr << "Failed to Initialization GLFW.\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Software Renderer", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to Create Window.\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);

    return true;

}
