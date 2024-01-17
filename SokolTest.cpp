#include "Application.h"
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdexcept>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    Application app;

    try
    {
        if (app.init()) {
            app.run();
        }
    }
    catch (const std::exception& ex) {
        app.showError(ex.what());
    }

    app.free();
    return 0;
}