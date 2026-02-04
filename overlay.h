#define IMGUI_DEFINE_MATH_OPERATORS

#include <D3DX11core.h>
#include <D3DX11tex.h>  
#include <D3D11.h>
#include <dxgi.h>
#include <dwmapi.h>
#include <thread>
#include <shellapi.h>
#include <tlhelp32.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../Loading/driver.hpp"
#include "spoof.h"
#include "actorloop.h"
#include "settings.h"
#include "slowdown.h"
#include "../blur/blur.h"
#include "../gui.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")

void draw_menu();
void watermark(ImDrawList* draw_list);

DWORD imgui_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

HWND hwnd;
RECT rc;

static HWND g_hOSK = nullptr;
static bool g_bOSKInitialized = false;

static DWORD g_target_process_id = 0;
static DWORD g_main_process_id = 0;
static std::thread g_process_monitor_thread;
static std::thread g_main_monitor_thread;
static bool g_should_exit = false;

static HANDLE g_exit_event = nullptr;
static HANDLE g_cleanup_mutex = nullptr;

void Error()
{
    getchar();
    exit(-1);
}

ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

ID3D11Device* d3d_device;
ID3D11DeviceContext* d3d_device_ctx;
IDXGISwapChain* d3d_swap_chain;
ID3D11RenderTargetView* d3d_render_target;
D3DPRESENT_PARAMETERS d3d_present_params;

static ID3D11ShaderResourceView* users = nullptr;

typedef struct _Header
{
    UINT Magic;
    UINT FrameCount;
    UINT NoClue;
    UINT Width;
    UINT Height;
    BYTE Buffer[1];
} Header;

namespace overlay
{
    void CloseOSK();
    void PerformCleanup();

    BOOL WINAPI ConsoleHandler(DWORD dwType)
    {
        switch (dwType)
        {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        g_should_exit = true;
            if (g_exit_event) SetEvent(g_exit_event);
            CloseOSK();
            Sleep(1000);
            return TRUE;
        }
        return FALSE;
    }

