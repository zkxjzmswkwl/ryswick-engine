#include <stdio.h>
#include "render/render.h"

int main(int, char**) {
    RenderContext* render_ctx = Render_prelude();
    if (render_ctx == nullptr) {
        printf("Failed to initialize render context\n");
        return 1;
    }

    Render_loop(render_ctx->window);
    Render_die(render_ctx);
    return 0;
}
