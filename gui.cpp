#include "gui.h"
#include "font.h"
#include "image.h"
#include "font_defines.h"
#include "images.h"
#include "blur/blur.h"
#include "custom_widgets.hpp"
#include "notifications.h"
#include "Sources/settings.h"
#include "Sources/platform_icons.h"
#include "Sources/ranked_icons.h"

namespace gui::font
{
    ImFont* regular_m = nullptr;
    ImFont* regular_s = nullptr;
    ImFont* regular_l = nullptr;
    ImFont* s_inter_semibold = nullptr;
    ImFont* inter_semibold_f = nullptr;
    ImFont* inter_medium = nullptr;
}

namespace gui::texture
{
    ID3D11ShaderResourceView* logo_pic = nullptr;
}

namespace gui::menu_anim
{
    bool active = true;
    float size_delta = 0.f;
    float fixed_size = 0.f;
    bool changing_size = false;
}

namespace gui::tab_info
{
    bool is_subtab = false;
    bool tab_changing = false;
    int wanted_id = 0;
    int wanted_subid = 0;
}

namespace gui::c
{
    namespace bg {
        ImVec2 size = ImVec2(750, 700);
        ImVec2 padding = ImVec2(10, 10);
        float rounding = 9.f;
    }

    namespace anim {
        float speed = 0.3f;
        ImU32 active = IM_COL32(50, 128, 230, 255);
    }

    namespace label {
        ImU32 active = IM_COL32(240, 240, 255, 255);
    }

    namespace window_bg_color {
        ImColor value = ImColor(10, 10, 13, 255);
    }
}

namespace gui
{
    bool menu_disabled = false;
    int iSubTabs = 1;
    int iTabs = 0;
    bool checkboxes[60] = { false };
    int slider_int[135] = { 0 };
    float color_edit[110][4] = { 0 };
    int combo[135] = { 0 };
    int keybind[136] = { 0 };
    int keybind_mode[136] = { 0 };

    std::vector<s_tab> tabs_info;
    c_tabs* p_tabs = nullptr;
}

gui::c_tabs::c_tabs(std::vector<s_tab> tab_info)
{
    this->current_idx = 0;
    this->tab_selection = tab_info;
}

int gui::c_tabs::GetCurrentTab()
{
    return current_idx;
}

void gui::c_tabs::SetCurrentTab(int v)
{
    this->current_idx = v;
}

bool gui::c_tabs::IsTabActive(int id)
{
    return bool(this->current_idx == id);
}

void gui::c_tabs::DrawTabs(ImVec2 pos)
{
    ImGui::SetCursorPos(ImVec2(20, 115));

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 20));
    ImGui::BeginGroup(); {
        if (custom::Tab(ICON_AIMING_FILL, &tab_info::wanted_id, 0))
        {
            tab_info::is_subtab = false;
            tab_info::tab_changing = true;
            iSubTabs = 1;
        };
        if (custom::Tab(ICON_ANDROID_FILL, &tab_info::wanted_id, 1))
        {
            tab_info::is_subtab = false;
            tab_info::tab_changing = true;
            iSubTabs = 1;
        };
        if (custom::Tab(ICON_BOMB_FILL, &tab_info::wanted_id, 2))
        {
            tab_info::is_subtab = false;
            tab_info::tab_changing = true;
            iSubTabs = 1;
        };
        if (custom::Tab(ICON_PIC_AI_FILL, &tab_info::wanted_id, 3))
        {
            tab_info::is_subtab = false;
            tab_info::tab_changing = true;
            iSubTabs = 1;
        };
        if (custom::Tab(ICON_SWORD_FILL, &tab_info::wanted_id, 4))
        {
            tab_info::is_subtab = false;
            tab_info::tab_changing = true;
            iSubTabs = 1;
        };
        if (custom::Tab(ICON_SETTINGS_6_FILL, &tab_info::wanted_id, 5))
        {
            tab_info::is_subtab = false;
            tab_info::tab_changing = true;
            iSubTabs = 1;
        };

    } ImGui::EndGroup();
    ImGui::PopStyleVar();
}

