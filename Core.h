#pragma once
#include "CoreCallbacks.h"
#include <string>


namespace Core
{
    bool init(bool fullscreen, int windowWidth, int windowHeight, int sampleCount, bool useVsync);
    void free();
    void setKeyDownCallback(KeyDownCallback cb);
    void setCloseAppCallback(SimpleCallback cb);
    void showError(const std::string& msg);
};

