#include "render.h"
#include "shader.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "texture.h"
#include "../object/gameobject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

GameObject* floor_obj;
GameObject* cube_obj;
Shader* shader;
Texture* texture;
Texture* texture_cobblestone;
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
float delta_time = 0.0f;
float last_frame = 0.0f;
float camera_speed = 4.5f * delta_time;
float last_x = 400, last_y = 300;
float yaw = -90.0f, pitch = 0.0f;
bool first_mouse = true;

Shader* text_shader = nullptr;


FT_Library ft;
FT_Face face;
GLuint textVAO, textVBO;

bool init_free_type() {
    text_shader = Shader_gimme("../../src/shaders/textv.glsl", "../../src/shaders/textf.glsl");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (FT_Init_FreeType(&ft)) {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        return false;
    }
    // if (FT_New_Face(ft, "C:\\Users\\owcar\\milo\\res\\fonts\\inter.ttf", 0, &face)) {
    if (FT_New_Face(ft, "C:\\Windows\\Fonts\\Arial.ttf", 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font\n");
        return false;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void draw_string(std::string text, float x, float y, float scale, glm::vec3 color) {
    Shader_bind(text_shader);
    Shader_set_uniform3f(text_shader, "textColor", color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        if (FT_Load_Char(face, *c, FT_LOAD_RENDER)) {
            printf("ERROR::FREETYPE: Failed to load Glyph\n");
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        float xpos = x + face->glyph->bitmap_left * scale;
        float ypos = y - (face->glyph->bitmap.rows - face->glyph->bitmap_top) * scale;

        float w = face->glyph->bitmap.width * scale;
        float h = face->glyph->bitmap.rows * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (face->glyph->advance.x >> 6) * scale;
        y += (face->glyph->advance.y >> 6) * scale;

        glDeleteTextures(1, &texture);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; 
    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(front);
}

RenderContext* Render_prelude() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return nullptr;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    RenderContext* ctx = (RenderContext*)malloc(sizeof(RenderContext));
    if (!ctx) {
        printf("Couldn't allocate memory for RenderContext structure.\n");
        return nullptr;
    }

    ctx->window = glfwCreateWindow(800, 600, "Ryswick", NULL, NULL);
    if (ctx->window == NULL) {
        printf("Failed to create window\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(ctx->window);
    printf(glfwGetVersionString());

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return nullptr;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(ctx->window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // Blending
    // --------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    // Depth test, without this cubes are fucked.
    // ------------------------------------------
    glEnable(GL_DEPTH_TEST);

    floor_obj = GameObject_new("cobblestone.png", "floor.bin");
    cube_obj = GameObject_new("heurix.png", "cube.bin"); 
    return ctx;
}

void draw_floor(GLFWwindow* window) {
    glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

    GameObject_draw(
        floor_obj,
        light_pos,
        camera_pos,
        view,
        proj
    );
}

void draw_square(GLFWwindow* window) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cube_obj->position);
    model = glm::rotate(model, glm::radians(cube_obj->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(cube_obj->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(cube_obj->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, cube_obj->scale);

    glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

    GameObject_draw(
        cube_obj,
        light_pos,
        camera_pos,
        view,
        proj
    );
}

void render(GLFWwindow* window) {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    Renderer_clear();
    draw_floor(window);
    draw_square(window);

    std::string debug_text = std::to_string(camera_pos.x) + ", " + std::to_string(camera_pos.y) + ", " + std::to_string(camera_pos.z);
    draw_string(debug_text, 25.0f, 25.0f, 0.6f, { 1.0f, 0.0f, 0.0f });

}

void process_input(GLFWwindow* window) {
    float camera_speed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        GameObject_set_position(cube_obj, cube_obj->position.x, cube_obj->position.y, cube_obj->position.z - 0.01f);
        GameObject_set_scale(cube_obj, 3.0f, 3.0f, 3.0f);
    }
}

void Render_loop(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!init_free_type()) {
        printf("Failed to initialize FreeType\n");
        return;
    }

    glm::mat4 projection = glm::ortho(0.0f, (float)800, 0.0f, (float)600);
    Shader_bind(text_shader);
    Shader_set_uniform_mat4f(text_shader, "projection", projection);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}

void Render_die(RenderContext* ctx) {
    glfwDestroyWindow(ctx->window);
    glfwTerminate();
    delete shader;
}