const char* combo_list[] = { "Peyote #1$29.6m", "Peyote #2$29.6m", "Peyote #3$29.6m", "Peyote #4$29.6m", "Peyote #5$29.6m" };
const char* combo_list2[] = { "Peyote #1", "Peyote #2", "Peyote #3", "Peyote #4", "Peyote #5" };

namespace {
    ImColor GetColorWithAlpha(ImColor color, float alpha)
    {
        return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
    }
}

bool gui::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context, IDXGISwapChain* swap_chain)
{
    g_pd3dDevice = device;
    g_pd3dDeviceContext = device_context;
    g_pSwapChain = swap_chain;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    if (!ImGui_ImplWin32_Init(hwnd))
        return false;

    if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext))
        return false;

    SetupStyle();

    if (!LoadFonts())
        return false;

    p_tabs = new c_tabs(tabs_info);

    return true;
}

void gui::Shutdown()
{
    if (p_tabs) {
        delete p_tabs;
        p_tabs = nullptr;
    }

    CleanupTextures();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool gui::LoadFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;
    io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    static ImWchar icomoon_ranges[] = { 0x1, 0x10FFFD, 0 };
    static ImFontConfig icomoon_config;
    icomoon_config.OversampleH = icomoon_config.OversampleV = 1;
    icomoon_config.MergeMode = true;
    icomoon_config.GlyphOffset.y = 1.7f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 17.f, &icomoon_config, icomoon_ranges);
    font::regular_m = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 13.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icomoon_config.GlyphOffset.y = 4.5f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 25.f, &icomoon_config, icomoon_ranges);

    font::regular_s = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icomoon_config.GlyphOffset.y = 2.5f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 16.f, &icomoon_config, icomoon_ranges);

    font::regular_l = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 30.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icomoon_config.GlyphOffset.y = 3.f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 30.f, &icomoon_config, icomoon_ranges);

    icomoon_config.GlyphOffset.y = 2.5f;
    font::s_inter_semibold = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 24.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 23.f, &icomoon_config, icomoon_ranges);

    font::inter_semibold_f = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icomoon_config.GlyphOffset.y = 4.5f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 21.f, &icomoon_config, icomoon_ranges);

    font::inter_medium = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 23.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icomoon_config.GlyphOffset.y = 4.5f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 26.f, &icomoon_config, icomoon_ranges);

    // Platform Icons 
    if (images::windows_logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, winlogo, sizeof(winlogo), nullptr, nullptr, &images::windows_logo, 0);
    if (images::xbox_logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, xboxlogo, sizeof(xboxlogo), nullptr, nullptr, &images::xbox_logo, 0);
    if (images::ps_logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, pslogo, sizeof(pslogo), nullptr, nullptr, &images::ps_logo, 0);
    if (images::nintendo_logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, nintendologo, sizeof(nintendologo), nullptr, nullptr, &images::nintendo_logo, 0);
    if (images::android_logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, androidlogo, sizeof(androidlogo), nullptr, nullptr, &images::android_logo, 0);
    if (images::ios_logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ioslogo, sizeof(ioslogo), nullptr, nullptr, &images::ios_logo, 0);

    // Rank Icons 
    if (images::unranked == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, unranked, sizeof(unranked), nullptr, nullptr, &images::unranked, 0);
    if (images::bronze1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, bronze1, sizeof(bronze1), nullptr, nullptr, &images::bronze1, 0);
    if (images::bronze2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, bronze2, sizeof(bronze2), nullptr, nullptr, &images::bronze2, 0);
    if (images::bronze3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, bronze3, sizeof(bronze3), nullptr, nullptr, &images::bronze3, 0);
    if (images::silver1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, silver1, sizeof(silver1), nullptr, nullptr, &images::silver1, 0);
    if (images::silver2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, silver2, sizeof(silver2), nullptr, nullptr, &images::silver2, 0);
    if (images::silver3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, silver3, sizeof(silver3), nullptr, nullptr, &images::silver3, 0);
    if (images::gold1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, gold1, sizeof(gold1), nullptr, nullptr, &images::gold1, 0);
    if (images::gold2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, gold2, sizeof(gold2), nullptr, nullptr, &images::gold2, 0);
    if (images::gold3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, gold3, sizeof(gold3), nullptr, nullptr, &images::gold3, 0);
    if (images::platinum1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, platinum1, sizeof(platinum1), nullptr, nullptr, &images::platinum1, 0);
    if (images::platinum2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, platinum2, sizeof(platinum2), nullptr, nullptr, &images::platinum2, 0);
    if (images::platinum3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, platinum3, sizeof(platinum3), nullptr, nullptr, &images::platinum3, 0);
    if (images::diamond1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, diamond1, sizeof(diamond1), nullptr, nullptr, &images::diamond1, 0);
    if (images::diamond2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, diamond2, sizeof(diamond2), nullptr, nullptr, &images::diamond2, 0);
    if (images::diamond3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, diamond3, sizeof(diamond3), nullptr, nullptr, &images::diamond3, 0);
    if (images::elite == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, elite, sizeof(elite), nullptr, nullptr, &images::elite, 0);
    if (images::champion == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, champion, sizeof(champion), nullptr, nullptr, &images::champion, 0);
    if (images::unreal == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, unreal, sizeof(unreal), nullptr, nullptr, &images::unreal, 0);

    return io.Fonts->Build();
}

