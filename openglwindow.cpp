/*
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 *
 * Adapted from code by Stefan Johansson, stefanj@cs.umu.se
 */

#include <iostream>
#include "openglwindow.h"

// Initialize OpenGL context and viewport.
OpenGLWindow::OpenGLWindow(const GlfwContext &x, std::string title, int width,
	int height, bool verbose)
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Window size must be positive");
    
    // Store verbosity for shutdown
    m_verbose = verbose;

    // Store the title for debugging purposes
    m_title = title;

    if (m_verbose)
	std::cout << "Creating a window with title \"" << m_title << "\"...";
    
    // Store window size and create window
    m_windowWidth = width;
    m_windowHeight = height;

    // Create a window with the requested size and title, in windowed
    // (non-fullscree) mode, without any sharing of resources
    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
	std::cerr << "GLFW: Failed to create a window." << std::endl;
	return;
    }
    

    
    if (m_verbose)
	std::cout << "done." << std::endl;

    // Have OpenGL calls affect this window's framebuffer
    glfwMakeContextCurrent(m_window);

 // Set the user pointer of this window, to be used by frameBufferSizeCallback
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);


    // Set redraw speed to one refresh cycle (VSync)
    glfwSwapInterval(1); 

    // Init glad unless it has been loaded before
    static bool gladLoaded = false;
    if (!gladLoaded) {
	if (m_verbose)
	    std::cout << "Window \"" << m_title << "\": Setting up glad...";
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    std::cerr << "Failed to init glad, exiting." << std::endl;
	    return;
	}
	if (m_verbose)
	    std::cout << "done." << std::endl;
	// Remember that glad has been loaded
	gladLoaded = true;
    } else {
	// Just do a printout if glad is already loaded
	if (m_verbose)
	    std::cout << "Window \"" << m_title
		 << "\": glad is already loaded, continuing." << std::endl;
    }

    // Set initial viewport rectangle
    // Must be re-called on window resize
    glViewport(0,0, width, height);
}

// Close the window if opened and shut down GLFW
OpenGLWindow::~OpenGLWindow()
{
    if (m_verbose)
	std::cout << "Window \"" << m_title << "\": Closing GLFW window...";
    if (m_window) {
	glfwDestroyWindow(m_window);
	if (m_verbose)
	    std::cout << "done." << std::endl;
    } else {
	if (m_verbose)
	    std::cout << "window ptr was null, nothing to do." << std::endl;
    }
}

// Set the background color of the window with a specified alpha
void OpenGLWindow::setBackgroundColor(Vec4 c)
{
    if (m_verbose)
	std::cout << "Window \"" << m_title << "\": Setting background color to "
	     << c.toString(2) << "...";
    // Set the background color
    glClearColor(c.values[0], c.values[1], c.values[2], c.values[3]);
    if (m_verbose)
	std::cout << "done." << std::endl;
}

// Set the background color of the window with alpha = 1
void OpenGLWindow::setBackgroundColor(Vec3 c)
{
    setBackgroundColor(Vec4(c, 1.0f));
}

// Render the content of the window
void OpenGLWindow::display(void)
{
    // Clear the framebuffer using the current clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // Present the back buffer to the front buffer
    glfwSwapBuffers(m_window);

    if (m_verbose)
	std::cout << "Window \"" << m_title << "\": display() called." << std::endl;
}

// Render the scene 
void OpenGLWindow::displayNow()
{
    if (glfwGetCurrentContext() == nullptr)
        return;

    display();
}

// Start the event loop
void OpenGLWindow::start()
{
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(m_window)) {
        makeCurrent();           // matters only with several windows
        display();               // virtual: calls GeometryRender::display()
        glfwSwapBuffers(m_window);

        // Sleep until an event arrives
        glfwWaitEvents();
    }
}


// Function to be called when the window size is changed
void OpenGLWindow::framebufferSizeCallback(GLFWwindow* w, int width, int height){
	// Recover our object from the raw GLFW pointer
	auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(w));
	if (!self) return;
	
	// Must make the context current before the call to glViewPort
	self->makeCurrent();
	glViewport(0, 0, width, height);
	
	// Store the window size
	self->m_windowWidth = width;
	self->m_windowHeight = height;
	// Redraw immediately unless the window is minimized (size 0)
	if (width > 0 && height > 0) { self->displayNow(); }
}




