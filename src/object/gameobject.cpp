#include "gameobject.h"
#include "../render/renderer.h"
#include "../render/vertexarray.h"
#include "../render/vertexbuffer.h"
#include "../render/indexbuffer.h"
#include "../render/shader.h"
#include "../render/texture.h"
#include "binread.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject* GameObject_new(
    const std::string& texture_file,
    const std::string& bin_file
) {
    GameObject* go = new GameObject();
    go->vertices = Bin_readf("C:\\Users\\owcar\\milo\\res\\objects\\vertices\\" + bin_file);
    go->indices = Bin_readi("C:\\Users\\owcar\\milo\\res\\objects\\indices\\" + bin_file);
    go->shader = Shader_gimme("../../src/shaders/vertex.glsl", "../../src/shaders/fragment.glsl");
    go->texture = Texture_new("../../res/textures/" + texture_file);
    go->va = VertexArray_new();
    go->vb = VertexBuffer_new(go->vertices->buffer, go->vertices->size);
    go->ib = IndexBuffer_new((const unsigned int*)go->indices->buffer, go->indices->size);
    go->layout = VertexBufferLayout_new();
    // Pos
    VBL_push<float>(go->layout, 3);
    // Txtr coords
    VBL_push<float>(go->layout, 2);
    // Normals
    VBL_push<float>(go->layout, 3);

    VertexArray_add_buffer(go->va, go->vb, go->layout);

    go->position = glm::vec3(0.0f, 0.0f, 0.0f);
    go->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    go->scale = glm::vec3(1.0f, 1.0f, 1.0f);

    return go;
}

void GameObject_draw(
    GameObject* go,
    glm::vec3& light_pos,
    glm::vec3& view_pos,
    glm::mat4& view,
    glm::mat4& proj
) {
    Texture_bind(go->texture, 0);
    Shader_set_uniform1i(go->shader, "u_Texture", 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, go->position);
    model = glm::rotate(model, glm::radians(go->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(go->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(go->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, go->scale);

    Shader_set_uniform_mat4f(go->shader, "u_Model", model);
    Shader_set_uniform_mat4f(go->shader, "u_View", view);
    Shader_set_uniform_mat4f(go->shader, "u_Projection", proj);
    Shader_set_uniform3f(go->shader, "u_LightPos", light_pos.x, light_pos.y, light_pos.z);
    Shader_set_uniform3f(go->shader, "u_ViewPos", view_pos.x, view_pos.y, view_pos.z);

    Renderer_draw(go->va, go->ib, go->shader);
}

void GameObject_set_position(GameObject* go, float x, float y, float z) {
    go->position = glm::vec3(x, y, z);
}

void GameObject_set_rotation(GameObject* go, float pitch, float yaw, float roll) {
    go->rotation = glm::vec3(pitch, yaw, roll);
}

void GameObject_set_scale(GameObject* go, float x, float y, float z) {
    go->scale = glm::vec3(x, y, z);
}