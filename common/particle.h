#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <glm/glm.hpp>
#include "glad.h"

// Represents a single particle's state on the CPU
struct Particle {
    glm::vec3 pos, speed;
    glm::vec4 color;
    float size;
    float life; // Remaining life of the particle; if < 0 particle is dead
    float cameraDistance; // Squared distance to the camera. if dead: -1.0f

    bool operator<(const Particle &that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameraDistance > that.cameraDistance;
    }
};

class ParticleSystem {
public:
    ParticleSystem(unsigned int maxParticles, GLuint shader, GLuint texture);

    ~ParticleSystem();

    void update(float deltaTime, int newParticles, glm::vec3 cameraPosition);

    void render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) const;

private:
    static void spawnParticle(Particle &particle);

    int findUnusedParticle();

    std::vector<Particle> particles;
    int max_particles;
    int last_used_particle = 0;

    // OpenGL handles
    GLuint vao;
    GLuint vbo_quad; // VBO for the quad's vertices
    GLuint vbo_instanced_data; // VBO for the per-particle data (pos, size, color)

    // Shader uniform locations
    GLuint view_loc;
    GLuint projection_loc;
    GLuint texture_sampler_loc;
    GLuint shader_id;
    GLuint texture_id;
};

#endif // PARTICLE_H
