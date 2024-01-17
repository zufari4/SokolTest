#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <memory>

struct sapp_event;

namespace Core
{
    using KeyDownCallback = std::function<void(int)>;

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
        void initCallback();
        void eventCallback(const sapp_event* event);
        void frameCallback();
        void cleanupCallback();

        void setKeyDownCallback(KeyDownCallback cb);
        void showError(const std::string& msg);

    private:
        KeyDownCallback keyDownCb_;
        std::thread mainThread_;
        std::atomic_bool isInit_;
        std::atomic_bool initDone_;
    };
}

