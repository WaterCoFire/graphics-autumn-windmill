#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <glm/glm.hpp>
#include "glad.h"

// Represents a single particle in the system
struct Particle {
    glm::vec3 pos, speed;
    glm::vec4 color;
    float size, angle, weight;
    float life; // Remaining life of the particle. if < 0, means particle is dead.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
};

class ParticleSystem {
public:
    ParticleSystem(unsigned int maxParticles, GLuint shader, GLuint texture);
    ~ParticleSystem();

    void update(float deltaTime, int newParticles, glm::vec3 cameraPosition);
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
    void spawnParticle(Particle& particle);
    int findUnusedParticle();

    std::vector<Particle> particles;
    GLuint vao, vbo_quad, vbo_particle_pos, vbo_particle_color;
    GLuint shader_id;
    GLuint texture_id;
    int max_particles;
    int last_used_particle = 0;

    // Uniform locations
    GLint view_loc;
    GLint projection_loc;
    GLint texture_sampler_loc;
};

#endif // PARTICLE_H