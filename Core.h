#pragma once
#include <string>
#include <functional>


namespace Core
{
    using KeyDownCallback = std::function<void(int)>;

    bool init(int windowWidth, int windowHeight, int sampleCount);
    void setKeyDownCallback(KeyDownCallback cb);
    void showError(const std::string& msg);
};