bool gui::LoadTextures()
{
    static bool image_loaded = false;
    if (image_loaded)
        return true;

    D3DX11_IMAGE_LOAD_INFO iInfo;
    ID3DX11ThreadPump* threadPump = nullptr;

    if (g_pd3dDevice) {
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logotype, sizeof(logotype), &iInfo, threadPump, &texture::logo_pic, 0);
        image_loaded = true;
        return true;
    }

    return false;
}

void gui::CleanupTextures()
{
    if (texture::logo_pic) {
        texture::logo_pic->Release();
        texture::logo_pic = nullptr;
    }
}

void gui::SetupStyle()
{
    ImGuiStyle& s = ImGui::GetStyle();
    s.FramePadding = ImVec2(5, 10);
    s.ItemSpacing = ImVec2(10, 10);
    s.FrameRounding = 2.f;
    s.WindowRounding = 20.f;
    s.WindowBorderSize = 0.f;
    s.PopupBorderSize = 0.f;
    s.WindowPadding = ImVec2(0, 0);
    s.ChildBorderSize = 1.f;
    s.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 0.f);
    s.Colors[ImGuiCol_Separator] = ImVec4(1.f, 1.f, 1.f, 0.2f);
    s.Colors[ImGuiCol_BorderShadow] = ImVec4(0.f, 0.f, 0.f, 0.f);
    s.WindowShadowSize = 0;
    s.PopupRounding = 2.f;
    s.ScrollbarSize = 5;
    s.SeparatorTextPadding = ImVec2(10, 10);
}

void gui::NewFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::Render()
{
    ImGui::Render();
}

