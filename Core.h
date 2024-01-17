#pragma once
#include <string>
#include <functional>


namespace Core
{
    using SimpleCallback = std::function<void(void)>;
    using KeyDownCallback = std::function<void(int)>;

    bool init(int windowWidth, int windowHeight, int sampleCount);
    void free();
    void setKeyDownCallback(KeyDownCallback cb);
    void setCloseAppCallback(SimpleCallback cb);
    void showError(const std::string& msg);
};

