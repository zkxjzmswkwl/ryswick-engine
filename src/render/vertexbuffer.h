#pragma once

struct VertexBuffer {
    unsigned int id;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int vertex_count;
    unsigned int index_count;
};

VertexBuffer* VertexBuffer_new(const void* data, unsigned int usize);
void VertexBuffer_bind(VertexBuffer* vb);
void VertexBuffer_unbind(VertexBuffer* vb);
void VertexBuffer_delete(VertexBuffer* vb);