#include "render.h"
#include "shader.h"
#include <stdio.h>
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include <stdlib.h>
#include <string>

Shader* shader;

float vertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        0.5f, 0.5f,

        -0.5f, 0.5f,
};
unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
};

RenderContext* Render_prelude() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return nullptr;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

void draw_square() {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexArray va;
    VertexBuffer* vb = VertexBuffer_new(vertices, 4*2*sizeof(float));
    VertexBufferLayout layout;

    VBL_push<float>(&layout, 2);
    VertexArray_add_buffer(&va, vb, &layout);

    IndexBuffer* ib = IndexBuffer_new(indices, 6);

    Shader_bind(shader);
    Shader_set_uniform4f(shader, "u_Color", 1.0f, 0.3f, 0.3f, 1.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    VertexBuffer_delete(vb);
    IndexBuffer_delete(ib);
}

void render() {
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_square();
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