#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_freetype.h>
#include <d3d11.h>
#include <string>
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

struct HWND__;
typedef HWND__* HWND;

namespace gui {
    struct s_tab;
    class c_tabs;
}

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;

namespace gui
{
    namespace font
    {
        extern ImFont* regular_m;
        extern ImFont* regular_s;
        extern ImFont* regular_l;
        extern ImFont* s_inter_semibold;
        extern ImFont* inter_semibold_f;
        extern ImFont* inter_medium;

        extern ImFont* platform_icons;
    }

    namespace texture
    {
        extern ID3D11ShaderResourceView* logo_pic;
    }

    namespace menu_anim
    {
        extern bool active;
        extern float size_delta;
        extern float fixed_size;
        extern bool changing_size;
    }

    namespace tab_info
    {
        extern bool is_subtab;
        extern bool tab_changing;
        extern int wanted_id;
        extern int wanted_subid;
    }

    namespace c
    {
        namespace bg {
            extern ImVec2 size;
            extern ImVec2 padding;
            extern float rounding;
        }

        namespace anim {
            extern float speed;
            extern ImU32 active;
        }

        namespace label {
            extern ImU32 active;
        }

        namespace window_bg_color {
            extern ImColor value;
        }
    }

    extern bool menu_disabled;
    extern int iSubTabs;
    extern int iTabs;
    extern bool checkboxes[60];
    extern int slider_int[135];
    extern float color_edit[110][4];
    extern int combo[135];
    extern int keybind[136];
    extern int keybind_mode[136];

    struct s_tab
    {
        const char* header;
        std::vector<const char*> tabs;
    };

    class c_tabs {
    private:
        int current_idx = 0;
        std::vector<s_tab> tab_selection;

    public:
        c_tabs(std::vector<s_tab> tab_info);
        int GetCurrentTab();
        void SetCurrentTab(int v);
        bool IsTabActive(int id);
        void DrawTabs(ImVec2 pos);
    };

    extern c_tabs* p_tabs;

     
    bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context, IDXGISwapChain* swap_chain);
    void Shutdown();

    bool LoadFonts();

    bool LoadTextures();
    void CleanupTextures();

    void NewFrame();
    void Render();

    void SetupStyle();

    void RenderMainUI();

    void PushFont(ImFont* font);
    void PopFont();
    ImDrawList* GetForegroundDrawList();
    ImDrawList* GetWindowDrawList();

    bool BeginWindow(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);
    void EndWindow();

    void ToggleMenu();
    bool IsMenuAnimating();

    int GetCurrentSubTab();
    void SetCurrentSubTab(int tab);
    bool IsTabChanging();
    void SetTabChanging(bool changing);

    void RenderTab1();  
    void RenderTab2();
    void RenderTab3();
}