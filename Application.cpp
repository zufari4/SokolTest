#include "Application.h"
#include <imgui_freetype.h>
#define SOKOL_IMPL
#define SOKOL_D3D11
#define SOKOL_NO_ENTRY
#include "sokol_gfx.h"
#include "sokol_gp.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "util/sokol_imgui.h"
#include <stdexcept>


void init_cb();
void event_cb(const sapp_event* event);
void frame_cb();
Application* gApp = nullptr;


Application::Application()
{
    gApp = this;
}

Application::~Application()
{
    gApp = nullptr;
}

bool Application::init()
{
    return true;
}

void Application::run()
{
    sapp_desc desc{ 0 };
    desc.init_cb  = init_cb;
    desc.frame_cb = frame_cb;
    desc.event_cb = event_cb;
    desc.width  = 1024;
    desc.height = 768;
    desc.sample_count = 8;

    sapp_run(desc);
}

void Application::free()
{
    simgui_shutdown();
    sgp_shutdown();
    sg_shutdown();
}

void Application::showError(const std::string& msg)
{
    MessageBoxA(nullptr, "Error", msg.c_str(), MB_ICONERROR);
}



void Application::initCallback()
{
    // Initialize Sokol GFX.
    sg_desc sgdesc{ 0 };
    sgdesc.context = sapp_sgcontext();
    sg_setup(&sgdesc);

    if (!sg_isvalid()) {
        throw std::runtime_error("Failed to create Sokol GFX context!");
    }

    // Initialize Sokol GP, adjust the size of command buffers for your own use.
    sgp_desc sgpdesc{0};
    sgp_setup(&sgpdesc);
    if (!sgp_is_valid()) {
        throw std::runtime_error(std::string("Failed to create Sokol GP context: ") + sgp_get_error_message(sgp_get_last_error()));
    }

    simgui_desc_t imdesc{ 0 };
    imdesc.sample_count = 8;
    imdesc.no_default_font = false;
    simgui_setup(&imdesc);

    //ImFontConfig cfg;
    //cfg.FontBuilderFlags = (ImGuiFreeTypeBuilderFlags)settings_.fontHinting;
    //ImGui::GetIO().Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 19);
    //ImGui::GetIO().Fonts->Build();
}

void Application::eventCallback(const sapp_event* event)
{
    simgui_handle_event(event);
}

void Application::frameCallback()
{
    static int width = sapp_width(), height = sapp_height();

    static simgui_frame_desc_t im_desc{ 0 };
    im_desc.width = width;
    im_desc.height = height;
    im_desc.delta_time = sapp_frame_duration();
    im_desc.dpi_scale = sapp_dpi_scale();
    
    simgui_new_frame(&im_desc);
    sgp_begin(width, height);

    // draw background
    sgp_set_color(0.05f, 0.05f, 0.05f, 1.0f);
    sgp_clear();
    //sgp_set_blend_mode(SGP_BLENDMODE_BLEND);

    sgp_set_color(0.55f, 0.55f, 0.55f, 1.0f);
    sgp_push_transform();
    sgp_translate(200, 200);
    sgp_rotate_at(0.5, 0, 0);
    sgp_draw_filled_rect(0,0, 200,200);
    sgp_pop_transform();

    
    ImGui::ShowDemoWindow();

    // dispatch draw commands
    static const sg_pass_action pass_action = {0};
    sg_begin_default_pass(&pass_action, width, height);
    sgp_flush();
    sgp_end();
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void init_cb() 
{
    if (gApp) gApp->initCallback();
}

void event_cb(const sapp_event* event)
{
    if (gApp) gApp->eventCallback(event);
}

void frame_cb()
{
    if (gApp) gApp->frameCallback();
}