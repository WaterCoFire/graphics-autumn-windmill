#ifndef GRAPHICS_ACTION_SCENE_GEOMETRY_H
#define GRAPHICS_ACTION_SCENE_GEOMETRY_H

#pragma once

#include <vector>
#include "glad/glad.h" // For GLuint

namespace Geometry {
    // === Tower (Quadrangular Frustum) ===
    static const std::vector towerVertices = {
        // Column 1,2,3 positions; 4,5,6 normals; 7,8 texture coords
        // Bottom face
        -2.0f, 0.0f, 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        -2.0f, 0.0f, -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        // Top face
        -1.0f, 10.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 10.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 10.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // Side faces - front
        -2.0f, 0.0f, 2.0f, 0.0f, 0.4472f, 0.8944f, 0.0f, 0.0f,
        2.0f, 0.0f, 2.0f, 0.0f, 0.4472f, 0.8944f, 1.0f, 0.0f,
        1.0f, 10.0f, 1.0f, 0.0f, 0.4472f, 0.8944f, 1.0f, 1.0f,
        -1.0f, 10.0f, 1.0f, 0.0f, 0.4472f, 0.8944f, 0.0f, 1.0f,

        // Side faces - right
        2.0f, 0.0f, 2.0f, 0.8944f, 0.4472f, 0.0f, 0.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.8944f, 0.4472f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, -1.0f, 0.8944f, 0.4472f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, 1.0f, 0.8944f, 0.4472f, 0.0f, 0.0f, 1.0f,

        // Side faces - back
        2.0f, 0.0f, -2.0f, 0.0f, 0.4472f, -0.8944f, 0.0f, 0.0f,
        -2.0f, 0.0f, -2.0f, 0.0f, 0.4472f, -0.8944f, 1.0f, 0.0f,
        -1.0f, 10.0f, -1.0f, 0.0f, 0.4472f, -0.8944f, 1.0f, 1.0f,
        1.0f, 10.0f, -1.0f, 0.0f, 0.4472f, -0.8944f, 0.0f, 1.0f,

        // Side faces - left
        -2.0f, 0.0f, -2.0f, -0.8944f, 0.4472f, 0.0f, 0.0f, 0.0f,
        -2.0f, 0.0f, 2.0f, -0.8944f, 0.4472f, 0.0f, 1.0f, 0.0f,
        -1.0f, 10.0f, 1.0f, -0.8944f, 0.4472f, 0.0f, 1.0f, 1.0f,
        -1.0f, 10.0f, -1.0f, -0.8944f, 0.4472f, 0.0f, 0.0f, 1.0f
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
    static const std::vector capVertices = {
        // Column 1,2,3 positions; 4,5,6 normals; 7,8 texture coords
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
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
