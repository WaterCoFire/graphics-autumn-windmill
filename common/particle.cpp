#include "particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm> // For std::sort

// A simple helper function to get a random float
float rand_float(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

ParticleSystem::ParticleSystem(unsigned int maxParticles, GLuint shader, GLuint texture)
    : max_particles(maxParticles), shader_id(shader), texture_id(texture) {

    particles.resize(max_particles);

    // The VBO containing the 4 vertices of the particles.
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // Start recording state into the VAO

    // --- 1. Set up the static quad vertex data (attribute 0) ---
    glGenBuffers(1, &vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(0, 0); // This attribute is not instanced

    // --- 2. Set up the instanced particle position/size buffer (attribute 1) ---
    glGenBuffers(1, &vbo_particle_pos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_particle_pos);
    // Initialize with an empty buffer, we'll upload data each frame
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(1, 1); // This attribute is instanced

    // --- 3. Set up the instanced particle color buffer (attribute 2) ---
    glGenBuffers(1, &vbo_particle_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_particle_color);
    // Initialize with an empty buffer, we'll upload data each frame
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(2, 1); // This attribute is instanced

    glBindVertexArray(0); // Stop recording state into the VAO

    // Get uniform locations from the particle shader
    glUseProgram(shader_id);
    view_loc = glGetUniformLocation(shader_id, "view");
    projection_loc = glGetUniformLocation(shader_id, "projection");
    texture_sampler_loc = glGetUniformLocation(shader_id, "particleTexture");
}

ParticleSystem::~ParticleSystem() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo_quad);
    glDeleteBuffers(1, &vbo_particle_pos);
    glDeleteBuffers(1, &vbo_particle_color);
}

// Finds an unused particle in the container.
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
    return 0; // All particles are taken, override the first one
}

void ParticleSystem::spawnParticle(Particle &p) {
    p.life = rand_float(2.0f, 4.0f); // Particle lives for 2-4 seconds
    p.pos = glm::vec3(-10.0f, 15.1f, -30.0f); // Spawn at the top of the chimney

    // Give it a random initial velocity to spread out
    float spread = 1.5f;
    glm::vec3 maindir = glm::vec3(0.0f, 2.0f, 0.0f); // Smoke rises
    glm::vec3 randomdir = glm::vec3(
        rand_float(-spread, spread),
        rand_float(-spread, spread),
        rand_float(-spread, spread)
    );
    p.speed = maindir + randomdir * 0.2f;

    // Give it a random color (grayscale for smoke)
    unsigned char r = rand() % 50 + 205; // 205-255
    p.color = glm::vec4(r / 255.0f, r / 255.0f, r / 255.0f, 1.0f);

    p.size = rand_float(0.5f, 1.5f);
}

void ParticleSystem::update(float deltaTime, int newParticles, glm::vec3 cameraPosition) {
    // Add new particles
    for (int i = 0; i < newParticles; i++) {
        int particleIndex = findUnusedParticle();
        spawnParticle(particles[particleIndex]);
    }

    // Simulate all particles
    int particles_count = 0;
    for (int i = 0; i < max_particles; i++) {
        Particle &p = particles[i];

        if (p.life > 0.0f) {
            // Decrease life
            p.life -= deltaTime;
            if (p.life > 0.0f) {
                // Simulate simple physics
                p.speed += glm::vec3(0.0f, 0.5f, 0.0f) * deltaTime * 0.5f; // Smoke accelerates upwards slightly
                p.pos += p.speed * deltaTime;
                glm::vec3 toCamera = p.pos - cameraPosition;
                p.cameradistance = glm::dot(toCamera, toCamera);

                // Fade out particles
                p.color.a = p.life / 2.0f; // Fade out over 2 seconds
                if (p.color.a > 1.0f) p.color.a = 1.0f;
            } else {
                // Particle just died
                p.cameradistance = -1.0f;
            }
            particles_count++;
        }
    }

    std::sort(particles.begin(), particles.end());
}

void ParticleSystem::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    std::vector<glm::vec4> particle_positions_and_size;
    std::vector<glm::vec4> particle_colors;

    for(const auto& p : particles) {
        if(p.life > 0.0f) {
            particle_positions_and_size.push_back(glm::vec4(p.pos, p.size));
            particle_colors.push_back(p.color);
        }
    }

    if (particle_positions_and_size.empty()) {
        return; // Nothing to render
    }

    // --- Upload this frame's data to the GPU ---
    glBindBuffer(GL_ARRAY_BUFFER, vbo_particle_pos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particle_positions_and_size.size() * sizeof(glm::vec4), &particle_positions_and_size[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_particle_color);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particle_colors.size() * sizeof(glm::vec4), &particle_colors[0]);

    // --- Render ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // Don't write to depth buffer for transparent objects

    glUseProgram(shader_id);

    // Set uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(texture_sampler_loc, 0);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind the VAO that contains our complete rendering recipe
    glBindVertexArray(vao);

    // Draw all particles in a single call
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particle_positions_and_size.size());

    // --- Reset state ---
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}