#include <particlesystem/collisionsystem.h>

#include <cassert>
#include <span>
#include <numeric>
#include <fmt/format.h>

namespace particlesystem {

namespace {

/**
 * Help function to add a new event between particleA and particleB to the queue
 * The event's time must be smaller than simulationTime to be added to the queue
 */
void addEvent(double time, Particle* particleA, Particle* particleB, PriorityQueue<Event>& queue,
              double simulationTime) {
    if (time < simulationTime) {
        queue.insert(Event{time, particleA, particleB});
    }
}

}  // namespace

/**
 * Constructor to create a system with the specified collection of particles
 * The individual particles will be mutated during the simulation
 */
CollisionSystem::CollisionSystem(std::vector<Particle> particles)
    : particles_{std::move(particles)} {}

/**
 * Update priority queue with all new events for particle
 */
void CollisionSystem::predict(PriorityQueue<Event>& queue, Particle& particle, double currentTime,
                              double simulationTime) {
    // particle-particle collisions
    for (auto& p : particles_) {
        const double dt = particle.timeToHit(p);
        addEvent(currentTime + dt, &particle, &p, queue, simulationTime);
    }

    // particle-wall collisions
    const double dtX = particle.timeToHitVerticalWall();
    addEvent(currentTime + dtX, &particle, nullptr, queue, simulationTime);

    const double dtY = particle.timeToHitHorizontalWall();
    addEvent(currentTime + dtY, nullptr, &particle, queue, simulationTime);
}

void CollisionSystem::simulate(double simulationTime, double drawFrequenzy) {
    PriorityQueue<Event> queue;  // the priority queue
    double currentTime = 0.0;    // initialize simulation clock time

    // add first redraw event to the queue
    addEvent(0.0, nullptr, nullptr, queue, simulationTime);

    // add all possible collisions of particle with other particles and walls to the queue
    for (auto& particle : particles_) {
        predict(queue, particle, currentTime, simulationTime);
    }

    // the main event-driven simulation loop
    while (!queue.isEmpty()) {
        // get impending event, discard if invalidated
        const Event e = queue.deleteMin();
        if (!e.isValid()) {
            continue;
        }

        Particle* particleA = e.particleA;  // pointer to particle A
        Particle* particleB = e.particleB;  // pointer to particle B

        // update particles positions
        for (auto& p : particles_) {
            p.move(e.time - currentTime);
        }
        currentTime = e.time;  // update simulation clock

        // process event: update velocity, if needed
        if (particleA != nullptr && particleB != nullptr) {
            particleA->bounceOff(*particleB);  // particle-particle collision
            predict(queue, *particleA, currentTime, simulationTime);
            predict(queue, *particleB, currentTime, simulationTime);
        } else if (particleA != nullptr && particleB == nullptr) {
            particleA->bounceOffVerticalWall();  // particle-horizontal wall collision
            predict(queue, *particleA, currentTime, simulationTime);
        } else if (particleA == nullptr && particleB != nullptr) {
            particleB->bounceOffHorizontalWall();  // particle-vertical wall collision
            predict(queue, *particleB, currentTime, simulationTime);
        } else if (particleA == nullptr && particleB == nullptr) {
            renderCallback(particles_);

            // add another rendering event to the queue
            addEvent(currentTime + 1.0 / drawFrequenzy, nullptr, nullptr, queue, simulationTime);

            // fmt::print("Simulation Time: {:8.3f}, Queue Size: {:10}\n", currentTime, queue.size());

           if (abortCallback()) break; // in case user closes the simulation window
        }
    }
}

 /**
 * Return a vector with all system particles
 */
const std::vector<Particle>& CollisionSystem::particles() const { return particles_; }

/**
 * Returns the kinetic energy of the particles system
 */
double CollisionSystem::kineticEnergy() const {
    return std::transform_reduce(particles_.begin(), particles_.end(), 0.0, std::plus<>{},
                                 [](const auto& p) { return p.kineticEnergy(); });
}

}  // namespace particlesystem
