#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <gl/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

extern const char* vertexShader;
extern const char* fragmentShader;

glm::vec3 kEye = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 kCenter = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 kUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 kLightPos = glm::vec3(3.0f, 3.0f, 3.0f);
glm::vec3 kColor = glm::vec3(0.0f, 0.0f, 1.0f);
float kYaw = 0.0f;
float kPitch = 0.0f;

std::vector<glm::vec3> vertices = {
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f, -1.0f),

    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f),

    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),

    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f),

    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),

    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f),

    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),

    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),

    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f),

    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),

    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f),

    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
};

std::vector<glm::vec3> normals = {
    glm::vec3(0.0f,  1.0f, 0.0f),
    glm::vec3(0.0f,  1.0f, 0.0f),
    glm::vec3(0.0f,  1.0f, 0.0f),

    glm::vec3(0.0f, 0.0f,  1.0f),
    glm::vec3(0.0f, 0.0f,  1.0f),
    glm::vec3(0.0f, 0.0f,  1.0f),

    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),

    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),

    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),

    glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -1.0f),

    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),

    glm::vec3(0.0f,  0.0f,  1.0f),
    glm::vec3(0.0f,  0.0f,  1.0f),
    glm::vec3(0.0f,  0.0f,  1.0f),

    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),

    glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f),

    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),

    glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -1.0f), 
};

std::vector<int> indices = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 33, 34, 35, 36,
};


GLuint compileShader(GLchar* vertexShader, GLchar* fragmentShader) {
    auto debug_print = [](const GLuint shader) {
        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint log_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

            GLsizei length;
            GLchar* log = (GLchar*)malloc(log_length);
            glGetShaderInfoLog(shader, log_length, &length, log);

            printf("InfoLog:\n%s\n\n", log);

            free(log);

            exit(EXIT_FAILURE);
        }
        };

    GLint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    debug_print(vs);

    GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    debug_print(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    glDeleteShader(fs);
    glDeleteShader(vs);

    return shader_program;
}

void glfwCursorPos(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos;
    static double lastY = ypos;

    float dx = (float)(xpos - lastX);
    float dy = (float)(ypos - lastY);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        kYaw -= dx * 0.1f;
        kPitch -= dy * 0.1f;

        glm::vec3 front;
        front.x = sin(glm::radians(kYaw)) * cos(glm::radians(kPitch));
        front.y = sin(glm::radians(kPitch));
        front.z = cos(glm::radians(kYaw)) * cos(glm::radians(kPitch));
        front = glm::normalize(front);
        kEye = front * 3.0f -  kCenter;
    }

    lastX = xpos;
    lastY = ypos;
}

int main(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    int window_width = 800;
    int window_height = 600;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "GLExample", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, glfwCursorPos);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return -1;
    }

    GLuint shader = compileShader((GLchar*)vertexShader, (GLchar*)fragmentShader);

    GLuint vao, vbo[2], ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);

        glm::mat4 view = glm::lookAt(kEye, kCenter, kUp);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);

        glEnable(GL_DEPTH_TEST | GL_VERTEX_PROGRAM_POINT_SIZE);
        glDepthFunc(GL_LESS);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "u_View"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "u_Model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(shader, "u_Color"), 1, glm::value_ptr(kColor));
        glUniform3fv(glGetUniformLocation(shader, "u_lightPos"), 1, glm::value_ptr(kLightPos));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}