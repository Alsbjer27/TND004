#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <random>
#include <optional>
#include <filesystem>
#include <algorithm>
#include <numeric>

#include <particlesystem/particle.h>
#include <particlesystem/collisionsystem.h>

#include <rendering/window.h>

#include <fmt/format.h>

using namespace particlesystem;

/**
 * To test
 */
void test4PriorityQueue();

/**
 * Read particles for the simulation from file
 */
std::vector<Particle> read_particles(const std::filesystem::path& file);

/**
 * To run the simulation
 */
void runSimulation();

int main() {
#ifdef TEST_PRIORITY_QUEUE
    test4PriorityQueue();
#else
    runSimulation();
#endif
}

/**
 * Read particles for the simulation from file
 */
std::vector<Particle> read_particles(const std::filesystem::path& file) {
    std::ifstream is(file);
    if (!is) {
        return {};
    }

    int n_particles;
    is >> n_particles;  // read number of particles

    std::vector<Particle> particles;
    particles.reserve(n_particles);

    double rx, ry;
    double vx, vy;
    double radius;
    double mass;
    float r, g, b;
    for (int i = 0; i < n_particles; ++i) {
        is >> rx >> ry >> vx >> vy;
        is >> radius >> mass;
        is >> r >> g >> b;
        particles.push_back(Particle{.r = {rx, ry},
                                     .v = {vx, vy},
                                     .radius = radius,
                                     .mass = mass,
                                     .color = {r / 255.0f, g / 255.0f, b / 255.0f}});
    }
    return particles;
}

void runSimulation() {
    /*
    * billiards10.txt, diffusion.txt, sam4.txt, brownian.txt, sam4.txt
    * against-each-other.txt, newton-pendulum.txt, standing-stll.txt
    */
    std::cout << "Particles file (complete path): ";
    std::string name;
    std::cin >> name;
    
    std::filesystem::path particlesFile = name;
    auto theParticles = read_particles(particlesFile);

    if (std::size(theParticles) == 0) {
        fmt::print("No particles\n");
        return;
    }

    // create collision system
    CollisionSystem system{std::move(theParticles)};

    // Some initializations for rendering
    rendering::Window window(850, 850, rendering::Window::UseVSync::No);
    system.renderCallback = [&](std::span<Particle> particles) {
        window.beginFrame();
        window.clear({0, 0, 0, 1});
        window.drawParticles(particles);
        window.endFrame();
    };
    system.abortCallback = [&]() { return window.shouldClose(); };

    fmt::print("Simulation starts ...\n");
    system.simulate(10000, 10);  // simulate
    fmt::print("Simulation ends ...\n");
}

/**
 * To test
 */
void test4PriorityQueue() {
    /*constexpr int minItem = 10000;
    constexpr int maxItem = 99999;*/
    constexpr int minItem = 1000;
    constexpr int maxItem = 9999;
    PriorityQueue<int> h;

    fmt::print("Test: insert, deleteMin, isMinHeap\n");

    // std::vector<int> V(89999, 0);
    std::vector<int> V(8999, 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::iota(V.begin(), V.end(), minItem);
    std::shuffle(V.begin(), V.end(), g);

    for (int k : V) {
        fmt::print("Inserting {}\n", k);
        h.insert(k);
    }

    fmt::print("\n\n");

    for (int i = minItem; i < maxItem; ++i) {
        int x = h.deleteMin();
        fmt::print("{} deleted from queue\t\t Queue Size: {:10}\n", x, h.size());
        if (x != i) {
            fmt::print("Oops! Error after delete of {}\n", i);
        }
    }
    fmt::print("Successful test...\n");
}
