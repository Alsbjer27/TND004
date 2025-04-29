#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <span>
#include <functional>

//#define USE_PRIORITY_QUEUE_VECTOR

#ifdef USE_PRIORITY_QUEUE_VECTOR
    #include <particlesystem/priorityqueue-vector.h>
#else
    #include <particlesystem/priorityqueue.h>
#endif

#include <particlesystem/event.h>
#include <particlesystem/particle.h>

namespace particlesystem {

/**
 *  CollisionSystem class represents a collection of particles
 *  moving in the unit box, according to the laws of elastic collision.
 *  This event-based simulation relies on a priority queue.
 */
class CollisionSystem {
public:
    /**
     * Constructor to create a system with the specified collection of particles
     * The individual particles will be mutated during the simulation
     */
    CollisionSystem(std::vector<Particle> particles);

    // Disable copying
    CollisionSystem(const CollisionSystem&) = delete;
    CollisionSystem& operator=(const CollisionSystem&) = delete;

    /**
     * Simulate the system of particles for the specified amount of simulationTime
     * renderFrequenzy is the number of times the particles are rendered per time unit
     */
    void simulate(double simulationTime, double renderFrequenzy);

    /**
     * Returns the kinetic energy of the particles system
     */
    double kineticEnergy() const;

    /**
     * Return a vector with all system particles
     */
    const std::vector<Particle>& particles() const;

    // To be used by for rendering
    std::function<void(std::span<Particle>)> renderCallback;
    std::function<bool()> abortCallback;

private:
    /**
     * Update priority queue with all new events for particle
     */
    void predict(PriorityQueue<Event>& queue, Particle& particle, double currentTime,
                 double simulationTime);

    std::vector<Particle> particles_;  // the particles
};

}  // namespace particlesystem
