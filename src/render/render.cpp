#include "render.h"
#include "shader.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Shader* shader;
Texture* texture;

float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f,  -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,

        -0.5f, 0.5f, 0.0f, 1.0f
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
    texture = Texture_new("C:\\Users\\owcar\\Downloads\\AIGen\\wat.jpg");
    Texture_bind(texture, 0);
    return ctx;
}

void draw_square(GLFWwindow* window) {
    VertexArray* va = VertexArray_new();
    VertexBuffer* vb = VertexBuffer_new(vertices, 4*4*sizeof(float));
    IndexBuffer* ib = IndexBuffer_new(indices, 6);
    VertexBufferLayout layout;

    VBL_push<float>(&layout, 2);
    VBL_push<float>(&layout, 2);
    VertexArray_add_buffer(va, vb, &layout);
    Shader_set_uniform1i(shader, "u_Texture", 0);
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    Shader_set_uniform_mat4f(shader, "u_MVP", proj);

    Renderer_draw(va, ib, shader);

    VertexBuffer_delete(vb);
    IndexBuffer_delete(ib);
    VertexArray_delete(va);
}

void render(GLFWwindow* window) {
    Renderer_clear();
    draw_square(window);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Render_loop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        render(window);
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