void gui::RenderMainUI()
{
    LoadTextures();

    const char* menu_name = "LITHIUM.RIP";
    static ImRect main_window_bb;

    ImGui::BeginDisabled(menu_disabled);
    ImGui::SetNextWindowSize(c::bg::size, ImGuiCond_Once);
    ImGui::Begin(menu_name, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);
    {
        if (menu_anim::fixed_size == 0.f) {
            menu_anim::fixed_size = c::bg::size.y;
            menu_anim::size_delta = 1.f;
        }

        if (menu_anim::active && !menu_anim::changing_size && menu_anim::size_delta == 1.f) {
            menu_anim::fixed_size = c::bg::size.y;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Insert)) {
            if (!menu_anim::active) {
                menu_anim::fixed_size = c::bg::size.y;
            }
            menu_anim::changing_size = true;
            menu_anim::active = !menu_anim::active;
        }

        if (menu_anim::changing_size) {
            ImGui::GetStyle().WindowMinSize = ImVec2(660, 1);
            menu_anim::size_delta = ImLerp(menu_anim::size_delta, menu_anim::active ? 1.f : 0.f, c::anim::speed);

            ImGui::SetWindowSize({ ImGui::GetCurrentWindow()->Size.x, menu_anim::fixed_size * menu_anim::size_delta });

            if (fabs(menu_anim::size_delta - (menu_anim::active ? 1.f : 0.f)) < 0.01f) {
                menu_anim::changing_size = false;
                menu_anim::size_delta = menu_anim::active ? 1.f : 0.f;

                if (menu_anim::active) {
                    menu_anim::fixed_size = c::bg::size.y;
                }
            }
        }
        else if (menu_anim::active) {
            ImGui::SetWindowSize({ ImGui::GetCurrentWindow()->Size.x, menu_anim::fixed_size });
        }

        if (ImGui::GetCurrentWindow()->Size.y > 30)
        {
            if (!menu_anim::changing_size)
                ImGui::GetStyle().WindowMinSize = ImVec2(660, 530);

            main_window_bb = ImGui::GetCurrentWindow()->Rect();
            ImVec2 display_size = ImGui::GetIO().DisplaySize;
            ImDrawList* draw_list = ImGui::GetForegroundDrawList();

            c::anim::speed = ImGui::GetIO().DeltaTime * 12.f;

            const ImVec2& pos = ImGui::GetWindowPos();
            const ImVec2& region = ImGui::GetContentRegionMax();
            const ImVec2& spacing = ImGui::GetStyle().ItemSpacing;

            draw_background_blur(ImGui::GetBackgroundDrawList(), g_pSwapChain, g_pd3dDevice, g_pd3dDeviceContext, pos, pos + region, c::bg::rounding);

            ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + main_window_bb.GetSize(), utils::GetColorWithAlpha(c::window_bg_color::value, c::window_bg_color::value.Value.w * ImGui::GetStyle().Alpha / 3), c::bg::rounding, ImDrawFlags_RoundCornersLeft);
            ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(100, 0), pos + main_window_bb.GetSize(), utils::GetColorWithAlpha(c::window_bg_color::value, c::window_bg_color::value.Value.w * ImGui::GetStyle().Alpha), c::bg::rounding, ImDrawFlags_RoundCornersRight);

            ImGui::GetWindowDrawList()->AddImageRounded(texture::logo_pic, pos + ImVec2(20, 20), pos + ImVec2(80, 80), ImVec2(0, 0), ImVec2(1, 1), ImColor(1.f, 1.f, 1.f, 1.f), 4.f);

            PushFont(font::regular_l);
            p_tabs->DrawTabs(pos);
            PopFont();

            static float tab_alpha = 0.f;
            tab_alpha = ImLerp(tab_alpha, tab_info::tab_changing ? 0.f : 1.f, c::anim::speed);

            static float tab_offset = 0.f;

            if (tab_alpha < 0.1f && tab_info::tab_changing)
            {
                if (!tab_info::is_subtab)
                    p_tabs->SetCurrentTab(tab_info::wanted_id);
                else
                    iSubTabs = tab_info::wanted_subid;

                tab_offset = 350.f;
                tab_info::tab_changing = false;
            }

            tab_offset = ImLerp(tab_offset, 0.f, c::anim::speed * 2);

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha);

            if (p_tabs->IsTabActive(0)) {
                ImGui::SetCursorPos(ImVec2(120.f, 20 + tab_offset));
                RenderTab1();
            }
            else if (p_tabs->IsTabActive(1)) {
                ImGui::SetCursorPos(ImVec2(120.f, 20 + tab_offset));
                RenderTab2();
            }
            else if (p_tabs->IsTabActive(2)) {
                ImGui::SetCursorPos(ImVec2(120.f, 20 + tab_offset));
                RenderTab3();
            }
            ImGui::PopStyleVar();
        }
    }
    ImGui::End();
    ImGui::EndDisabled();
}

