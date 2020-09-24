#include "../libs/sokol/sokol_app.h"
#include "../libs/sokol/sokol_gfx.h"
#include "../libs/sokol/sokol_glue.h"

sg_pass_action pass_action;

void init(void) {
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext()
    });
    pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .val={1.0f, 0.0f, 0.0f, 1.0f} }
    };
}

void frame(void) {
    sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = 1024,
        .height = 600,
        .window_title = "ezy",
    };
}
