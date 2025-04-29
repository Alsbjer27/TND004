#pragma once

#include <iostream>
#include <fstream>
#include <limits>  // infinity()
#include <glm/glm.hpp>

namespace particlesystem {

/**
 * To represent RGB-color
 */
struct Color : glm::vec3 {
    using glm::vec3::vec3;
};

/**
 *  The Particle class represents a particle moving in the unit box,
 *  with a given position, velocity, radius, and mass.
 *  Member functions are provided for moving the particle
 *  and for predicting and resolving elastic collisions with vertical walls,
 *  horizontal walls, and other particles.
 *  This data type is mutable because the position and velocity change.
 */
struct Particle {
    /**
     * Move this particle in a straight line (based on its velocity)
     * for the specified amount of time dt
     */
    void move(double dt) { r += v * dt; }

    /**
     * Returns the number of collisions involving this particle with
     * vertical walls, horizontal walls, or other particles.
     */
    int counter() const { return count; }

    /**
     * Returns the amount of time for this particle to collide with 'that' specified
     * particle
     * Return std::numeric_limits<double>::infinity(), if the particles will not collide
     * Assume particles don't collide with themselves
     */
    double timeToHit(const Particle& that) const;

    /**
     * Returns the amount of time for this particle to collide with a vertical
     * wall
     * Return std::numeric_limits<double>::infinity(), if the particle will not collide with a
     * vertical wall
     */
    double timeToHitVerticalWall() const;

    /**
     * Returns the amount of time for this particle to collide with a horizontal
     * wall
     * Return std::numeric_limits<double>::infinity(), if the particle will not collide with a
     * horizontal wall
     */
    double timeToHitHorizontalWall() const;

    /**
     * Updates the velocities of this particle and the specified particle according
     * to the laws of elastic collision. Assumes that the particles are colliding
     * at this instant
     */
    void bounceOff(Particle& that);

    /**
     * Updates the velocity of this particle upon collision with a vertical
     * wall (by reflecting the velocity in the x-direction)
     * Assumes that the particle is colliding with a vertical wall at this instant.
     */
    void bounceOffVerticalWall();

    /**
     * Updates the velocity of this particle upon collision with a horizontal
     * wall (by reflecting the velocity in the y-direction)
     * Assumes that the particle is colliding with a horizontal wall at this instant.
     */
    void bounceOffHorizontalWall();

    /**
     * Returns the kinetic energy of this particle
     * The kinetic energy is given by the formula 1/2 * m * v^2,
     * where m is the mass of this particle and v is its velocity.
     */
    double kineticEnergy() const { return 0.5 * mass * glm::dot(v, v); }

    glm::dvec2 r = {0, 0};          // position
    glm::dvec2 v = {0, 0};          // velocity
    double radius = 0.01;           // radius
    double mass = 0.01;             // mass
    Color color = {1.0, 1.0, 1.0};  // color
    int count = 0;                  // number of collisions so far
};

/**
 * Returns the amount of time for this particle to collide with 'that' specified
 * particle
 * Return std::numeric_limits<double>::infinity(), if the particles will not collide
 */
inline double Particle::timeToHit(const Particle& that) const {
    if (this == &that) {  // particle colliding with itself?
        return std::numeric_limits<double>::infinity();
    }

    const auto dr = that.r - r;
    const auto dv = that.v - v;

    const double dvdr = glm::dot(dr, dv);
    if (dvdr > 0) {
        return std::numeric_limits<double>::infinity();
    }

    const double dvdv = glm::dot(dv, dv);
    if (dvdv == 0.0) {
        return std::numeric_limits<double>::infinity();
    }

    const double drdr = glm::dot(dr, dr);
    const double sigma = radius + that.radius;
    if (drdr < sigma * sigma) {
        return std::numeric_limits<double>::infinity();
    }

    const double d = (dvdr * dvdr) - dvdv * (drdr - sigma * sigma);
    if (d < 0.0) {
        return std::numeric_limits<double>::infinity();
    }

    const auto time = -(dvdr + std::sqrt(d)) / dvdv;
    if (time < 0.0) {
        return std::numeric_limits<double>::infinity();
    }

    return time;
}

/**
 * Returns the amount of time for this particle to collide with a vertical
 * wall
 * Return std::numeric_limits<double>::infinity(), if the particle will not collide with a
 * vertical wall
 */
inline double Particle::timeToHitVerticalWall() const {
    if (v.x > 0) {
        return (1.0 - r.x - radius) / v.x;
    } else if (v.x < 0) {
        return (radius - r.x) / v.x;
    } else {
        return std::numeric_limits<double>::infinity();
    }
}

/**
 * Returns the amount of time for this particle to collide with a horizontal
 * wall
 * Return std::numeric_limits<double>::infinity(), if the particle will not collide with a
 * horizontal wall
 */
inline double Particle::timeToHitHorizontalWall() const {
    if (v.y > 0) {
        return (1.0 - r.y - radius) / v.y;
    } else if (v.y < 0) {
        return (radius - r.y) / v.y;
    } else {
        return std::numeric_limits<double>::infinity();
    }
}

}  // namespace particlesystem
