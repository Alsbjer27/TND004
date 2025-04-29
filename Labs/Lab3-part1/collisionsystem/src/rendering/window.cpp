#include <rendering/window.h>

#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/common.hpp>
#include <glm/packing.hpp>

// Dear students:
// if you have found your way here, rest assured that understanding the rest of this file
// is *not* required to complete the lab. This file contains a lot of implementation for
// the rendering helper functionalities. Just have a look at the corresponding header file
// "rendering.h" and look at the structs and functions that are exposed there. They are
// all documented so that looking at the source code should not be necessary.
// Having said that, if you are interested in anything, of course continue browsing here

constexpr size_t VBO_CAP = 1024 * 1024;

// Internal definition of window implementation
struct rendering::Window::Impl {
    Impl(int width, int height, UseVSync sync);
    ~Impl();

    GLFWwindow* window;

    GLuint program;
    GLuint vao;
    GLuint vbo;
};

namespace {

// This structure represents how the points is stored in the vertex buffer
struct Point {
    glm::vec2 position;
    float scale;
    uint32_t color_packed;
};

/**
 * Checks the compilation status of the shader passed into it and prints out a message in
 * case the shader was not compiled successfully. If the shader is successfully compiled,
 * no message is printed. This function does **not** compile the shader.
 * This function only performs a check if it is compiled in Debug mode. In release builds
 * these checks will be omitted.
 *
 * \param shader The GL object of the shader object that should be checked
 * \param name The human-readable name of the shader object used in the printing
 *
 * \throw std::runtime_error An exception is raised if the compilation of the shader
 *        failed
 * \pre \p shader must be a valid GL shader object
 * \pre \p name must not be empty
 * \post Only the compile status of \p shader might be modified
 */
bool checkShader([[maybe_unused]] GLuint shader, [[maybe_unused]] std::string_view name) {

    assert(shader != 0);
    assert(!name.empty());

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> buf;
        buf.resize(static_cast<size_t>(logLength));
        glGetShaderInfoLog(shader, logLength, NULL, buf.data());

        fmt::print("Error compiling shader: {}\n{}", name, buf.data());
    }

    return status == GL_TRUE;
}

/**
 * Checks the linking status of the passed shader program passed into it and prints out a
 * message in case the program was not linked successfully. If the program was linked
 * successfully, no message is printed. This function does **not** link the program.
 * This function only performs a check if it is compiled in Debug mode. In release builds
 * these checks will be omitted.
 *
 * \param program The shader program that is to be checked
 * \param name The human-readable name of the shader program used in the printing
 *
 * \throw std::runtime_error An exception is raised if the linking of the program failed
 *
 * \pre \p program must be a valid GL program object
 * \pre \p name must not be empty
 * \post Only the link status of \p program might be modified
 */
bool checkProgram([[maybe_unused]] GLuint program, [[maybe_unused]] std::string_view name) {

    assert(program != 0);
    assert(!name.empty());

    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> buf;
        buf.resize(static_cast<size_t>(logLength));
        glGetProgramInfoLog(program, logLength, NULL, buf.data());

        fmt::print("Error linking program: {}\n{}", name, buf.data());
    }

    return status == GL_TRUE;
}

/**
 * Checks whether any of the GL functions called since the last call to this function have
 * raised any errors. If there were multiple errors, only the first error will be printed.
 * If there haven't been any errors, no message is printed.
 *
 * \param name The human-readable name that is used in the logging
 *
 * \throw std::runtime_error An exception is raised if there was an OpenGL error
 *
 * \pre \p name must not be empty
 * \post The OpenGL error will be GL_NO_ERROR
 */
void checkOpenGLError([[maybe_unused]] std::string_view name) {
    GLenum e = glGetError();
    if (e != GL_NO_ERROR) {
        throw std::runtime_error(fmt::format("OpenGL error occurred ({}) {}", name, e));
    }
}

