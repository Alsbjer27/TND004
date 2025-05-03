#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string_view>
#include <span>

/// This namespace contains objects that are helper functions that support the rendering
/// of general ui elements and graphics primitives.
namespace rendering {

struct Point {
    glm::vec2 position{};
    glm::vec4 color{};
    float size{};
};

class Window {
public:
    enum class UseVSync { Yes, No };

    Window(int width, int height, UseVSync sync);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    ~Window();

    // Returns a time value in seconds since window was created
    double time() const;

    // Returns bool which signifies if the user has requested to close the window
    bool shouldClose() const;

    // Returns the current size of the window in pixels
    glm::vec2 size() const;

    // Sets up the initial frame state and must be called in the beginning of the frame. It
    // will poll and respond to all the keyboard and mouse input and set up the user interface
    // that can be called during the frame.
    void beginFrame();

    // Finalizes the current frame and swaps the front and back buffers for double buffering.
    void endFrame();

    // Clear the window with specific color, each channel is in range [0,1]
    void clear(glm::vec4 color);

    // Draws points on screen
    void drawPoints(std::span<const Point> points);

    // Draws lines on screen, each pair of points represents a line
    void drawLines(std::span<const Point> lines);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}  // namespace rendering
