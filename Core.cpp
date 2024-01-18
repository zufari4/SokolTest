#include "Core.h"
#include "CoreImpl.h"
#include <memory>

namespace Core
{
    CoreImpl core_;
    void init_cb();
    void event_cb(const sapp_event* event);
    void frame_cb();
    void cleanup_cb();

    bool init(bool fullscreen, int windowWidth, int windowHeight, int sampleCount, bool useVsync)
    {
        InitParams params;
        params.init_cb    = init_cb;
        params.event_cb   = event_cb;
        params.frame_cb   = frame_cb;
        params.cleanup_cb = cleanup_cb;
        params.fullscreen = fullscreen;
        params.windowWidth  = windowWidth;
        params.windowHeight = windowHeight;
        params.sampleCount  = sampleCount;
        params.useVsync = useVsync;

        return core_.init(params);
    }

    void free()
    {
        core_.free();
    }

    void setKeyDownCallback(KeyDownCallback cb)
    {
        core_.setKeyDownCallback(cb);
    }

    void setCloseAppCallback(SimpleCallback cb)
    {
        core_.setCloseAppCallback(cb);
    }

    void showError(const std::string& msg)
    {
        core_.showError(msg);
    }

    void init_cb()
    {
        core_.initCallback();
    }

    void event_cb(const sapp_event* event)
    {
        core_.eventCallback(event);
    }

    void frame_cb()
    {
        core_.frameCallback();
    }

    void cleanup_cb()
    {
        core_.cleanupCallback();
    }
}