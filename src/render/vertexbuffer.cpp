#include "vertexbuffer.h"
#include "renderer.h"

VertexBuffer* VertexBuffer_new(const void* data, unsigned int usize) {
    VertexBuffer* vb = new VertexBuffer();
    glGenBuffers(1, &vb->id);
    glBindBuffer(GL_ARRAY_BUFFER, vb->id);
    glBufferData(GL_ARRAY_BUFFER, usize, data, GL_STATIC_DRAW);
    return vb;
}

void VertexBuffer_delete(VertexBuffer* vb) {
    glDeleteBuffers(1, &vb->id);
    delete vb;
}

void VertexBuffer_bind(VertexBuffer* vb) {
    glBindBuffer(GL_ARRAY_BUFFER, vb->id);
}

void VertexBuffer_unbind(VertexBuffer* vb) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}