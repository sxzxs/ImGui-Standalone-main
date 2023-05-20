#include "UI.h"
#include "spd_logger.h"
#include "configger.h"
#include <windows.h>
#include "global.h"
#include "exports.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "AutoHotkey.lib")
#ifdef _WINDLL

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)UI::Render, nullptr, NULL, nullptr);
        return TRUE;
    }

    return TRUE;
}

#else

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    //Alloc console
    AllocConsole();
    TRACE("Start imgui project...");
    Global::h_console = GetConsoleWindow();
    ShowWindow(Global::h_console, SW_HIDE);


    //配置文件加载
    Configger::instance()->set_config_path(u8"config/settings.json");
    Configger::instance()->load();
    auto &config = Configger::instance()->config();
    Configger::instance()->save();
    auto key = config["key"];
    TRACE(key);

    //开启ahk线程
	Global::ahk_thread_id = NewThread(L"#include ahk-script/hotkey.ah2", L"123");
    while (!ahkReady(Global::ahk_thread_id)){}

    int x_position{ 100 };
    int y_position{ 100 };
	ahkFunction((wchar_t *)L"highlight_position", (wchar_t *)to_wstring(x_position).c_str(), (wchar_t *)to_wstring(y_position).c_str(), NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, Global::ahk_thread_id);

    UI::Render();

    return 0;
}

#endif