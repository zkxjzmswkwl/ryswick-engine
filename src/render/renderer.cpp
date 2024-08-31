#include "renderer.h"
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

void Renderer_draw(VertexArray* va, IndexBuffer* ib, Shader* shader) {
    Shader_bind(shader);
    VertexArray_bind(va);
    IndexBuffer_bind(ib);

    glDrawElements(GL_TRIANGLES, ib->count, GL_UNSIGNED_INT, nullptr);
}

void Renderer_clear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}