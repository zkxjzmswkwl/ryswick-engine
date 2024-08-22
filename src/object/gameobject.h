#pragma once
#include <string>
#include <glm/glm.hpp>

struct VertexArray;
struct VertexBuffer;
struct IndexBuffer;
struct VertexBufferLayout;
struct Texture;
struct Shader;
struct Binf;
struct Bini;

struct GameObject {
    Binf* vertices;
    Bini* indices;
    VertexArray* va;
    VertexBuffer* vb;
    IndexBuffer* ib;
    VertexBufferLayout* layout;
    Shader* shader;
    Texture* texture;
    float x, y, w, h;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

GameObject* GameObject_new(
    const std::string& texture_file,
    const std::string& bin_fil
);
// void GameObject_delete(GameObject* object);
void GameObject_draw(
    GameObject* go,
    glm::vec3& light_pos,
    glm::vec3& view_pos,
    glm::mat4& view,
    glm::mat4& proj
);
void GameObject_set_position(GameObject* go, float x, float y, float z);
void GameObject_set_rotation(GameObject* go, float pitch, float yaw, float roll);
void GameObject_set_scale(GameObject* go, float x, float y, float z);