void gui::RenderTab1()
{
    ImGui::BeginGroup(); {

        custom::Child("Branding", ImVec2(ImGui::GetContentRegionAvail().x / 2 - c::bg::padding.x, 65), true, ImGuiWindowFlags_HorizontalScrollbar); {

            PushFont(font::s_inter_semibold);
            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(15, 15), c::anim::active, "LITHIUM.RIP");
            PopFont();

            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(15, 35), c::label::active, "Where advantage becomes inevitability.");
        }
        custom::EndChild();

        ImGui::SameLine();

        std::vector<std::string> subtabs_icon = { ICON_AI_FILL, ICON_TAG_2_FILL, ICON_TARGET_FILL, ICON_SETTINGS_5_FILL };
        std::vector<std::string> subtabs_name = { "General", "Legit", "Player", "Misc" };
        custom::Child("SubTabs", ImVec2(ImGui::GetContentRegionAvail().x - c::bg::padding.x, 65), true, ImGuiWindowFlags_HorizontalScrollbar); {
            for (size_t i = 0; i < subtabs_icon.size(); ++i) {
                if (custom::SubTab(subtabs_icon[i].c_str(), &tab_info::wanted_subid, i + 1, subtabs_name[i].c_str()) && iSubTabs != tab_info::wanted_subid)
                {
                    tab_info::is_subtab = true;
                    tab_info::tab_changing = true;
                } ImGui::SameLine();

            }
        }
        custom::EndChild();

        if (iSubTabs == 1) {

            ImVec2 child_size(ImGui::GetContentRegionAvail().x / 2 - c::bg::padding.x, (ImGui::GetContentRegionAvail().y) / 1.5f);

            custom::Child("MAIN", child_size, true); {

                custom::CategoryText("AIMBOT");

                custom::Checkbox("Enable AimBot", &aimbot.Active, "This is the description for your function\nand you can change it.", &aimbot.KeyBind);
				// custom::Checkbox("Enable Controller Support", &aimbot.ControllerSupport, "This is the description for your function\nand you can change it.");
				custom::Checkbox("Ignore Team", &aimbot.ignoreteam, "This is the description for your function\nand you can change it.");
               // custom::Checkbox("Enable TriggerBot", &aimbot.TriggerBot, "This is the description for your function\nand you can change it.", &keybind[1]);
                custom::Checkbox("Enable Prediction", &aimbot.Prediction, "This is the description for your function\nand you can change it.");
				//custom::Checkbox("Enable Magic Bullet", &exploits.MagicBullets, "This is the description for your function\nand you can change it.");
                custom::Checkbox("Enable Visible Check", &visuals.IsVisible, "This is the description for your function\nand you can change it.");
                //custom::Checkbox("Enable Humanize", &aimbot.Humanize, "This is the description for your function\nand you can change it.");
                custom::Checkbox("Enable FOV", &aimbot.FovCircle, "This is the description for your function\nand you can change it.");
                custom::Checkbox("Enable Crosshair", &aimbot.crosshair, "This is the description for your function\nand you can change it.");
                custom::Checkbox("Enable FOV Arrow", &visuals.FOVArrow, "This is the description for your function\nand you can change it.");
				custom::Checkbox("Enable Ignore Knocked", &visuals.Knocked, "This is the description for your function\nand you can change it.");
                custom::Checkbox("Enable Ignore Bots", &aimbot.ignoreBots, "This is the description for your function\nand you can change it.");

            }
            custom::EndChild();

            ImGui::SameLine();

            custom::Child("MAIN2", child_size, true); {

                custom::CategoryText("AIM SETTINGS");

                custom::SliderInt("Smoothness", &aimbot.Smooth, 1, 30);
                custom::SliderInt("FOV Circle", &aimbot.FovSize, 0, 350);
                custom::SliderInt("Max Target Range", &aimbot.AimDistance, 100, 500);
				// custom::SliderInt("Humanize", &aimbot.HumanizedDelay, 50, 250);
                // custom::SliderInt("TriggerBot Delay", &aimbot.TriggerDelay, 0, 15);

				custom::CategoryText("EXTRA SETTINGS");

                custom::Combo("HitBox", &settings::select, settings::items, IM_ARRAYSIZE(settings::items), 3);
                custom::Combo("Crosshair Type", &settings::crosshair_type, settings::crosshair_items, IM_ARRAYSIZE(settings::crosshair_items), 3);
            }
            custom::EndChild();

            custom::Child("NEW", ImVec2(ImGui::GetContentRegionAvail().x - c::bg::padding.x, ImGui::GetContentRegionAvail().y - 21), true); {

                custom::CategoryText("NEW");
                //custom::Checkbox("Silent Aim", &checkboxes[9], "This is the description for your function\nand you can change it.", &keybind[8]);
            }
            custom::EndChild();
        }

    }ImGui::EndGroup();
}

