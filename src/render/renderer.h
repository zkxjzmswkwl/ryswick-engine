#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct VertexArray;
struct IndexBuffer;
struct Shader;

void Renderer_draw(VertexArray* va, IndexBuffer* ib, Shader* shader);
void Renderer_clear();
