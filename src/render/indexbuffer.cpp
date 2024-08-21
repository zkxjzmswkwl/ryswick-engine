#include "indexbuffer.h"
#include "renderer.h"

IndexBuffer* IndexBuffer_new(const unsigned int* data, unsigned int count) {
    IndexBuffer* ib = new IndexBuffer();
    ib->count = count;

    glGenBuffers(1, &ib->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    return ib;

}

void IndexBuffer_delete(IndexBuffer* ib) {
    glDeleteBuffers(1, &ib->id);
    delete ib;
}

void IndexBuffer_bind(IndexBuffer* ib) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->id);
}

void IndexBuffer_unbind(IndexBuffer* ib) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}