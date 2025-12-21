#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <utility>
#include <vector>

/*
 * Vertex struct
 * A struct to hold all vertex attributes.
 */
struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // Texture Coordinates
    glm::vec2 TexCoords;
};

/*
 * Texture struct
 * A struct to hold texture data.
 * Used to manage different types of textures (diffuse, specular, etc.)
 */
struct Texture {
    GLuint id;
    std::string type; // e.g., "texture_diffuse", "texture_specular"
    std::string path; // Path of the texture, useful for caching
};

/*
 * Mesh Class
 * A mesh is a single drawable entity. A model can be composed of one or more meshes.
 * Handles the setup of vertex buffers (VBO, EBO) and the vertex array object (VAO),
 * as well as the drawing logic.
 */
class Mesh {
public:
    // Mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    // Constructor: takes vertices, indices, and textures to create a mesh
    Mesh(const std::vector<Vertex> &vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = std::move(indices);
        this->textures = std::move(textures);

        // Set the vertex buffers and its attribute pointers
        setupMesh();
    }

    // Render the mesh
    void draw(GLuint shaderProgram) const {
        // Bind the Vertex Array Object
        glBindVertexArray(VAO);
        // Draw the mesh using its indices
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
        // Unbind VAO after drawing
        glBindVertexArray(0);
    }

private:
    // Render data
    unsigned int VBO, EBO;

    // Initializes all the buffer objects/arrays
    void setupMesh() {
        // 1. Create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // 2. Bind VAO
        glBindVertexArray(VAO);

        // 3. Load data into vertex buffer (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // 4. Load data into element buffer (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // 5. Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));

        // 6. Unbind VAO
        glBindVertexArray(0);
    }
};
#endif
