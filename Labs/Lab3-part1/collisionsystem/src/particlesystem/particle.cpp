#include <particlesystem/particle.h>

#include <cmath>

namespace particlesystem {


/**
 * Updates the velocities of this particle and the specified particle according
 * to the laws of elastic collision. Assumes that the particles are colliding
 * at this instant
 */
void Particle::bounceOff(Particle& that) {
    const auto dr = that.r - r;
    const auto dv = that.v - v;
    const double dvdr = glm::dot(dv, dr);      // dv dot dr
    const double dist = radius + that.radius;  // distance between particle centers at collision

    // magnitude of normal force
    const double magnitude = 2.0 * mass * that.mass * dvdr / ((mass + that.mass) * dist);

    // normal force
    const auto f = magnitude * dr / dist;

    // update velocities according to normal force
    v += f / mass;
    that.v -= f / that.mass;

    // update collision counts
    count++;
    that.count++;
}

/**
 * Updates the velocity of this particle upon collision with a vertical
 * wall (by reflecting the velocity in the x-direction).
 * Assumes that the particle is colliding with a vertical wall at this instant.
 */
void Particle::bounceOffVerticalWall() {
    v.x = -v.x;
    count++;
}

/**
 * Updates the velocity of this particle upon collision with a horizontal
 * wall (by reflecting the velocity in the y-direction)
 * Assumes that the particle is colliding with a horizontal wall at this instant.
 */
void Particle::bounceOffHorizontalWall() {
    v.y = -v.y;
    count++;
}

}  // namespace particlesystem
