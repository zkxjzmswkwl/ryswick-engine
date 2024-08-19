#include "render.h"
#include "shader.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string>

Shader* shader;
float vertices[6] = {
    -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
};

RenderContext* Render_prelude() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return nullptr;
    }

    glfwSetErrorCallback(glfw_error_callback);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    RenderContext* ctx = (RenderContext*)malloc(sizeof(RenderContext));
    if (!ctx) {
        printf("Couldn't allocate memory for RenderContext structure.\n");
        return nullptr;
    }

    ctx->window = glfwCreateWindow(800, 600, "Ryswick", NULL, NULL);
    if (ctx->window == NULL) {
        printf("Failed to create window\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(ctx->window);
    printf(glfwGetVersionString());

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return nullptr;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(ctx->window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    shader = Shader_gimme("../../src/shaders/vertex.glsl", "../../src/shaders/fragment.glsl");
    return ctx;
}

void draw_triangle() {
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glUseProgram(shader->program_id);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_triangle();
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Render_loop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}

void Render_die(RenderContext* ctx) {
    glfwDestroyWindow(ctx->window);
    glfwTerminate();
    delete shader;
}