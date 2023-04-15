#include "UI.h"
#include "Drawing.h"
#include "../resource.h"

#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "utility.h"
#include "timer.hpp"

ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;
static bool                     g_wndMinimized = false;
bool UI::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const UINT createDeviceFlags = 0;
    
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UI::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr)
    {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void UI::CleanupRenderTarget()
{
    if (pMainRenderTargetView)
    {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void UI::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }

    if (pd3dDeviceContext)
    {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }

    if (pd3dDevice)
    {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

#define ID_TRAY_APP_ICON    5000
#define WM_TRAYICON         (WM_USER + 1)
#define IDM_SHOW            100
#define IDM_EXIT            101

LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
        {
            // 加载图标资源
            HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
            HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
            HICON hIconSmall = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
            
            // 设置窗口图标
            SendMessage(hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
            SendMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIconSmall));
        }
        break;
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        if (wParam == SIZE_MINIMIZED)
        {
            g_wndMinimized = true;
        }
        else if (wParam == SIZE_RESTORED)
        {
            g_wndMinimized = false;
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    case WM_TRAYICON:
        if (wParam == ID_TRAY_APP_ICON && lParam == WM_RBUTTONUP)
        {
            // 创建上下文菜单
            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, IDM_SHOW, L"&Show");
            AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"&Exit");

            // 关联上下文菜单
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hWnd);
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
            PostMessage(hWnd, WM_NULL, 0, 0);
            DestroyMenu(hMenu);
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_SHOW:
            ShowWindow(hWnd, SW_SHOWNORMAL);
	        ::SetWindowPos(hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        }
        break;

    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


void UI::Render()
{
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, _T("ImGui Standalone"), nullptr };
    wc.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1)); // 设置默认图标
    wc.hIconSm = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1)); // 设置小图标


    ::RegisterClassEx(&wc);
    ::SetProcessDPIAware();
    const HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("ImGui Standalone"), WS_OVERLAPPEDWINDOW, 100, 100, 900, 600, NULL, NULL, wc.hInstance, NULL);


    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
    // 创建托盘图标
    NOTIFYICONDATA nidApp = {};
    nidApp.cbSize = sizeof(NOTIFYICONDATA);
    nidApp.hWnd = hwnd;
    nidApp.uID = ID_TRAY_APP_ICON;
    nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nidApp.uCallbackMessage = WM_TRAYICON;
    nidApp.hIcon = hIcon;
    lstrcpyn(nidApp.szTip, L"My App", sizeof(nidApp.szTip));
    Shell_NotifyIcon(NIM_ADD, &nidApp);


    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigViewportsNoAutoMerge = true;

    int show_mode = SW_SHOWNORMAL;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        show_mode = SW_HIDE;
    }

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 4.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui::GetIO().IniFilename = nullptr;

    Drawing::set_style();
    Drawing::load_font(18);


    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool bDone = false;

    //加载图片
    Drawing::click_image_id = ImageFromFile(L"pic\\卡牌.jpg", UI::pd3dDevice);
    //加载动图
    imgui_stbi__load_gif(L"pic\\run.gif", UI::pd3dDevice, Drawing::run_gif);
    std::shared_ptr<Timer> t(new Timer());
    t->setInterval([=]() 
    {
        Drawing::run_gif.current_index++;
        if (Drawing::run_gif.current_index == Drawing::run_gif.srv_array.size() - 1)
        {
            Drawing::run_gif.current_index = 0;
        }
    }, 200); 

    //背景图片
    Drawing::back_image_id = ImageFromFile(L"pic\\back.jpg", UI::pd3dDevice);
    
    ImageGetSize(Drawing::back_image_id, &Drawing::back_image_size.w, &Drawing::back_image_size.h);

    ::ShowWindow(hwnd, show_mode);
	::SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
    //::UpdateWindow(hwnd);

    while (!bDone)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bDone = true;
        }
        if (bDone)
            break;

        // Don't render if the window is minimized
        if (g_wndMinimized)
        {
            Sleep(1); // Let the processor sleep a bit
            continue;
        }

        //解决 最小化时, CPU GPU利用率高的问题
        //ocornut建议用上面方法
		//if (ImGui::GetIO().Framerate > 61.f) 
        //{
			//std::this_thread::sleep_for(std::chrono::milliseconds(200)); // drop to 5 FPS when invisible
		//}

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            Drawing::Draw();
        }
        ImGui::EndFrame();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        pSwapChain->Present(1, 0);

        #ifndef _WINDLL
        if (!Drawing::isActive())
        {
            break;
        }
        #endif
    }

    // 释放托盘图标
    Shell_NotifyIcon(NIM_DELETE, &nidApp);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    #ifdef _WINDLL
    ExitThread(0);
    #endif
}
