/*
 * shaderprogram.h
 *
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 */

#pragma once

#include <string>
#include <filesystem>
#include <glad/glad.h>
#include "openglwindow.h"

// Reads, compiles and links a vertex + fragment shader pair.
// Requires a current OpenGL context when constructed.
class ShaderProgram {
public:
    // Throws std::runtime_error with the info log on failure
    ShaderProgram(const OpenGLWindow& w,
	const std::filesystem::path& shaderDir,
	const std::string& vertexFile,
	const std::string& fragmentFile, bool verbose = false);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    // Load the source code of all shaders.
    void loadSources(void);
    
    // Compile and link the shaders to create a shader program. Throws
    // std::runtime_error with error log if error.
    void compileShaders(void);
	
    // Install as part of the current rendering state
    void use() const { glUseProgram(m_programId); }
    GLuint program() const { return m_programId; }

    // Return the location of the VAO attribute within the current program
    GLint attributeLocation(const std::string& name) const;
    
    void ShaderProgram::setUniform(const std::string& name, const Mat4x4 m, GLboolean transpose) const {
        glProgramUniformMatrix4fv (program(), uniformLocation (name), 1, transpose, m);
    }
    
private:
    // OpenGL window that shader program will be attached to
    // The window must outlive this object (it owns the GL context)
    const OpenGLWindow& m_window;
    // id of the compiled shader program
    GLuint m_programId = 0;
    // Shader directory
    const std::filesystem::path m_shaderDir;
    
    // Shader file names
    std::string m_vertexFilename, m_fragmentFilename;

    // Shader file content
    std::string m_vertexCode, m_fragmentCode;

    // Read the shader source code. The type should be either
    // GL_VERTEX_SHADER or GL_FRAGMENT_SHADER. Throws
    // std::runtime_error if error, including file does not exist.
    void loadSource(GLenum type);

    // Compile a loaded shader. Shader code must have been loaded.
    // Throws std::runtime_error with compile log if error.
    GLuint compileShader(GLenum type);

    // Read a text file. Throws std::runtime_error if error. Uses type
    // in error message.
    static std::string readFile(const char *type, const std::string& file);
    
    // For debugging
    bool m_verbose = false;
};
