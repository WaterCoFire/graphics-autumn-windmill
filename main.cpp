#include "wrapper_glfw.h"
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

// Function for loading vertex & fragment shaders
GLuint loadShader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR: " << &e << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // Create vertex shader and compile
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create fragment shader and compile
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader application and link it
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shader objects after compilation is complete
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return shaderProgram;
}

int main() {
    // GLFW initialization
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Set OpenGL version: 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Major version num
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // Minor version num

    // Use the "Core Profile"
    // This indicates enabling only modern OpenGL features, with no compatibility for legacy interfaces
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // macOS specific requirement: Must set "Forward Compatible"
    // Otherwise, it will crash due to Core Profile being disabled by default
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Windmill Action Scene", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD Initialization
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // Shaders
    GLuint program = loadShader("shader.vert", "shader.frag");
    glUseProgram(program);

    // Get uniform location
    GLint modelLoc = glGetUniformLocation(program, "model");
    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint projLoc = glGetUniformLocation(program, "proj");
    GLint normalMatLoc = glGetUniformLocation(program, "normalMat");
    GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
    GLint lightColorLoc = glGetUniformLocation(program, "lightColor");
    GLint objectColorLoc = glGetUniformLocation(program, "objectColor");
    GLint shininessLoc = glGetUniformLocation(program, "shininess");
    GLint ambientColorLoc = glGetUniformLocation(program, "ambientColor");

    // Controllable light
    glUniform3f(lightColorLoc, 1.0f, 0.5f, 0.1f);

    // Global ambient color
    glUniform3f(ambientColorLoc, 0.76f, 0.64f, 0.23f);

    // Shininess
    glUniform1f(shininessLoc, 32.0f);

    // === Tower (Quadrangular Frustum) ===
    // Prism - height 10.0, top face side length 1.0, bottom face side length 2.0
    std::vector towerVertexData = {
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

    std::vector<GLuint> towerIndices = {
        0, 1, 2, 0, 2, 3, // Bottom
        4, 5, 6, 4, 6, 7, // Top
        8, 9, 10, 8, 10, 11, // Side Z+
        12, 13, 14, 12, 14, 15, // Side Z-
        16, 17, 18, 16, 18, 19, // Side X-
        20, 21, 22, 20, 22, 23 // Side X+
    };

    GLuint towerVAO, towerVBO, towerEBO;

    glGenVertexArrays(1, &towerVAO);
    glGenBuffers(1, &towerVBO);
    glGenBuffers(1, &towerEBO);
    glBindVertexArray(towerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, towerVBO);
    glBufferData(GL_ARRAY_BUFFER, towerVertexData.size() * sizeof(float), towerVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, towerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, towerIndices.size() * sizeof(GLuint), towerIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // === End of Tower ===

    // === Cap (cube) ===
    std::vector capVertexData = {
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
    std::vector<GLuint> capIndices = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    GLuint capVAO, capVBO, capEBO;
    glGenVertexArrays(1, &capVAO);
    glGenBuffers(1, &capVBO);
    glGenBuffers(1, &capEBO);
    glBindVertexArray(capVAO);
    glBindBuffer(GL_ARRAY_BUFFER, capVBO);
    glBufferData(GL_ARRAY_BUFFER, capVertexData.size() * sizeof(float), capVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, capEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, capIndices.size() * sizeof(GLuint), capIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // === End of Cap ===

    // === Blades (quad) ===
    std::vector bladeVertexData = {
        -0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom left
        0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom right
        0.2f, 3.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Top left
        -0.2f, 3.5f, 0.0f, 0.0f, 0.0f, 1.0f // Top right
    };

    std::vector<GLuint> bladeIndices = {
        0, 1, 2, 0, 2, 3
    };

    GLuint bladeVAO, bladeVBO, bladeEBO;
    glGenVertexArrays(1, &bladeVAO);
    glGenBuffers(1, &bladeVBO);
    glGenBuffers(1, &bladeEBO);
    glBindVertexArray(bladeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bladeVBO);
    glBufferData(GL_ARRAY_BUFFER, bladeVertexData.size() * sizeof(float), bladeVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bladeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bladeIndices.size() * sizeof(GLuint), bladeIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // === End of Blade ===

    // === Ground (Large Quad Plane) ===
    // Position (x, y, z) + Normal (nx, ny, nz)
    std::vector groundVertexData = {
        -1000.0f, 0.0f, -1000.0f, 0.0f, 1.0f, 0.0f, // Bottom left
        1000.0f, 0.0f, -1000.0f, 0.0f, 1.0f, 0.0f, // Bottom right
        1000.0f, 0.0f, 1000.0f, 0.0f, 1.0f, 0.0f, // Top right
        -1000.0f, 0.0f, 1000.0f, 0.0f, 1.0f, 0.0f // Top left
    };

    std::vector<GLuint> groundIndices = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint groundVAO, groundVBO, groundEBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glGenBuffers(1, &groundEBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, groundVertexData.size() * sizeof(float), groundVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, groundIndices.size() * sizeof(GLuint), groundIndices.data(), GL_STATIC_DRAW);
    // Position Attribute (index 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // Normal Attribute (index 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // === End of Ground ==

    // === Hub Cylinder (in the center of 4 blades) ===
    std::vector<float> hubVertexData;
    std::vector<GLuint> hubIndices;
    const int segments = 16;
    const float radius = 0.3f;
    const float length = 0.5f;

    // Centers: X=0.0, Y=0.0
    // Front Center (Z = length/2)
    hubVertexData.insert(hubVertexData.end(), {0.0f, 0.0f, length / 2.0f, 0.0f, 0.0f, 1.0f}); // Index 0
    // Back Center (Z = -length/2)
    hubVertexData.insert(hubVertexData.end(), {0.0f, 0.0f, -length / 2.0f, 0.0f, 0.0f, -1.0f}); // Index 1

    // Perimeter vertices
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments);
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);

        // Front perimeter vertex (Z = length/2)
        hubVertexData.insert(hubVertexData.end(), {x, y, length / 2.0f, 0.0f, 0.0f, 1.0f});
        // Back perimeter vertex (Z = -length/2)
        hubVertexData.insert(hubVertexData.end(), {x, y, -length / 2.0f, 0.0f, 0.0f, -1.0f});
    }

    // Indices for caps
    for (int i = 0; i < segments; ++i) {
        GLuint next_i = (i + 1) % segments;
        GLuint front_curr = 2 + i * 2;
        GLuint front_next = 2 + next_i * 2;
        GLuint back_curr = 3 + i * 2;
        GLuint back_next = 3 + next_i * 2;

        // Front cap triangle fan (using index 0 as center)
        hubIndices.insert(hubIndices.end(), {0, front_curr, front_next});

        // Back cap triangle fan (using index 1 as center)
        hubIndices.insert(hubIndices.end(), {1, back_next, back_curr});
    }

    // Duplicating perimeter vertices for correct side normals
    GLuint sideStartIndex = hubVertexData.size() / 6;
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments);
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);

        // Side normal (perpendicular to Z-axis)
        float nx = std::cos(angle);
        float ny = std::sin(angle);

        GLuint next_i = (i + 1) % segments;

        // Side Quad vertices
        hubVertexData.insert(hubVertexData.end(), {x, y, length / 2.0f, nx, ny, 0.0f}); // front_curr_side
        hubVertexData.insert(hubVertexData.end(), {x, y, -length / 2.0f, nx, ny, 0.0f}); // back_curr_side

        // Quad indices
        GLuint curr_f = sideStartIndex + i * 2;
        GLuint curr_b = sideStartIndex + i * 2 + 1;
        GLuint next_f = sideStartIndex + next_i * 2;
        GLuint next_b = sideStartIndex + next_i * 2 + 1;

        // Triangle 1 - front_curr, front_next, back_curr
        hubIndices.insert(hubIndices.end(), {curr_f, next_f, curr_b});
        // Triangle 2 - back_curr, front_next, back_next
        hubIndices.insert(hubIndices.end(), {curr_b, next_f, next_b});
    }

    GLuint hubVAO, hubVBO, hubEBO;
    glGenVertexArrays(1, &hubVAO);
    glGenBuffers(1, &hubVBO);
    glGenBuffers(1, &hubEBO);
    glBindVertexArray(hubVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hubVBO);
    glBufferData(GL_ARRAY_BUFFER, hubVertexData.size() * sizeof(float), hubVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hubEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hubIndices.size() * sizeof(GLuint), hubIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // === End of Hub Cylinder ===

    // Display control tip in console
    std::cout << "Controls:\n";
    std::cout << "Camera: W/S/A/D/Q/E to move (forward/back/left/right/down/up)\n";
    std::cout << "Light: Arrow keys left/right/up/down for x/y, comma/period for z\n";
    std::cout << "Windmill body rotation speed: + to increase, - to decrease\n";
    std::cout << "Blade rotation speed: I to increase, K to decrease\n";
    std::cout << "R to reverse direction (press once to toggle)\n";
    std::cout << "P to pause/play windmill main body rotation\n";
    std::cout << "ESC to exit\n";

    // Variables (INITIAL)
    float lastTime = 0.0f; // Last frame time
    glm::vec3 cameraPos = glm::vec3(0.0f, 6.5f, 20.0f); // Camera position
    glm::vec3 lookAtPos = glm::vec3(0.0f, 6.5f, 0.0f); // Camera look at (observation point)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up direction
    glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 5.0f); // Light source position
    int direction = 1; // Rotate direction
    float mainBodyAngle = 0.0f; // Rotation angle of the windmill main body
    float bladeAngle = 0.0f; // Rotation angle of the blade
    float mainBodyRotationSpeed = 10.0f; // Windmill main body rotation speed, degrees/sec
    float bladeRotationSpeed = 60.0f; // Blade rotation speed, degrees/sec
    float moveSpeed = 10.0f; // Camera movement speed
    bool rPressed = false; // R key pressed signal (R key is used for reversing rotation direction)
    bool isBodyRotating = false; // Control variable for windmill main body rotation (by default not rotating)
    bool pPressed = false; // P key pressed signal (for pausing/resuming windmill body rotation)

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents(); // Handle events

        // ESC quit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos.z -= moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos.z += moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos.x -= moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos.x += moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraPos.y -= moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameraPos.y += moveSpeed * deltaTime;

        // Light movement
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lightPos.x -= moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lightPos.x += moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) lightPos.y += moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) lightPos.y -= moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) lightPos.z -= moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) lightPos.z += moveSpeed * deltaTime;

        // Cap rotation speed
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) mainBodyRotationSpeed += 50.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) mainBodyRotationSpeed -= 50.0f * deltaTime;
        if (mainBodyRotationSpeed < 0.0f) mainBodyRotationSpeed = 0.0f;
        if (mainBodyRotationSpeed > 500.0f) mainBodyRotationSpeed = 500.0f;

        // Blade rotation speed
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) bladeRotationSpeed += 100.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) bladeRotationSpeed -= 100.0f * deltaTime;
        if (bladeRotationSpeed < 0.0f) bladeRotationSpeed = 0.0f;
        if (bladeRotationSpeed > 1000.0f) bladeRotationSpeed = 1000.0f;

        // Direction control
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            if (!rPressed) {
                direction = -direction; // Reverse rotation direction
                rPressed = true;
            }
        } else {
            rPressed = false;
        }

        // Windmill main body rotation pause/resume control
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            if (!pPressed) {
                isBodyRotating = !isBodyRotating; // Toggle rotation state (on/off)
                pPressed = true;
            }
        } else {
            pPressed = false;
        }

        // Update angles
        // Windmill main body rotation state can be controlled
        if (isBodyRotating) {
            mainBodyAngle += static_cast<float>(direction) * mainBodyRotationSpeed * deltaTime;
        }
        bladeAngle += static_cast<float>(direction) * bladeRotationSpeed * deltaTime;

        mainBodyAngle = std::fmod(mainBodyAngle, 360.0f);
        bladeAngle = std::fmod(bladeAngle, 360.0f);

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, lookAtPos, up);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        // === Draw Windmill Main Body ===
        // Main body Part 1 - Tower (Quadrangular Frustum)
        glm::mat4 model = glm::mat4(1.0f);
        // Rotate the tower (tetrahedron) and cube together around the Y-axis
        model = glm::rotate(model, glm::radians(mainBodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
        glUniform3f(objectColorLoc, 0.5f, 0.5f, 0.5f);
        glBindVertexArray(towerVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(towerIndices.size()), GL_UNSIGNED_INT, nullptr);

        // Main body Part 2 - Cap (Cube)
        glm::mat4 capModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        capModel = glm::rotate(capModel, glm::radians(mainBodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        capModel = glm::scale(capModel, glm::vec3(1.5f, 1.0f, 1.5f));
        normalMat = glm::transpose(glm::inverse(glm::mat3(capModel)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(capModel));
        glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
        glUniform3f(objectColorLoc, 0.42f, 0.48f, 0.85f);
        glBindVertexArray(capVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(capIndices.size()), GL_UNSIGNED_INT, nullptr);
        // === Draw Windmill Main Body end ===

        // === Draw Blades ===
        glUniform3f(objectColorLoc, 0.35f, 0.3f, 0.85f);
        for (int i = 0; i < 4; ++i) {
            glm::mat4 bladeModel = capModel;
            // Translate to the center of the block's side, leaving a slight gap
            bladeModel = glm::translate(bladeModel, glm::vec3(0.0f, 0.0f, 1.05f));
            // Rotate the blade around the Z-axis
            bladeModel = glm::rotate(bladeModel, glm::radians(bladeAngle + static_cast<float>(i) * 90.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f));
            normalMat = glm::transpose(glm::inverse(glm::mat3(bladeModel)));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bladeModel));
            glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
            glBindVertexArray(bladeVAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(bladeIndices.size()), GL_UNSIGNED_INT, nullptr);
        }
        // === Draw Blades end ===

        // === Draw Hub Cylinder ===
        glm::mat4 hubModel = glm::mat4(1.0f);

        // Move the hub to the center of the cap's front face
        hubModel = glm::translate(hubModel, glm::vec3(0.0f, 10.0f, 1.5f));

        // Rotate the hub with the main body around the Y-axis
        hubModel = glm::rotate(hubModel, glm::radians(mainBodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        normalMat = glm::transpose(glm::inverse(glm::mat3(hubModel)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(hubModel));
        glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));

        // Hub cylinder color
        glUniform3f(objectColorLoc, 0.1f, 0.1f, 0.05f);

        glBindVertexArray(hubVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(hubIndices.size()), GL_UNSIGNED_INT, nullptr);
        // === Draw Hub Cylinder end ===

        // === Draw Ground ===
        model = glm::mat4(1.0f); // Reset model matrix (Ground is at World Origin)
        normalMat = glm::transpose(glm::inverse(glm::mat3(model)));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));

        // Ground color
        glUniform3f(objectColorLoc, 0.32f, 0.53f, 0.05f);

        glBindVertexArray(groundVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(groundIndices.size()), GL_UNSIGNED_INT, nullptr);
        // === Draw Ground end ===

        glBindVertexArray(0); // Swap buffer display
        glfwSwapBuffers(window);
    }

    // Cleanup all resources
    glDeleteVertexArrays(1, &towerVAO);
    glDeleteBuffers(1, &towerVBO);
    glDeleteBuffers(1, &towerEBO);
    glDeleteVertexArrays(1, &capVAO);
    glDeleteBuffers(1, &capVBO);
    glDeleteBuffers(1, &capEBO);
    glDeleteVertexArrays(1, &bladeVAO);
    glDeleteBuffers(1, &bladeVBO);
    glDeleteBuffers(1, &bladeEBO);
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteBuffers(1, &groundEBO);
    glDeleteVertexArrays(1, &hubVAO);
    glDeleteBuffers(1, &hubVBO);
    glDeleteBuffers(1, &hubEBO);

    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}