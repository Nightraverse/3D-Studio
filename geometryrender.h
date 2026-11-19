/*
 * geometryrender.h
 *
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 *
 * Adapted from code by Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include "openglwindow.h"
#include "shaderprogram.h"

class GeometryRender : public OpenGLWindow {
public:
    GeometryRender(const GlfwContext &ctx, std::filesystem::path shaderDir,
	std::string title, int width, int height, bool verbose = false);    
    ~GeometryRender() override;
    
    void initialize();
    virtual void display() override;

private:
    // Created in initialize(); null until then
    std::unique_ptr<ShaderProgram> m_shader;
    std::filesystem::path m_shaderDir;    

    // OpenGL buffers
    // handles to 
    GLuint m_vao = 0;
    GLuint m_vBuffer = 0;
    GLuint m_iBuffer = 0;

    // Geometry data
    std::vector<Vec2> m_vertices;
    std::vector<unsigned int> m_indices;

    void loadGeometry(void);
};
