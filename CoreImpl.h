#pragma once
#include "CoreCallbacks.h"
#include <thread>
#include <atomic>
#include <memory>

struct sapp_event;

namespace Core
{
    typedef struct InitParams {
        void (*init_cb)(void); 
        void (*frame_cb)(void);
        void (*cleanup_cb)(void);
        void (*event_cb)(const sapp_event*);
        int windowWidth;
        int windowHeight;
        int sampleCount;
    } InitParams;

    class CoreImpl
    {
    public:
        CoreImpl();
        ~CoreImpl();

        bool init(const InitParams& params);
        void free();

        void initCallback();
        void eventCallback(const sapp_event* event);
        void frameCallback();
        void cleanupCallback();

        void setKeyDownCallback(KeyDownCallback cb);
        void setCloseAppCallback(SimpleCallback cb);
        void showError(const std::string& msg);

    private:
        std::atomic_bool isInit_;
        std::atomic_bool initDone_;
        std::atomic_bool isClean_;
        std::thread mainThread_;
        KeyDownCallback keyDownCb_;
        SimpleCallback  closeAppCb_;
    };
}

