#include "Core.h"
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <thread>
#include <chrono>

bool bWork = true;

void onKeyDown(int key)
{
    if (key == 256) {
        bWork = false;
    }
}

void onAppClose()
{
    bWork = false;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    if (Core::init(true, 1024, 768, 8, false)) {
        Core::setKeyDownCallback(onKeyDown);
        Core::setCloseAppCallback(onAppClose);

        while (bWork)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    Core::free();
    return 0;
}