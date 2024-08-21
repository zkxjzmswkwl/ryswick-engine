#pragma once
#include <string>

struct GLFWwindow;

struct RenderContext {
    GLFWwindow* window;
};

RenderContext* Render_prelude();

void render();
// Probably move this elsewhere later.
void process_input(GLFWwindow*);
void Render_loop(GLFWwindow*);
void glfw_error_callback(int error, const char* description);
void Render_die(RenderContext*);