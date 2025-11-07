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

// 加载着色器程序函数
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
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 创建顶点着色器并编译
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建片段着色器并编译
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建着色器程序并链接
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 编译完成后删除着色器对象，程序已链接
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return shaderProgram;
}

// 生成圆柱体的顶点和索引
void generateCylinder(float radius, float height, int slices, std::vector<float> &vertexData,
                      std::vector<GLuint> &indices) {
    vertexData.clear();
    indices.clear();
    float thetaStep = 2.0f * 3.141592653589793f / slices; // 每一片的角度

    // 底面中心顶点
    vertexData.push_back(0.0f);     // x
    vertexData.push_back(0.0f);     // y
    vertexData.push_back(0.0f);     // z
    vertexData.push_back(0.0f);     // 法向量 x
    vertexData.push_back(-1.0f);    // 法向量 y
    vertexData.push_back(0.0f);     // 法向量 z

    // 底面周围顶点
    for (int i = 0; i < slices; ++i) {
        float theta = i * thetaStep;
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);
        vertexData.push_back(x);
        vertexData.push_back(0.0f);
        vertexData.push_back(z);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);
        vertexData.push_back(0.0f);
    }

    // 底面索引（三角形）
    for (int i = 0; i < slices; ++i) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % slices + 1);
    }

    // 顶面中心顶点索引
    GLuint topCenterIndex = static_cast<GLuint>(vertexData.size() / 6);
    vertexData.push_back(0.0f);
    vertexData.push_back(height);
    vertexData.push_back(0.0f);
    vertexData.push_back(0.0f);
    vertexData.push_back(1.0f);
    vertexData.push_back(0.0f);

    // 顶面周围顶点
    for (int i = 0; i < slices; ++i) {
        float theta = i * thetaStep;
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);
        vertexData.push_back(x);
        vertexData.push_back(height);
        vertexData.push_back(z);
        vertexData.push_back(0.0f);
        vertexData.push_back(1.0f);
        vertexData.push_back(0.0f);
    }

    // 顶面索引
    for (int i = 0; i < slices; ++i) {
        indices.push_back(topCenterIndex);
        indices.push_back(topCenterIndex + (i + 1) % slices + 1);
        indices.push_back(topCenterIndex + i + 1);
    }

    // 侧面顶点（下圈）
    GLuint sideBottomStart = static_cast<GLuint>(vertexData.size() / 6);
    for (int i = 0; i < slices; ++i) {
        float theta = i * thetaStep;
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);
        float nx = std::cos(theta);
        float nz = std::sin(theta);
        vertexData.push_back(x);
        vertexData.push_back(0.0f);
        vertexData.push_back(z);
        vertexData.push_back(nx);
        vertexData.push_back(0.0f);
        vertexData.push_back(nz);
    }

    // 侧面顶点（上圈）
    GLuint sideTopStart = static_cast<GLuint>(vertexData.size() / 6);
    for (int i = 0; i < slices; ++i) {
        float theta = i * thetaStep;
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);
        float nx = std::cos(theta);
        float nz = std::sin(theta);
        vertexData.push_back(x);
        vertexData.push_back(height);
        vertexData.push_back(z);
        vertexData.push_back(nx);
        vertexData.push_back(0.0f);
        vertexData.push_back(nz);
    }

    // 侧面索引
    for (int i = 0; i < slices; ++i) {
        GLuint bottomLeft = sideBottomStart + i;
        GLuint bottomRight = sideBottomStart + (i + 1) % slices;
        GLuint topLeft = sideTopStart + i;
        GLuint topRight = sideTopStart + (i + 1) % slices;
        indices.push_back(bottomLeft);
        indices.push_back(bottomRight);
        indices.push_back(topRight);
        indices.push_back(bottomLeft);
        indices.push_back(topRight);
        indices.push_back(topLeft);
    }
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
    GLFWwindow *window = glfwCreateWindow(800, 600, "Windmill Action Scene", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD Initialization
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST); // 开启深度测试

    // Shaders
    GLuint program = loadShader("shader.vert", "shader.frag");
    glUseProgram(program);

    // ===== 获取 uniform 位置 =====
    GLint modelLoc = glGetUniformLocation(program, "model");
    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint projLoc = glGetUniformLocation(program, "proj");
    GLint normalMatLoc = glGetUniformLocation(program, "normalMat");
    GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
    GLint lightColorLoc = glGetUniformLocation(program, "lightColor");
    GLint objectColorLoc = glGetUniformLocation(program, "objectColor");
    GLint shininessLoc = glGetUniformLocation(program, "shininess");

    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform1f(shininessLoc, 32.0f);

    // ===== 风车塔体（圆柱）Tower (cylinder) =====
    std::vector<float> towerVertexData;
    std::vector<GLuint> towerIndices;
    generateCylinder(1.0f, 10.0f, 32, towerVertexData, towerIndices);
    GLuint towerVAO, towerVBO, towerEBO;
    glGenVertexArrays(1, &towerVAO);
    glGenBuffers(1, &towerVBO);
    glGenBuffers(1, &towerEBO);
    glBindVertexArray(towerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, towerVBO);
    glBufferData(GL_ARRAY_BUFFER, towerVertexData.size() * sizeof(float), towerVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, towerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, towerIndices.size() * sizeof(GLuint), towerIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Cap (cube)
    std::vector<float> capVertexData = {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Blade (quad)
    std::vector<float> bladeVertexData = {
        -0.2f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // 左下
         0.2f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // 右下
         0.2f, 3.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // 右上
        -0.2f, 3.5f, 0.0f,  0.0f, 0.0f, 1.0f   // 左上
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Display control tip in console
    std::cout << "Controls:\n";
    std::cout << "Camera: W/S/A/D/Q/E to move (forward/back/left/right/down/up)\n";
    std::cout << "Light: Arrow keys left/right/up/down for x/y, comma/period for z\n";
    std::cout << "Cap rotation speed: + to increase, - to decrease\n";
    std::cout << "Blade rotation speed: I to increase, K to decrease\n";
    std::cout << "R to reverse direction (press once to toggle)\n";
    std::cout << "ESC to exit\n";

    // Variables
    float lastTime = 0.0f;                                    // 上一帧时间
    glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 20.0f); // 摄像机位置
    glm::vec3 lookAtPos = glm::vec3(0.0f, 5.0f, 0.0f);  // 摄像机观察点
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);         // 上方向
    glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 5.0f);   // 光源位置
    int direction = 1;                                        // 旋转方向
    float capAngle = 0.0f;                                    // 风车帽旋转角度
    float bladeAngle = 0.0f;                                  // 叶片旋转角度
    float capRotationSpeed = 10.0f;                           // 风车帽旋转速度（度/秒）degrees/sec
    float bladeRotationSpeed = 60.0f;                         // 风车叶片旋转速度（度/秒）degrees/sec
    float moveSpeed = 10.0f;                                  // 摄像机移动速度
    bool rPressed = false;                                    // R键按下标志


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents(); // 处理事件

        // ESC 退出
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

        // Cap 旋转速度
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) capRotationSpeed += 50.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) capRotationSpeed -= 50.0f * deltaTime;
        if (capRotationSpeed < 0.0f) capRotationSpeed = 0.0f;
        if (capRotationSpeed > 500.0f) capRotationSpeed = 500.0f;

        // Blade 旋转速度
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) bladeRotationSpeed += 100.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) bladeRotationSpeed -= 100.0f * deltaTime;
        if (bladeRotationSpeed < 0.0f) bladeRotationSpeed = 0.0f;
        if (bladeRotationSpeed > 1000.0f) bladeRotationSpeed = 1000.0f;

        // Direction control
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            if (!rPressed) {
                direction = -direction; // 翻转旋转方向
                rPressed = true;
            }
        } else {
            rPressed = false;
        }

        // Update angles
        capAngle += direction * capRotationSpeed * deltaTime;
        bladeAngle += direction * bladeRotationSpeed * deltaTime;
        capAngle = std::fmod(capAngle, 360.0f);
        bladeAngle = std::fmod(bladeAngle, 360.0f);

        // ===== 渲染 =====
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, lookAtPos, up);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        // Draw tower
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
        glUniform3f(objectColorLoc, 0.5f, 0.5f, 0.5f);
        glBindVertexArray(towerVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(towerIndices.size()), GL_UNSIGNED_INT, 0);

        // Draw cap
        glm::mat4 capModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        capModel = glm::rotate(capModel, glm::radians(capAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        capModel = glm::scale(capModel, glm::vec3(1.5f, 1.0f, 1.5f));
        normalMat = glm::transpose(glm::inverse(glm::mat3(capModel)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(capModel));
        glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
        glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f);
        glBindVertexArray(capVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(capIndices.size()), GL_UNSIGNED_INT, 0);

        // Draw 4 blades
        glUniform3f(objectColorLoc, 0.0f, 0.0f, 1.0f);
        for (int i = 0; i < 4; ++i) {
            glm::mat4 bladeModel = capModel;
            // 平移到方块侧面中心，稍微留间隙
            bladeModel = glm::translate(bladeModel, glm::vec3(0.0f, 0.0f, 1.05f));
            // 围绕 Z 轴旋转叶片
            bladeModel = glm::rotate(bladeModel, glm::radians(bladeAngle + i * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            normalMat = glm::transpose(glm::inverse(glm::mat3(bladeModel)));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bladeModel));
            glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
            glBindVertexArray(bladeVAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(bladeIndices.size()), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0); // 交换缓冲区显示
        glfwSwapBuffers(window);
    }

    // Cleanup resources
    glDeleteVertexArrays(1, &towerVAO);
    glDeleteBuffers(1, &towerVBO);
    glDeleteBuffers(1, &towerEBO);
    glDeleteVertexArrays(1, &capVAO);
    glDeleteBuffers(1, &capVBO);
    glDeleteBuffers(1, &capEBO);
    glDeleteVertexArrays(1, &bladeVAO);
    glDeleteBuffers(1, &bladeVBO);
    glDeleteBuffers(1, &bladeEBO);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}
