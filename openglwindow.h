/*
 * openglwindow.h
 *
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 *
 * Adapted from code by Stefan Johansson, stefanj@cs.umu.se
 */

#pragma once

#include "glfwcontext.h"
#include "3dstudio.h"

// Define a class to handle the initialization of an OpenGL window.
//
// All geometry-specific calls are handled by the subclass
// GeometryRender.

class OpenGLWindow {
public:
    // Create a window using GLFW with the given title and size
    // The GlfwContext enforces that GLFW has been initialized
    OpenGLWindow(const GlfwContext &,std::string title, int width, int height,
	bool verbose = false);
    // Cleanup
    virtual ~OpenGLWindow();

    // Disallow copy constructors
    OpenGLWindow(const OpenGLWindow&) = delete;
    OpenGLWindow& operator=(const OpenGLWindow&) = delete;
    
    // Set the background color of the window
    // The color contains r,g,b,a
    void setBackgroundColor(Vec4 c);
    // Set the background color with a=1
    void setBackgroundColor(Vec3 c);

    // Make the OpenGL context of this window current
    void makeCurrent() const { glfwMakeContextCurrent(m_window); }

    // Display the content of the window. Overridden in the
    // GeometryRender subclass.
    virtual void display();

    // Force a display call
    void displayNow(void);

    // Start the event loop
    void start(void);

    std::string getTitle(void) const { return m_title; }

protected:
    // For debugging
    bool m_verbose = false;
    std::string m_title;
private:
    static void framebufferSizeCallback(GLFWwindow* w, int width, int height);	
    	
    // Store the window pointer and size
    GLFWwindow* m_window = nullptr;
    int m_windowWidth = 0;
    int m_windowHeight = 0;
};