// Creates the shader program for points.
GLuint createPointProgram() {

    constexpr const char* vsSrc[1] = {R"(
        #version 330
        layout(location = 0) in vec2  in_position;
        layout(location = 1) in float in_scale;
        layout(location = 2) in vec4  in_color;

        out vec4 vs_color;

        void main() {
            vs_color = in_color;
            gl_PointSize = in_scale;
            gl_Position = vec4(in_position, 0.0, 1.0);
        }
    )"};

    constexpr const char* fsSrc[1] = {R"(
        #version 330
        in vec4 vs_color;
        out vec4 out_color;

        void main() {
            vec2 pos = gl_PointCoord - vec2(0.5, 0.5);
            if (length(pos) > 0.5) discard;
            
            out_color = vec4(vs_color);
        }
    )"};

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, vsSrc, nullptr);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, fsSrc, nullptr);
    glCompileShader(fragment);

    GLuint program = 0;
    if (checkShader(vertex, "point-vertex") && checkShader(fragment, "point-fragment")) {
        program = glCreateProgram();

        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        glLinkProgram(program);
        checkProgram(program, "point-program");

        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

}  // namespace

namespace rendering {

Window::Impl::Impl(int width, int height, UseVSync sync)
    : window{nullptr}, program{0}, vao{0}, vbo{0} {

    // Initialize GLFW for window handling
    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("Unable to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, "Particle System", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (sync == UseVSync::No) {
        glfwSwapInterval(0);
    }

    // Initialize the GLAD OpenGL wrapper
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    // Create GL objects
    program = createPointProgram();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Allocate vertex buffer memory
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, VBO_CAP * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);

    // Setup vertex attribute pointers for Points
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point),
                          reinterpret_cast<const void*>(offsetof(Point, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Point),
                          reinterpret_cast<const void*>(offsetof(Point, scale)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Point),
                          reinterpret_cast<const void*>(offsetof(Point, color_packed)));

    glBindVertexArray(0);

    checkOpenGLError("postInit");
}

Window::Impl::~Impl() {
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwDestroyWindow(window);

    // @NOTE: This is should be performed outside the lifetime of a single window
    glfwTerminate();
}

Window::Window(int width, int height, UseVSync sync)
    : impl(std::make_unique<Impl>(width, height, sync)) {}

Window::~Window() {}

double Window::time() const { return glfwGetTime(); }

bool Window::shouldClose() const { return glfwWindowShouldClose(impl->window); }

void Window::beginFrame() {
    checkOpenGLError("beginFrame");

    // Query the events from the operating system, such as input from mouse or keyboards
    glfwPollEvents();

    // Set the default OpenGL state
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    int width, height;
    glfwGetFramebufferSize(impl->window, &width, &height);

    // Update render window
    glViewport(0, 0, width, height);
}

void Window::clear(glm::vec4 color) {
    // Clear the rendering buffer with the selected background color
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::drawParticles(std::span<particlesystem::Particle> particles) {

    if (particles.size() > VBO_CAP) {
        throw std::runtime_error("Too many rectangles to draw in a single call");
    }

    int width, height;
    glfwGetFramebufferSize(impl->window, &width, &height);

    // Upload the passed particle information to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, impl->vbo);
    Point* point_data = static_cast<Point*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    if (point_data) {
        for (size_t i = 0; auto& p : particles) {
            point_data[i].position = static_cast<glm::vec2>(2.0 * (p.r - glm::dvec2{0.5f}));
            point_data[i].scale = static_cast<float>(p.radius * width * 2.0);
            point_data[i].color_packed = glm::packUnorm4x8(glm::vec4{p.color, 1.0f});
            ++i;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        throw std::runtime_error("Failed to map buffer");
    }

    glBindVertexArray(impl->vao);
    glUseProgram(impl->program);
    glDrawArrays(GL_POINTS, 0, static_cast<int>(particles.size()));
    glUseProgram(0);
    glBindVertexArray(0);

    checkOpenGLError("drawPoint");
}

void Window::endFrame() {
    // Swapping the front and back buffer. Since we are doing v-sync is enabled, this
    // call will block until its our turn to swap the buffers (usually every 16.6 ms
    // on a 60 Hz monitor
    glfwSwapBuffers(impl->window);

    checkOpenGLError("endFrame");
}

}  // namespace rendering
