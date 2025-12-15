#include "particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cstddef>

// Data structure for instanced rendering, matches layout in the shader
struct ParticleInstanceData {
    glm::vec4 posAndSize; // .xyz = position, .w = size
    glm::vec4 color;
};

// A more robust helper function to get a random float
float rand_float(float min, float max) {
    if (min > max) std::swap(min, max);
    return min + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min);
}

ParticleSystem::ParticleSystem(unsigned int maxParticles, GLuint shader, GLuint texture)
    : max_particles(maxParticles), shader_id(shader), texture_id(texture) {
    srand(time(nullptr));
    particles.resize(max_particles);

    for (auto& p : particles) {
        p.life = -1.0f;
    }

    static constexpr GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // --- 1. Static quad vertex data (attribute 0) ---
    glGenBuffers(1, &vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(nullptr));
    glVertexAttribDivisor(0, 0); // Not instanced

    // --- 2. Interleaved instanced data (attributes 1 and 2) ---
    glGenBuffers(1, &vbo_instanced_data);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_instanced_data);
    glBufferData(GL_ARRAY_BUFFER, max_particles * sizeof(ParticleInstanceData), nullptr, GL_STREAM_DRAW);

    // Attribute 1: Position (vec3) and Size (float)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData),
                          reinterpret_cast<void *>(offsetof(ParticleInstanceData, posAndSize)));
    glVertexAttribDivisor(1, 1); // Instanced

    // Attribute 2: Color (vec4)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData),
                          reinterpret_cast<void *>(offsetof(ParticleInstanceData, color)));
    glVertexAttribDivisor(2, 1); // Instanced

    glBindVertexArray(0);

    // Get uniform locations
    glUseProgram(shader_id);
    view_loc = glGetUniformLocation(shader_id, "view");
    projection_loc = glGetUniformLocation(shader_id, "projection");
    texture_sampler_loc = glGetUniformLocation(shader_id, "particleTexture");
}

ParticleSystem::~ParticleSystem() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo_quad);
    glDeleteBuffers(1, &vbo_instanced_data);
}

int ParticleSystem::findUnusedParticle() {
    for (int i = last_used_particle; i < max_particles; i++) {
        if (particles[i].life < 0) {
            last_used_particle = i;
            return i;
        }
    }
    for (int i = 0; i < last_used_particle; i++) {
        if (particles[i].life < 0) {
            last_used_particle = i;
            return i;
        }
    }
    return 0;
}

void ParticleSystem::spawnParticle(Particle &p) {
    // Lifetime: 2 seconds
    p.life = 2.0f;
    p.pos = glm::vec3(-10.0f, 15.0f, -30.0f); // Correct chimney top

    // A clear, consistent upward speed
    // Y-speed of 4 means it will travel 8 units up over its 2s lifetime
    glm::vec3 mainDir = glm::vec3(0.0f, 4.0f, 0.0f);
    // Very slight randomness for variation
    glm::vec3 randomDir = glm::vec3(
        rand_float(-0.3f, 0.3f),
        rand_float(-0.3f, 0.3f),
        rand_float(-0.3f, 0.3f)
    );
    p.speed = mainDir + randomDir;

    p.color = glm::vec4(1.0f); // Alpha will be controlled for fade-out
    p.size = rand_float(1.4f, 2.0f); // Size of the smoke, slightly varied
}

void ParticleSystem::update(float deltaTime, int newParticles, glm::vec3 cameraPosition) {
    for (int i = 0; i < newParticles; i++) {
        int particleIndex = findUnusedParticle();
        spawnParticle(particles[particleIndex]);
    }

    for (auto &p: particles) {
        if (p.life > 0.0f) {
            p.life -= deltaTime;
            if (p.life > 0.0f) {
                // Simple, constant velocity motion. No extra forces.
                p.pos += p.speed * deltaTime;

                glm::vec3 toCamera = p.pos - cameraPosition;
                p.cameraDistance = glm::dot(toCamera, toCamera);

                // Fade out based on its 2-second lifetime
                p.color.a = p.life / 2.0f;
            } else {
                p.cameraDistance = -1.0f;
            }
        }
    }
    std::sort(particles.begin(), particles.end());
}

void ParticleSystem::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) const {
    std::vector<ParticleInstanceData> instance_data;
    instance_data.reserve(max_particles);

    for (const auto &p: particles) {
        if (p.life > 0.0f) {
            instance_data.push_back({glm::vec4(p.pos, p.size), p.color});
        }
    }

    if (instance_data.empty()) return;

    // --- Simplified and Robust Data Upload ---
    glBindBuffer(GL_ARRAY_BUFFER, vbo_instanced_data);
    // Replace the entire buffer content with the new data for this frame.
    glBufferData(GL_ARRAY_BUFFER, instance_data.size() * sizeof(ParticleInstanceData), &instance_data[0],
                 GL_STREAM_DRAW);

    // --- Render ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glUseProgram(shader_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(texture_sampler_loc, 0);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instance_data.size());

    // --- Reset state ---
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
