#pragma once

#include <iostream>
#include <compare>

#include <particlesystem/particle.h>

namespace particlesystem {

class CollisionSystem;

/**
 *  An event during a particle collision simulation. Each event contains
 *  the time at which it will occur and the particles a and b involved.
 *  There are 4 types of events:
 *    -  a and b both null:      rendering event
 *    -  a null, b not null:     collision with vertical wall
 *    -  a not null, b null:     collision with horizontal wall
 *    -  a and b both not null:  binary collision between a and b
 *
 */
class Event {
public:
    /**
     * Constructor to create a new event to occur at time t involving two particles
     */
    explicit Event(double t = 0.0, Particle* ptrA = nullptr, Particle* ptrB = nullptr);

    /*
     * Overloaded three-way comparison operator: chronological comparison using time
     */
    // std::strong_ordering operator<=>(const Event& e) const;
    auto operator<=>(const Event& e) const { return time <=> e.time; }

    /**
     * To check whether any collision occurred between when event was created and now
     */
    bool isValid() const;

    friend CollisionSystem;

private:
    double time;          // time that event is scheduled to occur
    Particle* particleA;  // particle involved in event, possibly null
    Particle* particleB;  // particle involved in event, possibly null
    int countA;           // collision count at event creation
    int countB;           // collision count at event creation
};

/**
 * Constructor to create a new event to occur at time t involving two particles
 */
inline Event::Event(double t, Particle* ptrA, Particle* ptrB)
    : time{t}
    , particleA{ptrA}
    , particleB{ptrB}
    , countA{particleA != nullptr ? particleA->counter() : -1}
    , countB{particleB != nullptr ? particleB->counter() : -1} {}

/*
 * Overloaded three-way comparison operator: chronological comparison using time
 */
//inline std::strong_ordering Event::operator<=>(const Event& e) const {
//    if (time == e.time) {
//        return std::strong_ordering::equivalent;
//    }
//    if (time < e.time) {
//        return std::strong_ordering::less;
//    }
//    return std::strong_ordering::greater;
//}

/**
 * To check whether any collision occurred between when event was created and now
 */
inline bool Event::isValid() const {
    if (particleA != nullptr && particleA->counter() != countA) {
        return false;
    }
    if (particleB != nullptr && particleB->counter() != countB) {
        return false;
    }
    return true;
}

}  // namespace particlesystem
