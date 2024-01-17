#pragma once
#include <string>

struct sapp_event;

class Application
{
public:
    Application();
    ~Application();
    bool init();
    void run();
    void free();
    void showError(const std::string& msg);

    void initCallback();
    void eventCallback(const sapp_event* event);
    void frameCallback();
};