    void PerformCleanup()
    {
        if (g_cleanup_mutex) {
            WaitForSingleObject(g_cleanup_mutex, INFINITE);
        }
        if (!g_should_exit) {
            g_should_exit = true;
        }
        CloseOSK();
        if (g_process_monitor_thread.joinable()) {
            g_process_monitor_thread.join();
        }
        if (g_main_monitor_thread.joinable()) {
            g_main_monitor_thread.join();
        }
        if (ImGui::GetCurrentContext()) {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
        if (d3d_render_target) {
            d3d_render_target->Release();
            d3d_render_target = nullptr;
        }
        if (d3d_swap_chain) {
            d3d_swap_chain->Release();
            d3d_swap_chain = nullptr;
        }
        if (d3d_device_ctx) {
            d3d_device_ctx->Release();
            d3d_device_ctx = nullptr;
        }
        if (d3d_device) {
            d3d_device->Release();
            d3d_device = nullptr;
        }
        if (globals.window_handle) {
            DestroyWindow(globals.window_handle);
            globals.window_handle = nullptr;
        }

        gui::Shutdown();

        if (g_cleanup_mutex) {
            ReleaseMutex(g_cleanup_mutex);
        }
        PostQuitMessage(0);
        exit(0);
    }

    void CloseOSK()
    {
        if (g_hOSK && IsWindow(g_hOSK))
        {
            PostMessage(g_hOSK, WM_CLOSE, 0, 0);
            Sleep(500);
            if (IsWindow(g_hOSK))
            {
                DWORD oskProcessId = 0;
                GetWindowThreadProcessId(g_hOSK, &oskProcessId);

                if (oskProcessId != 0)
                {
                    HANDLE hOSKProcess = OpenProcess(PROCESS_TERMINATE, FALSE, oskProcessId);
                    if (hOSKProcess)
                    {
                        TerminateProcess(hOSKProcess, 0);
                        CloseHandle(hOSKProcess);
                    }
                }
            }

            g_hOSK = nullptr;
        }
        HWND hOskWindow = nullptr;
        do {
            hOskWindow = FindWindow(OBF(L"OSKMainClass"), nullptr);
            if (hOskWindow)
            {
                PostMessage(hOskWindow, WM_CLOSE, 0, 0);
                Sleep(100);
            }
        } while (hOskWindow);
    }

    void MainProcessMonitor()
    {
        while (!g_should_exit)
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, g_main_process_id);

            if (hProcess == NULL)
            {
                g_should_exit = true;
                if (g_exit_event) SetEvent(g_exit_event);
                PerformCleanup();
                break;
            }

            DWORD exitCode = 0;
            if (GetExitCodeProcess(hProcess, &exitCode))
            {
                if (exitCode != STILL_ACTIVE)
                {
                    CloseHandle(hProcess);
                    g_should_exit = true;
                    if (g_exit_event) SetEvent(g_exit_event);
                    PerformCleanup();
                    break;
                }
            }
            CloseHandle(hProcess);
            Sleep(500);
        }
    }

    void ProcessMonitor()
    {
        while (!g_should_exit)
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, g_target_process_id);

            if (hProcess == NULL)
            {
                g_should_exit = true;
                if (g_exit_event) SetEvent(g_exit_event);
                PerformCleanup();
                break;
            }

            DWORD exitCode = 0;
            if (GetExitCodeProcess(hProcess, &exitCode))
            {
                if (exitCode != STILL_ACTIVE)
                {
                    CloseHandle(hProcess);
                    g_should_exit = true;
                    if (g_exit_event) SetEvent(g_exit_event);
                    PerformCleanup();
                    break;
                }
            }
            CloseHandle(hProcess);
            Sleep(1000);
        }
    }

    void DisableOSKContent()
    {
        if (g_hOSK && IsWindow(g_hOSK))
        {
            EnumChildWindows(g_hOSK, [](HWND hwnd, LPARAM) -> BOOL {
                ShowWindow(hwnd, SW_HIDE);
                SetWindowTextA(hwnd, OBF(""));
                SetWindowTextW(hwnd, OBF(L""));
                SetWindowPos(hwnd, HWND_BOTTOM, -2000, -2000, 1, 1, SWP_NOACTIVATE | SWP_NOZORDER);
                return TRUE;
                },
                0);
            SetWindowTextA(g_hOSK, OBF(""));
            SetWindowTextW(g_hOSK, OBF(L""));
        }
    }

    void EnsureOSKRunning()
    {
        if (!g_hOSK || !IsWindow(g_hOSK))
        {
            bool osk_launched = false;

            HINSTANCE result = ShellExecuteA(nullptr, OBF("open"), OBF("osk.exe"), nullptr, nullptr, SW_HIDE);
            if ((intptr_t)result > 32) {
                osk_launched = true;
            }
            if (!osk_launched) {
                STARTUPINFOA si = { 0 };
                PROCESS_INFORMATION pi = { 0 };
                si.cb = sizeof(si);
                si.dwFlags = STARTF_USESHOWWINDOW;
                si.wShowWindow = SW_HIDE;

                char oskPath[MAX_PATH];
                GetSystemDirectoryA(oskPath, MAX_PATH);
                strcat_s(oskPath, OBF("\\osk.exe"));

                if (CreateProcessA(oskPath, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                    osk_launched = true;
                }
            }
            if (!osk_launched) {
                char systemPath[MAX_PATH];
                GetSystemDirectoryA(systemPath, MAX_PATH);
                strcat_s(systemPath, OBF("\\osk.exe"));

                HINSTANCE result2 = ShellExecuteA(nullptr, OBF("open"), systemPath, nullptr, nullptr, SW_HIDE);
                if ((intptr_t)result2 > 32) {
                    osk_launched = true;
                }
            }
            if (!osk_launched) {
                if (WinExec(OBF("osk.exe"), SW_HIDE) > 31) {
                    osk_launched = true;
                }
            }
            for (int i = 0; i < 50 && !g_hOSK; i++) {
                Sleep(100);

                g_hOSK = FindWindowA(OBF("OSKMainClass"), nullptr);
                if (!g_hOSK) {
                    g_hOSK = FindWindowW(OBF(L"OSKMainClass"), nullptr);
                }
                if (!g_hOSK) {
                    g_hOSK = FindWindowA(OBF("OSK"), nullptr);
                }
                if (!g_hOSK) {
                    g_hOSK = FindWindowA(OBF("On-Screen Keyboard"), nullptr);
                }
                if (!g_hOSK) {
                    g_hOSK = FindWindowExA(NULL, NULL, OBF("OSKMainClass"), NULL);
                }
            }
            if (!g_hOSK) {
                EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
                    char className[256];
                    char windowText[256];
                    GetClassNameA(hwnd, className, sizeof(className));
                    GetWindowTextA(hwnd, windowText, sizeof(windowText));

                    if (strstr(className, OBF("OSK")) != nullptr ||
                        strstr(windowText, OBF("On-Screen Keyboard")) != nullptr ||
                        strstr(windowText, OBF("Bildschirmtastatur")) != nullptr) {
                        g_hOSK = hwnd;
                        return FALSE;
                    }
                    return TRUE;
                    }, 0);
            }
        }
    }

    void LoadTextures()
    {
    }

    bool InitImgui()
    {
        DXGI_SWAP_CHAIN_DESC swap_chain_description;
        ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
        swap_chain_description.BufferCount = 2;
        swap_chain_description.BufferDesc.Width = 0;
        swap_chain_description.BufferDesc.Height = 0;
        swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
        swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_description.OutputWindow = globals.window_handle;
        swap_chain_description.SampleDesc.Count = 1;
        swap_chain_description.SampleDesc.Quality = 0;
        swap_chain_description.Windowed = 1;
        swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL d3d_feature_lvl;
        const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

        if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, d3d_feature_array, 2,
            D3D11_SDK_VERSION, &swap_chain_description, &d3d_swap_chain, &d3d_device, &d3d_feature_lvl, &d3d_device_ctx)))
            return false;

        ID3D11Texture2D* pBackBuffer;
        d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        d3d_device->CreateRenderTargetView(pBackBuffer, NULL, &d3d_render_target);
        pBackBuffer->Release();

        g_pd3dDevice = d3d_device;
        g_pd3dDeviceContext = d3d_device_ctx;
        g_pSwapChain = d3d_swap_chain;
        g_mainRenderTargetView = d3d_render_target;

        if (!gui::Initialize(globals.window_handle, d3d_device, d3d_device_ctx, d3d_swap_chain)) {
            return false;
        }

        return true;
    }

    auto hijack() -> bool {
        HWND hWnd = GetConsoleWindow();
        SPOOF;

        globals.window_handle = nullptr;

        EnsureOSKRunning();

        int attempts = 0;
        while (!globals.window_handle && attempts < 100) {
            globals.window_handle = FindWindowA(OBF("OSKMainClass"), nullptr);
            if (!globals.window_handle) {
                globals.window_handle = FindWindowW(OBF(L"OSKMainClass"), nullptr);
            }
            if (!globals.window_handle) {
                globals.window_handle = FindWindowA(OBF("OSK"), nullptr);
            }
            if (!globals.window_handle) {
                globals.window_handle = FindWindowA(OBF("On-Screen Keyboard"), nullptr);
            }
            if (!globals.window_handle) {
                globals.window_handle = FindWindowExA(NULL, NULL, OBF("OSKMainClass"), NULL);
            }
            if (!globals.window_handle && attempts % 20 == 19) {
                EnsureOSKRunning();
            }
            if (!globals.window_handle) {
                Sleep(100);
                attempts++;
            }
        }
        if (!globals.window_handle) {
            MessageBoxA(nullptr, OBF("Overlay cant start..."), OBF("ERROR"), MB_OK | MB_ICONERROR);
            return false;
        }
        if (hWnd) {
            ShowWindow(hWnd, SW_SHOW);
        }
        RECT desktop_rect;
        GetWindowRect(GetDesktopWindow(), &desktop_rect);
        globals.ScreenWidth = desktop_rect.right - desktop_rect.left;
        globals.ScreenHeight = desktop_rect.bottom - desktop_rect.top;

        SetWindowLong(globals.window_handle, GWL_STYLE, WS_POPUP);

        LONG exStyle = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;
        SetWindowLong(globals.window_handle, GWL_EXSTYLE, exStyle);

        SetWindowPos(globals.window_handle, HWND_TOPMOST,
            0, 0, globals.ScreenWidth, globals.ScreenHeight,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOACTIVATE);

        SPOOF_CALL(SetLayeredWindowAttributes)(globals.window_handle, RGB(0, 0, 0), 240, LWA_ALPHA);

        MARGINS margin = { -1, -1, -1, -1 };
        SPOOF_CALL(DwmExtendFrameIntoClientArea)(globals.window_handle, &margin);

        DisableOSKContent();

        SPOOF_CALL(ShowWindow)(globals.window_handle, SW_SHOW);
        SPOOF_CALL(UpdateWindow)(globals.window_handle);

        return true;
    }

    void draw_menu() {
        if (!globals.show_menu)
        return;
        gui::RenderMainUI();
    }

    void menu_loop() {
        if (g_should_exit || (g_exit_event && WaitForSingleObject(g_exit_event, 0) == WAIT_OBJECT_0)) {
            PerformCleanup();
            return;
        }

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            globals.show_menu = !globals.show_menu;
            gui::ToggleMenu();
        }

        if (globals.show_menu) {
            LONG exStyle = GetWindowLong(globals.window_handle, GWL_EXSTYLE);
            exStyle &= ~WS_EX_TRANSPARENT;
            SetWindowLong(globals.window_handle, GWL_EXSTYLE, exStyle);

            SetLayeredWindowAttributes(globals.window_handle, RGB(0, 0, 0), 255, LWA_ALPHA);

            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = true;
            io.WantCaptureKeyboard = true;

            draw_menu();
        }
        else {
            LONG exStyle = GetWindowLong(globals.window_handle, GWL_EXSTYLE);
            exStyle |= WS_EX_TRANSPARENT;
            SetWindowLong(globals.window_handle, GWL_EXSTYLE, exStyle);
            SetLayeredWindowAttributes(globals.window_handle, RGB(0, 0, 0), 240, LWA_ALPHA);
            DisableOSKContent();

            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = false;
            io.WantCaptureKeyboard = false;
        }
    }

    void render_radar(ImDrawList* draw_list)
    {
        if (!radar.enable) return;

        ImVec2 radar_pos = ImVec2(radar.pos_x, radar.pos_y);
        ImVec2 radar_size = ImVec2(radar.size, radar.size);
        ImVec2 center = ImVec2(radar_pos.x + radar_size.x / 2, radar_pos.y + radar_size.y / 2);
        float radius = radar_size.x / 2;

        ImU32 bgColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.35f));
        ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.6f, 1.0f, 0.9f));
        ImU32 faintLine = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.07f));
        ImU32 pointColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.3f, 0.8f, 1.0f, 1.0f));
        ImU32 fovColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.6f, 1.0f, 0.10f));

        draw_list->AddRectFilled(radar_pos, ImVec2(radar_pos.x + radar_size.x, radar_pos.y + radar_size.y), bgColor);
        draw_list->AddRect(radar_pos, ImVec2(radar_pos.x + radar_size.x, radar_pos.y + radar_size.y), borderColor, 0.0f, 0, 1.5f);

         
        const int grid_lines = 10;
        float cell_size = radar_size.x / grid_lines;
        for (int i = 1; i < grid_lines; ++i) {
            float x = radar_pos.x + i * cell_size;
            float y = radar_pos.y + i * cell_size;
            draw_list->AddLine(ImVec2(x, radar_pos.y), ImVec2(x, radar_pos.y + radar_size.y), faintLine);
            draw_list->AddLine(ImVec2(radar_pos.x, y), ImVec2(radar_pos.x + radar_size.x, y), faintLine);
        }

        float fov_half_angle_deg = 45.0f;
        float fov_half_rad = fov_half_angle_deg * IM_PI / 180.0f;
        float fov_length = radius * 1.414f; 

        uintptr_t camera_manager = kernel->read_t<uintptr_t>(CachePointers.PlayerController + CAMERA_MANAGER);
        fvector cam_rot = kernel->read_t<fvector>(camera_manager + CAMERA_ROTATION);
        float yaw_rad = (cam_rot.y - 90.0f) * IM_PI / 180.0f;
        float left_angle = yaw_rad - fov_half_rad;
        float right_angle = yaw_rad + fov_half_rad;

        ImVec2 left_point = ImVec2(center.x + cosf(left_angle) * fov_length, center.y + sinf(left_angle) * fov_length);
        ImVec2 right_point = ImVec2(center.x + cosf(right_angle) * fov_length, center.y + sinf(right_angle) * fov_length);
        draw_list->AddTriangleFilled(center, left_point, right_point, fillColor);
        draw_list->AddLine(center, left_point, fovColor, 1.5f);
        draw_list->AddLine(center, right_point, fovColor, 1.5f);

        draw_list->AddCircleFilled(center, 3.0f, pointColor, 16);
        const char* distance_text = "(4.6m)";
        ImVec2 text_size = ImGui::CalcTextSize(distance_text);
        ImVec2 text_pos = ImVec2(center.x - text_size.x / 2, center.y + 6);
        draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), distance_text);

        uintptr_t game_state = kernel->read_t<uintptr_t>(CachePointers.UWorld + GAME_STATE);
        uintptr_t player_array = kernel->read_t<uintptr_t>(game_state + PLAYER_ARRAY);
        int player_count = kernel->read_t<int>(game_state + (PLAYER_ARRAY + sizeof(uintptr_t)));

        for (int i = 0; i < player_count; i++) {
            uintptr_t player_state = kernel->read_t<uintptr_t>(player_array + i * sizeof(uintptr_t));
            if (!player_state) continue;
            uintptr_t current_actor = kernel->read_t<uintptr_t>(player_state + PAWN_PRIVATE);
            if (!current_actor) continue;
            uintptr_t skeletalmesh = kernel->read_t<uintptr_t>(current_actor + MESH);
            if (!skeletalmesh) continue;

            bool isLocalPlayer = (current_actor == CachePointers.AcknownledgedPawn);
            int enemyTeamIndex = kernel->read_t<char>(player_state + TEAM_INDEX);
            bool isTeammate = (enemyTeamIndex == CachePointers.TeamIndex);
            bool isPlayerVisible = IsVisible(skeletalmesh);

            fvector player_pos = game_helper.getsocketlocation(skeletalmesh, 0);
            int radar_x, radar_y;
            if (isLocalPlayer) {
                radar_x = center.x;
                radar_y = center.y;
            }
            else {
                decryption.CalcRadarPoint(player_pos, radar_x, radar_y);
            }

            ImU32 player_color;
            if (isLocalPlayer)
                player_color = colorss.VisibleBoxColor;
            else if (isTeammate)
                player_color = isPlayerVisible ? colorss.TeammateVisibleColor : colorss.TeammateInvisibleColor;
            else
                player_color = isPlayerVisible ? colorss.VisibleBoxColor : colorss.InvisibleBoxColor;

            float dx = player_pos.x - camera_postion.location.x;
            float dy = player_pos.y - camera_postion.location.y;
            float angle_to_target = atan2f(dy, dx);
            float relative_angle = angle_to_target - yaw_rad;

            while (relative_angle < -IM_PI) relative_angle += 2 * IM_PI;
            while (relative_angle > IM_PI) relative_angle -= 2 * IM_PI;

            if (fabsf(relative_angle) < fov_half_rad) {

                if (radar.radar_style == 0 && radar.show_dots) {
                    ImVec2 dot_pos = ImVec2(radar_x, radar_y);
                    float dx = dot_pos.x - center.x;
                    float dy = dot_pos.y - center.y;
                    float distance_from_center = sqrtf(dx * dx + dy * dy);
                    float max_distance = radius - radar.dot_size;
                    if (distance_from_center > max_distance && distance_from_center > 0.0f) {
                        float scale = max_distance / distance_from_center;
                        dot_pos.x = center.x + dx * scale;
                        dot_pos.y = center.y + dy * scale;
                    }
                    draw_list->AddCircleFilled(dot_pos, radar.dot_size, player_color);
                }

                if (radar.radar_style == 1 && radar.show_arrows && !isLocalPlayer) {
                    ImVec2 arrow_pos = ImVec2(radar_x, radar_y);
                    float arrow_size = radar.arrow_size;
                    float dx_center = arrow_pos.x - center.x;
                    float dy_center = arrow_pos.y - center.y;
                    float distance_from_center = sqrtf(dx_center * dx_center + dy_center * dy_center);
                    float max_distance = radius - arrow_size;
                    if (distance_from_center > max_distance && distance_from_center > 0.0f) {
                        float scale = max_distance / distance_from_center;
                        arrow_pos.x = center.x + dx_center * scale;
                        arrow_pos.y = center.y + dy_center * scale;
                    }

                    ImVec2 p1 = ImVec2(arrow_pos.x + cosf(relative_angle) * arrow_size, arrow_pos.y + sinf(relative_angle) * arrow_size);
                    ImVec2 p2 = ImVec2(arrow_pos.x + cosf(relative_angle - 2.5f) * arrow_size * 0.6f, arrow_pos.y + sinf(relative_angle - 2.5f) * arrow_size * 0.6f);
                    ImVec2 p3 = ImVec2(arrow_pos.x + cosf(relative_angle + 2.5f) * arrow_size * 0.6f, arrow_pos.y + sinf(relative_angle + 2.5f) * arrow_size * 0.6f);
                    draw_list->AddTriangleFilled(p1, p2, p3, player_color);
                }
            }
        }
    }


    void draw()
    {
        if (g_should_exit) return;

        gui::NewFrame();

        auto draw_list = ImGui::GetBackgroundDrawList();

        cache_loop();
        actor_loop();
        render_radar(draw_list);
        render_crosshair();

        menu_loop();

        gui::Render();

        D3D11_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.MultisampleEnable = FALSE;
        rasterizer_desc.AntialiasedLineEnable = FALSE;
        ID3D11RasterizerState* rasterizer_state = nullptr;
        d3d_device->CreateRasterizerState(&rasterizer_desc, &rasterizer_state);
        d3d_device_ctx->RSSetState(rasterizer_state);

        const float clear_color_with_alpha[4] = {
            0.0f, 0.0f, 0.0f, 0.0f
        };

        d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
        d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color_with_alpha);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (!globals.vsync) {
            d3d_swap_chain->Present(0, 0);
        }
        else {
            d3d_swap_chain->Present(1, 0);
        }

        if (rasterizer_state) {
            rasterizer_state->Release();
        }
    }

    bool render()
    {
        MSG msg = { NULL };
        ZeroMemory(&msg, sizeof(MSG));

        while (msg.message != WM_QUIT && !g_should_exit && (!g_exit_event || WaitForSingleObject(g_exit_event, 0) != WAIT_OBJECT_0))
        {
            UpdateWindow(globals.window_handle);
            ShowWindow(globals.window_handle, SW_SHOW);

            if (PeekMessageA(&msg, globals.window_handle, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            ImGuiIO& io = ImGui::GetIO();
            io.DeltaTime = 1.0f / 60.0f;

            if (globals.show_menu) {
                POINT p_cursor;
                GetCursorPos(&p_cursor);
                io.MousePos.x = p_cursor.x;
                io.MousePos.y = p_cursor.y;

                io.WantCaptureMouse = true;
                io.WantCaptureKeyboard = true;

                if (GetAsyncKeyState(VK_LBUTTON)) {
                    io.MouseDown[0] = true;
                    io.MouseClicked[0] = true;
                    io.MouseClickedPos[0].x = io.MousePos.x;
                    io.MouseClickedPos[0].y = io.MousePos.y;
                }
                else {
                    io.MouseDown[0] = false;
                }
            }
            else {
                io.WantCaptureMouse = false;
                io.WantCaptureKeyboard = false;
                io.WantTextInput = false;
                io.WantSetMousePos = false;
                io.MouseDown[0] = false;
                io.MouseDown[1] = false;
                io.MouseDown[2] = false;
                io.MouseDown[3] = false;
                io.MouseDown[4] = false;
                io.MouseClicked[0] = false;
                io.MouseClicked[1] = false;
                io.MouseClicked[2] = false;
                io.MouseClicked[3] = false;
                io.MouseClicked[4] = false;
                io.MousePos.x = -FLT_MAX;
                io.MousePos.y = -FLT_MAX;
                io.MouseDelta.x = 0.0f;
                io.MouseDelta.y = 0.0f;
            }
            draw();
        }
        PerformCleanup();
        return true;
    }

    void start()
    {
        g_exit_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        g_cleanup_mutex = CreateMutex(nullptr, FALSE, nullptr);

        SetConsoleCtrlHandler(ConsoleHandler, TRUE);

        g_main_process_id = GetCurrentProcessId();
        g_target_process_id = kernel->get_process_pid(L"FortniteClient-Win64-Shipping.exe");

        g_process_monitor_thread = std::thread(ProcessMonitor);
        g_main_monitor_thread = std::thread(MainProcessMonitor);

        if (!hijack()) {
            g_should_exit = true;
            PerformCleanup();
            return;
        }

        if (!InitImgui()) {
            g_should_exit = true;
            PerformCleanup();
            return;
        }

        std::thread([&]() {
            while (!g_should_exit && (!g_exit_event || WaitForSingleObject(g_exit_event, 0) != WAIT_OBJECT_0)) {
                camera_postion = game_helper.get_camera();
                Sleep(16);
            }
            }).detach();

        render();

        if (g_exit_event) {
            CloseHandle(g_exit_event);
        }
        if (g_cleanup_mutex) {
            CloseHandle(g_cleanup_mutex);
        }
    }
}
