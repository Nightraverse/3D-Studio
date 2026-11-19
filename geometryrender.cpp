/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include <stdexcept>
#include "geometryrender.h"

// Store shaderDir and call parent constructor with the rest
GeometryRender::GeometryRender(const GlfwContext &ctx,
                               std::filesystem::path shaderDir,
                               std::string title, int width, int height,
                               bool verbose)
    : OpenGLWindow(ctx, std::move(title), width, height, verbose),
      m_shaderDir(std::move(shaderDir))
{
}

GeometryRender::~GeometryRender()
{
    makeCurrent();                       // delete in the right context
    glDeleteVertexArrays(1, &m_vao);       // deleting 0 is silently ignored
    glDeleteBuffers(1, &m_vBuffer);
    glDeleteBuffers(1, &m_iBuffer);
}

// Initialize OpenGL
void GeometryRender::initialize()
{
    makeCurrent();
    
    // Enable depth test. Needed for 3D data only.
    glEnable(GL_DEPTH_TEST);

    // Create a new ShaderProgram and store it in the shader member.
    // Destroys any old ShaderProgram object.
    m_shader.reset(new ShaderProgram(*this, m_shaderDir, "vshader.glsl", "fshader.glsl",
	    m_verbose));

    // Load the source code for the shaders. Will throw an exception
    // if either file does not exist.
    m_shader->loadSources();
    // Compile and link the shaders. Will throw an exception for failure.
    m_shader->compileShaders();

    // Load the vertex data
    loadGeometry();
}

void GeometryRender::loadGeometry()
{
    // 2d data; 3 corners of a triangle. Stored in system RAM. The
    // coordinates are in normalized device coordinates [-1,1].
    m_vertices = { Vec2(0,0), Vec2(0.5,0), Vec2(0,0.5) };
    // Vertex numbers (Indices into vertices). Will be used to
    // describe how to connect the points when drawn. Stored in system
    // RAM.
    m_indices = { 0, 1, 2 };

    // Create OpenGL buffer objects. Each object is initialized into
    // an empty, valid state with no GPU memory connected.
    // Each object is known via an object handle (an integer).

    // m_vBuffer will be used as a VBO (Vertex Buffer Object) and
    // store the vertex data.
    glCreateBuffers(1, &m_vBuffer);
    // m_iBuffer will be used as an EBO (Element Buffer Object) and
    // store indices into the VBO.
    glCreateBuffers(1, &m_iBuffer);
    
    // Allocate storage to the VBO and EBO in the GPU driver and copy
    // data from the CPU. The driver is responsible for how and when
    // to transfer the data to the GPU. Flags 0 means the content can
    // never be modified from the CPU.
    //
    // Since the data is copied to the driver, vertices and indices
    // may be freed after these calls.
    glNamedBufferStorage(m_vBuffer, m_vertices.size() * sizeof(Vec2),
	m_vertices.data(), 0);
    glNamedBufferStorage(m_iBuffer, m_indices.size() * sizeof(unsigned int),
	m_indices.data(), 0);

    // Create an empty VAO (Vertex Array Object) that will store the
    // "plumbing diagram" of how to get the vertex data to the vertex
    // processor on the GPU.
    glCreateVertexArrays(1, &m_vao);

    // This example only has one data buffer; use index 0 for it. If
    // we, e.g., had vertices stored in one buffer and color in
    // another, we would use different binding sites (indices) for
    // each. The bindingIndex is the "input" to the VAO.
    const GLuint bindingIndex = 0;
    // Specify the data layout for binding site 0 of this VAO; where
    // does the raw data come from? The buffer (m_vBuffer), the start
    // offset (0), and the stride (sizeof(Vec2)), i.e., the number of
    // bytes between consecutive vertices.
    glVertexArrayVertexBuffer(m_vao, bindingIndex, m_vBuffer, 0, sizeof(Vec2));

    // The attribIndex is the "output" of the VAO and is the input to
    // the vertex shader
    //
    // This example only has one attribute; vPosition that will
    // receive the position data. Other examples may have color,
    // normals, etc.
    GLint attribIndex = m_shader->attributeLocation("vPosition");

    if (attribIndex < 0) {
        throw std::runtime_error("Could not get attribute index of vPosition");
    }
    
    // The attribute named vPosition of this VAO should get its data
    // from the buffer attached to binding site 0.
    glVertexArrayAttribBinding(m_vao, attribIndex, bindingIndex);

    // Attribute 0 has 2 components each of type GL_FLOAT, not to be
    // normalized (GL_FALSE). It starts at byte offset 0 within each
    // vertex.
    glVertexArrayAttribFormat(m_vao, attribIndex, 2, GL_FLOAT, GL_FALSE, 0);

    // Turn on the use of this attribute. Otherwise, the vertex shader
    // would never see it.
    glEnableVertexArrayAttrib(m_vao, attribIndex);
    
    // Specify the element buffer for the VAO. It supplies the vertex
    // numbers for indexed draws. The grouping, usage, and type of the
    // elements in m_iBuffer is specified when calling glDrawElements().
    glVertexArrayElementBuffer(m_vao, m_iBuffer);
}

// Render object
void GeometryRender::display()
{
    if (!m_shader)
        throw std::logic_error("GeometryRender::initialize() must "
		"be called before display()");

    // Make the correct context current.
    makeCurrent();

    // Fill color and depth buffers with the "clear" values.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select shader program and plumbing (VAO) to use.
    m_shader->use();
    glBindVertexArray(m_vao);

    // Draw primitives as triangles (GL_TRIANGLES). Read
    // indices.size() indices in total, i.e., indices.size()/3
    // triangles. The indices are of type GL_UNSIGNED_INT and are read
    // from the element buffer of the currently bound VAO, starting at
    // byte offset 0.
    const size_t byteOffset = 0 * sizeof(unsigned int);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
	GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset));

    // Unbind the VAO and program to make failures loud.
    glBindVertexArray(0);
    glUseProgram(0);

}
