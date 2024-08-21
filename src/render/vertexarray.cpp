#include "vertexarray.h"
#include "renderer.h"

VertexArray* VertexArray_new() {
    VertexArray* va = new VertexArray();
    glGenVertexArrays(1, &va->id);
    return va;
}

void VertexArray_delete(VertexArray* va) {
    glDeleteVertexArrays(1, &va->id);
    delete va;
}

void VertexArray_bind(VertexArray* va) {
    glBindVertexArray(va->id);
}

void VertexArray_unbind(VertexArray* va) {
    glBindVertexArray(0);
}

void VertexArray_add_buffer(VertexArray* va, VertexBuffer* vb, VertexBufferLayout* layout) {
    VertexArray_bind(va);
    VertexBuffer_bind(vb);

    unsigned int offset = 0;
    for (unsigned int i = 0; i < layout->elements.size(); i++) {
        const auto& element = layout->elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->stride, (const void*)offset);
        offset += element.count * VertexBufferElement::size_of_type(element.type);
    }
}