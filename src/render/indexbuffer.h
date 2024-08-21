#pragma once

struct IndexBuffer {
    unsigned int id;
    unsigned int count;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int vertex_count;
    unsigned int index_count;
};

IndexBuffer* IndexBuffer_new(const unsigned int* data, unsigned int count);
void IndexBuffer_bind(IndexBuffer* vb);
void IndexBuffer_unbind(IndexBuffer* vb);
void IndexBuffer_delete(IndexBuffer* vb);