#pragma once
#include <functional>

namespace Core
{
    using SimpleCallback  = std::function<void(void)>;
    using KeyDownCallback = std::function<void(int)>;
}