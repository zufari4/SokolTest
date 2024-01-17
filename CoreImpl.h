#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <memory>

struct sapp_event;

namespace Core
{
    using KeyDownCallback = std::function<void(int)>;
    using SimpleCallback = std::function<void(void)>;

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
        void requestQuit();

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

