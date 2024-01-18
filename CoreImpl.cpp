#include "CoreImpl.h"
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
#include <chrono>


namespace Core
{
    CoreImpl::CoreImpl()
        : isInit_{false}
        , initDone_{false}
        , isClean_{true}
    {

    }

    CoreImpl::~CoreImpl()
    {
    }

    void CoreImpl::initCallback()
    {
        try
        {
            // Initialize Sokol GFX
            sg_desc sgdesc{ 0 };
            sgdesc.context = sapp_sgcontext();
            sg_setup(&sgdesc);

            if (!sg_isvalid()) {
                throw std::runtime_error("Failed to create Sokol GFX context!");
            }

            // Initialize Sokol GP
            sgp_desc sgpdesc{ 0 };
            sgp_setup(&sgpdesc);
            if (!sgp_is_valid()) {
                throw std::runtime_error(std::string("Failed to create Sokol GP context: ") + sgp_get_error_message(sgp_get_last_error()));
            }

            // Initialize ImGUI
            simgui_desc_t imdesc{ 0 };
            imdesc.sample_count = sapp_sgcontext().sample_count;
            imdesc.no_default_font = true;
            simgui_setup(&imdesc);

            ImGuiIO& im_io = ImGui::GetIO();
            ImFontConfig cfg;
            cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_LightHinting;
            im_io.Fonts->AddFontFromFileTTF("Rubik-Regular.ttf", 20, &cfg);
            {
                unsigned char* font_pixels;
                int font_width, font_height;
                im_io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);

                sg_image_desc font_img_desc{ 0 };
                font_img_desc.width = font_width;
                font_img_desc.height = font_height;
                font_img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
                font_img_desc.data.subimage[0][0].ptr = font_pixels;
                font_img_desc.data.subimage[0][0].size = (size_t)(font_width * font_height) * sizeof(uint32_t);
                font_img_desc.label = "sokol-imgui-font-image";
                _simgui.font_img = sg_make_image(&font_img_desc);

                simgui_image_desc_t img_desc{ 0 };
                img_desc.image = _simgui.font_img;
                img_desc.sampler = _simgui.font_smp;
                _simgui.default_font = simgui_make_image(&img_desc);
                im_io.Fonts->TexID = simgui_imtextureid(_simgui.default_font);
            }

            isInit_ = true;
        }
        catch (const std::exception&)
        {
            isInit_ = false;
        }
        initDone_ = true;
        isClean_ = false;
    }

    void CoreImpl::eventCallback(const sapp_event* event)
    {
        if (event->type == sapp_event_type::SAPP_EVENTTYPE_QUIT_REQUESTED && closeAppCb_) {
            closeAppCb_();
            return;
        }
        simgui_handle_event(event);
        if (event->type == sapp_event_type::SAPP_EVENTTYPE_KEY_DOWN && keyDownCb_) {
            keyDownCb_((int)event->key_code);
        }
    }

    void CoreImpl::frameCallback()
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
        sgp_set_blend_mode(SGP_BLENDMODE_BLEND);

        sgp_set_color(0.55f, 0.55f, 0.55f, 1.0f);
        sgp_push_transform();
        sgp_translate(200, 200);
        sgp_rotate_at(0.5, 0, 0);
        sgp_draw_filled_rect(0, 0, 200, 200);
        sgp_pop_transform();


        ImGui::ShowDemoWindow();

        // dispatch draw commands
        static const sg_pass_action pass_action = { 0 };
        sg_begin_default_pass(&pass_action, width, height);
        sgp_flush();
        sgp_end();
        simgui_render();
        sg_end_pass();
        sg_commit();
    }

    void CoreImpl::cleanupCallback()
    {
        if (!isClean_) {
            simgui_shutdown();
            sgp_shutdown();
            sg_shutdown();
            isClean_ = true;
        }
    }

    void CoreImpl::setKeyDownCallback(KeyDownCallback cb)
    {
        keyDownCb_ = cb;
    }

    void CoreImpl::setCloseAppCallback(SimpleCallback cb)
    {
        closeAppCb_ = cb;
    }

    void CoreImpl::showError(const std::string& msg)
    {
        MessageBoxA(nullptr, "Error", msg.c_str(), MB_ICONERROR);
    }

    void sokolMainThread(const sapp_desc& desc)
    {
        sapp_run(desc);
    }

    bool CoreImpl::init(const InitParams& params)
    {
        isInit_   = false;
        initDone_ = false;

        sapp_desc desc{ 0 };
        desc.init_cb  = params.init_cb;
        desc.frame_cb = params.frame_cb;
        desc.event_cb = params.event_cb;
        desc.cleanup_cb = params.cleanup_cb;
        desc.width  = params.windowWidth;
        desc.height = params.windowHeight;
        desc.sample_count = params.sampleCount;

        mainThread_ = std::thread(sokolMainThread, std::ref(desc));

        while (!initDone_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return isInit_;
    }

    void CoreImpl::free()
    {
        sapp_request_quit();
        mainThread_.join();
        cleanupCallback();
    }
}

