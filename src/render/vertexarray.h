#pragma once
#include "vertexbuffer.h"
#include <vector>

struct VertexBufferLayout;
struct VertexBufferElement;

struct VertexArray {
    unsigned int id;
};

VertexArray* VertexArray_new();
void VertexArray_bind(VertexArray* va);
void VertexArray_unbind(VertexArray* va);
void VertexArray_delete(VertexArray* va);
void VertexArray_add_buffer(VertexArray* va, VertexBuffer* vb, VertexBufferLayout* layout);

struct VertexBufferElement {
    unsigned int count;
    unsigned int type;
    unsigned int normalized;

    static unsigned int size_of_type(unsigned int type) {
        switch (type) {
            case /*GL_FLOAT*/        0x1406: return sizeof(float);
            case /*GL_UNSIGNED_INT*/ 0x1405: return sizeof(unsigned int);
            case /*GL_UNSIGNED_BYTE*/0x1401: return sizeof(unsigned char);
        }
        return 0;
    }
};

// ------------------
// VertexBufferLayout
// ------------------
struct VertexBufferLayout {
    VertexBufferLayout() : stride(0) {}
    std::vector<VertexBufferElement> elements;
    unsigned int stride = 0;
};

template <typename T>
static void VBL_push(VertexBufferLayout* layout, unsigned int count) {
}

template <>
static void VBL_push<float>(VertexBufferLayout* layout, unsigned int count) {
    layout->elements.push_back({count, /*GL_FLOAT*/0x1406, 0});
    layout->stride += count * sizeof(float);
}

template <>
static void VBL_push<unsigned int>(VertexBufferLayout* layout, unsigned int count) {
    layout->elements.push_back({count, /*GL_UNSIGNED_INT*/0x1405, 0});
    layout->stride += count * sizeof(unsigned int);
}

template <>
static void VBL_push<unsigned char>(VertexBufferLayout* layout, unsigned int count) {
    layout->elements.push_back({count, /*GL_UNSIGNED_BYTE*/0x1401, 1});
    layout->stride += count * sizeof(unsigned char);
}