void gui::RenderTab2()
{
    ImGui::BeginGroup(); {

        custom::Child("Branding", ImVec2(ImGui::GetContentRegionAvail().x / 2 - c::bg::padding.x, 65), true, ImGuiWindowFlags_HorizontalScrollbar); {
            PushFont(font::s_inter_semibold);
            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(15, 15), c::anim::active, "LITHIUM.RIP");
            PopFont();
            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(15, 35), c::label::active, "Precision through clarity.");
        }
        custom::EndChild();

        ImGui::SameLine();

        std::vector<std::string> subtabs_icon = { ICON_EYE_FILL, ICON_EXPAND_PLAYER_FILL, ICON_COLOR_FILTER_FILL, ICON_MAGIC_1_FILL };
        std::vector<std::string> subtabs_name = { "ESP", "Glow", "Colors", "Extras" };
        custom::Child("SubTabs", ImVec2(ImGui::GetContentRegionAvail().x - c::bg::padding.x, 65), true, ImGuiWindowFlags_HorizontalScrollbar); {
            for (size_t i = 0; i < subtabs_icon.size(); ++i) {
                if (custom::SubTab(subtabs_icon[i].c_str(), &tab_info::wanted_subid, i, subtabs_name[i].c_str()) && iSubTabs != tab_info::wanted_subid) {
                    tab_info::is_subtab = true;
                    tab_info::tab_changing = true;
                } ImGui::SameLine();
            }
        }
        custom::EndChild();

        ImVec2 child_size(ImGui::GetContentRegionAvail().x / 2 - c::bg::padding.x, (ImGui::GetContentRegionAvail().y) / 1.5f);

        custom::Child("ESP_MAIN", child_size, true); {

            custom::CategoryText("VISUALS");


            custom::Checkbox("Enable Visuals", &visuals.active, "toggle for all ESP elements.");
            custom::Checkbox("Enable Box", &visuals.box, "Showing a Box for the player.");
            custom::Checkbox("Enable Skeleton", &visuals.skeleton, "Skeleton-rendering.");
            custom::Checkbox("Enable Platform", &visuals.platform, "Shows the Platform of the player.");
            custom::Checkbox("Enable Weapon", &visuals.Weapon, "Current weapon of enemy.");
            custom::Checkbox("Enable Username", &visuals.name, "Displays the Username of the player.");
			custom::Checkbox("Enable Distance", &visuals.distance, "Distance to the player.");
			custom::Checkbox("Enable Rank", &visuals.rank, "Player rank rendering.");
			custom::Checkbox("Enable Tracers", &visuals.tracers, "Draw lines to players.");
            custom::Checkbox("Enable Radar", &radar.enable, "Show the radar.");
           // custom::Checkbox("Enable Level", &visuals.level, "Display the level of the player.");
            custom::Checkbox("Enable Kill Score", &visuals.killscore, "Display the kills.");
			custom::Checkbox("Enable Rebootcount", &visuals.rebootcount, "Display FOV Circle.");
			custom::Checkbox("Enable Outline Text", &visuals.outlinedtext, "Display FOV Circle.");
            static const char* box_types[] = { "Normal", "Cornered", "Filled" };
            custom::Combo("Box Type", &current_item, box_types, 3);
            static const char* platform_types[] = { "Text", "Icons" };
            custom::Combo("Platform Display", &visuals.platform_display_type, platform_types, 2);
            static const char* rank_types[] = { "Text", "Icons" };
            custom::Combo("Rank Display", &visuals.rank_display_type, rank_types, 2);
			// custom::Checkbox("Enable Watermark", &visuals.Watermark, "Display watermark on screen.");
        }
        custom::EndChild();
        ImGui::SameLine();

        custom::Child("VISUAL COLORS", child_size, true); {
            custom::CategoryText("VISUAL OPTIONS");

            static bool enable_visible_box = true;
            custom::ColorEdit4("Box Color", (float*)&colorss.VisibleBoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

            static bool enable_invisible_box = true;
            custom::ColorEdit4("Invisible Box Color", (float*)&colorss.InvisibleBoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

            static bool enable_skeleton_color = true;
            custom::ColorEdit4("Skeleton Color", (float*)&colorss.VisibleSkeletonColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

            static bool enable_tracers_color = true;
            custom::ColorEdit4("Tracers Color", (float*)&colorss.TracersColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

            static bool enable_fov_circle_color = true;
            custom::ColorEdit4("FOV Circle Color", (float*)&colorss.FovCircleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
			custom::Separator_line();
            custom::SliderInt("Position X", &radar.pos_x, 0, 1920);
            custom::SliderInt("Position Y", &radar.pos_y, 0, 1080);
            custom::SliderInt("Size", &radar.size, 100, 500);
            custom::SliderInt("Range", &radar.range, 1, 25);
			custom::SliderInt("Render Distance ",&visuals.renderer, 50, 500);
            //ImGui::SliderInt("Dot Size", &radar.dot_size, 1.0f, 10.0f, "%.1f");
            ///ImGui::SliderInt("Arrow Size", &radar.arrow_size, 4.0f, 20.0f, "%.1f");
            //ImGui::SliderInt("Arrow Height", &radar.arrow_height, 16.0f, 64.0f, "%.1f");
            // ImGui::SliderInt("Arrow Length", &radar.arrow_long, 12.0f, 40.0f, "%.1f");
            //ImGui::SliderInt("Arrow Gap", &radar.arrow_gap, 10.0f, 50.0f, "%.1f");
        }
        custom::EndChild();

        custom::Child("VISUAL EXTRA", ImVec2(ImGui::GetContentRegionAvail().x - c::bg::padding.x, ImGui::GetContentRegionAvail().y - 21), true); {

            custom::CategoryText("SIZE OPTIONS");
            custom::SliderInt("Box Thickness", &visuals.box_tickness, 1.0f, 10.0f);
            custom::SliderInt("Skeleton Thickness", &visuals.skeleton_tickness, 1.0f, 5.0f);
			//custom::SliderInt("Head Box Thickness", &visuals.head_box_tickness, 1.0f, 10.0f);
			custom::SliderInt("Tracers Thickness", &visuals.tracers_tickness, 1.0f, 10.0f);

        }
        custom::EndChild();
    } ImGui::EndGroup();
}
void gui::RenderTab3()
{
    ImGui::BeginGroup(); {

        custom::Child("Branding", ImVec2(ImGui::GetContentRegionAvail().x / 2 - c::bg::padding.x, 65), true, ImGuiWindowFlags_HorizontalScrollbar); {
            PushFont(font::s_inter_semibold);
            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(15, 15), c::anim::active, "LITHIUM.RIP");
            PopFont();
            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(15, 35), c::label::active, "Power without limits.");
        }
        custom::EndChild();

        ImGui::SameLine();

        std::vector<std::string> subtabs_icon = { ICON_BOMB_FILL, ICON_SETTINGS_5_FILL, ICON_SWORD_FILL, ICON_MAGIC_1_FILL };
        std::vector<std::string> subtabs_name = { "Main", "Items", "Movement", "World" };
        custom::Child("SubTabs", ImVec2(ImGui::GetContentRegionAvail().x - c::bg::padding.x, 65), true, ImGuiWindowFlags_HorizontalScrollbar); {
            for (size_t i = 0; i < subtabs_icon.size(); ++i) {
                if (custom::SubTab(subtabs_icon[i].c_str(), &tab_info::wanted_subid, i + 1, subtabs_name[i].c_str()) && iSubTabs != tab_info::wanted_subid) {
                    tab_info::is_subtab = true;
                    tab_info::tab_changing = true;
                } ImGui::SameLine();
            }
        }
        custom::EndChild();

        if (iSubTabs == 1) {
            ImVec2 child_size(ImGui::GetContentRegionAvail().x / 2 - c::bg::padding.x, (ImGui::GetContentRegionAvail().y) / 1.5f);

            custom::Child("EXPLOITS_MAIN", child_size, true); {
                custom::CategoryText("EXPLOITS [USE AT OWN RISK]");

               custom::Checkbox("Enable FOV Changer", &exploits.fov_changer, "Change your field of view.", &keybind[20]); // need fix
               // custom::Checkbox("Enable Speed", &exploits.speedhack, "Run faster than normal.");
                custom::Checkbox("Enable Aim While Jumping", &exploits.aimwhilejumping, "Aim down sights in the air."); 
                custom::Checkbox("Enable Car Fly", &exploits.carfly, "Fly with vehicles (F4).", &keybind[22]);
                // custom::Checkbox("Enable Air Stuck", &exploits.airstuck, "Freeze in air (ALT).", &keybind[23]);
                custom::Checkbox("Enable No Recoil", &exploits.norecoil, "This is the description for your function\nand you can change it.");
                custom::Checkbox("Enable No Weapon Animation", &exploits.noweaponanim, "Remove weapon equip animations.");
                custom::Checkbox("Enable Rapid Fire", &exploits.rapidfire, "Reload weapons instantly.");
				custom::Checkbox("Enable Zipline TP", &exploits.ziplinetp, "Teleport to zipline start (F6).", &keybind[24]);
				// custom::Checkbox("Enable Instant Reload", &exploits.instant_reload, "instant reloading."); // Buggy
				custom::Checkbox("Enable SpinBot", &exploits.spinbot, "Never run out of stamina.");
            }
            custom::EndChild();

            ImGui::SameLine();

            custom::Child("EXPLOITS_SETTINGS", child_size, true); {
                custom::CategoryText("EXPLOIT SETTINGS");

                custom::SliderInt("FOV Changer", &exploits.fovchanger_value, 90, 180);
				custom::SliderInt("Rapid Fire Rate", &exploits.rapidfire_value, 50, 300);
                custom::SliderInt("Speed", &exploits.speedvalue, 50, 200);
                // custom::SliderInt("Teleport Distance", &slider_int[33], 10, 500);

                custom::CategoryText("ADDITIONAL OPTIONS");

            }
            custom::EndChild();

            custom::Child("EXPLOITS_EXTRA", ImVec2(ImGui::GetContentRegionAvail().x - c::bg::padding.x, ImGui::GetContentRegionAvail().y - 21), true); {
                custom::CategoryText("ADVANCED EXPLOITS");

            }
            custom::EndChild();
        }

    } ImGui::EndGroup();
}

void gui::PushFont(ImFont* font) { ImGui::PushFont(font); }
void gui::PopFont() { ImGui::PopFont(); }
ImDrawList* gui::GetForegroundDrawList() { return ImGui::GetForegroundDrawList(); }
ImDrawList* gui::GetWindowDrawList() { return ImGui::GetWindowDrawList(); }
bool gui::BeginWindow(const char* name, bool* p_open, ImGuiWindowFlags flags) { return ImGui::Begin(name, p_open, flags); }
void gui::EndWindow() { ImGui::End(); }
void gui::ToggleMenu() {
    if (menu_anim::active) {
        menu_anim::fixed_size = ImGui::GetWindowSize().y;
    }
    menu_anim::changing_size = true;
    menu_anim::active = !menu_anim::active;
}
bool gui::IsMenuAnimating() { return menu_anim::changing_size; }
int gui::GetCurrentSubTab() { return iSubTabs; }
void gui::SetCurrentSubTab(int tab) { iSubTabs = tab; }
bool gui::IsTabChanging() { return tab_info::tab_changing; }
void gui::SetTabChanging(bool changing) { tab_info::tab_changing = changing; }