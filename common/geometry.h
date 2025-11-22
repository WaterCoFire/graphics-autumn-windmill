#ifndef GRAPHICS_ACTION_SCENE_GEOMETRY_H
#define GRAPHICS_ACTION_SCENE_GEOMETRY_H

#pragma once

#include <vector>
#include "glad/glad.h" // For GLuint

namespace Geometry {
    // === Tower (Quadrangular Frustum) ===
    // Position (x, y, z) + Normal (nx, ny, nz)
    // Prism - height 10.0, top face side length 1.0, bottom face side length 2.0
    static const std::vector towerVertices = {
        // Vertex position (x, y, z) + normal vector (nx, ny, nz)
        // Bottom (Y = 0.0, Normal 0,-1,0)
        -2.0f, 0.0f, -2.0f, 0.0f, -1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f, -1.0f, 0.0f,
        2.0f, 0.0f, 2.0f, 0.0f, -1.0f, 0.0f,
        -2.0f, 0.0f, 2.0f, 0.0f, -1.0f, 0.0f,

        // Top (Y = 10.0, Normal 0,1,0)
        -1.0f, 10.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 10.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        // Side Z+ (Normal 0, 0.0995, 0.995)
        -2.0f, 0.0f, 2.0f, 0.0f, 0.0995f, 0.995f,
        2.0f, 0.0f, 2.0f, 0.0f, 0.0995f, 0.995f,
        1.0f, 10.0f, 1.0f, 0.0f, 0.0995f, 0.995f,
        -1.0f, 10.0f, 1.0f, 0.0f, 0.0995f, 0.995f,

        // Side Z- (Normal 0, 0.0995, -0.995)
        -2.0f, 0.0f, -2.0f, 0.0f, 0.0995f, -0.995f,
        2.0f, 0.0f, -2.0f, 0.0f, 0.0995f, -0.995f,
        1.0f, 10.0f, -1.0f, 0.0f, 0.0995f, -0.995f,
        -1.0f, 10.0f, -1.0f, 0.0f, 0.0995f, -0.995f,

        // Side X- (Normal -0.995, 0.0995, 0)
        -2.0f, 0.0f, -2.0f, -0.995f, 0.0995f, 0.0f,
        -2.0f, 0.0f, 2.0f, -0.995f, 0.0995f, 0.0f,
        -1.0f, 10.0f, 1.0f, -0.995f, 0.0995f, 0.0f,
        -1.0f, 10.0f, -1.0f, -0.995f, 0.0995f, 0.0f,

        // Side X+ (Normal 0.995, 0.0995, 0)
        2.0f, 0.0f, -2.0f, 0.995f, 0.0995f, 0.0f,
        2.0f, 0.0f, 2.0f, 0.995f, 0.0995f, 0.0f,
        1.0f, 10.0f, 1.0f, 0.995f, 0.0995f, 0.0f,
        1.0f, 10.0f, -1.0f, 0.995f, 0.0995f, 0.0f
    };

    static const std::vector<GLuint> towerIndices = {
        0, 1, 2, 0, 2, 3, // Bottom
        4, 5, 6, 4, 6, 7, // Top
        8, 9, 10, 8, 10, 11, // Side Z+
        12, 13, 14, 12, 14, 15, // Side Z-
        16, 17, 18, 16, 18, 19, // Side X-
        20, 21, 22, 20, 22, 23 // Side X+
    };

    // === Cap (Cube) ===
    // Position (x, y, z) + Normal (nx, ny, nz)
    static const std::vector capVertices = {
        // Front
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // Back
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        // Left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        // Right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        // Bottom
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        // Top
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    static const std::vector<GLuint> capIndices = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    // === Blades (Quad) ===
    // Position (x, y, z) + Normal (nx, ny, nz)
    static const std::vector bladeVertices = {
        -0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom left
        0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom right
        0.2f, 3.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Top left
        -0.2f, 3.5f, 0.0f, 0.0f, 0.0f, 1.0f // Top right
    };

    static const std::vector<GLuint> bladeIndices = {
        0, 1, 2, 0, 2, 3
    };

    // === Ground (Large Quad Plane) ===
    // Position (x, y, z) + Normal (nx, ny, nz)
    static const std::vector groundVertices = {
        -1000.0f, 0.0f, -1000.0f, 0.0f, 1.0f, 0.0f, // Bottom left
        1000.0f, 0.0f, -1000.0f, 0.0f, 1.0f, 0.0f, // Bottom right
        1000.0f, 0.0f, 1000.0f, 0.0f, 1.0f, 0.0f, // Top right
        -1000.0f, 0.0f, 1000.0f, 0.0f, 1.0f, 0.0f // Top left
    };

    static const std::vector<GLuint> groundIndices = {
        0, 1, 2,
        0, 2, 3
    };
}

#endif //GRAPHICS_ACTION_SCENE_GEOMETRY_H