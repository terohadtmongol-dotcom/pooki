#pragma once
#include <string>
#include "imgui.h"
#include <windows.h>

inline ImVec2 radarSize = { 200, 200 };
inline ImVec2 radarPos = { 10, 35 };

namespace settings {
    inline int tab = 0;
    inline int sliderint = 150;
    inline int select = 0;
    inline const char* items[3] = { "Head", "Chest", "Pelvis" };

    inline int crosshair_type = 0;
    inline const char* crosshair_items[3] = { "Cross", "Dot", "Normal" };
}

namespace images {
	// Platforms
    inline ID3D11ShaderResourceView* windows_logo = nullptr;
    inline ID3D11ShaderResourceView* xbox_logo = nullptr;
    inline ID3D11ShaderResourceView* ps_logo = nullptr;
    inline ID3D11ShaderResourceView* nintendo_logo = nullptr;
    inline ID3D11ShaderResourceView* android_logo = nullptr;
    inline ID3D11ShaderResourceView* ios_logo = nullptr;

    // Ranks 
    inline ID3D11ShaderResourceView* unranked = nullptr;
    inline ID3D11ShaderResourceView* bronze1 = nullptr;
    inline ID3D11ShaderResourceView* bronze2 = nullptr;
    inline ID3D11ShaderResourceView* bronze3 = nullptr;
    inline ID3D11ShaderResourceView* silver1 = nullptr;
    inline ID3D11ShaderResourceView* silver2 = nullptr;
    inline ID3D11ShaderResourceView* silver3 = nullptr;
    inline ID3D11ShaderResourceView* gold1 = nullptr;
    inline ID3D11ShaderResourceView* gold2 = nullptr;
    inline ID3D11ShaderResourceView* gold3 = nullptr;
    inline ID3D11ShaderResourceView* platinum1 = nullptr;
    inline ID3D11ShaderResourceView* platinum2 = nullptr;
    inline ID3D11ShaderResourceView* platinum3 = nullptr;
    inline ID3D11ShaderResourceView* diamond1 = nullptr;
    inline ID3D11ShaderResourceView* diamond2 = nullptr;
    inline ID3D11ShaderResourceView* diamond3 = nullptr;
    inline ID3D11ShaderResourceView* elite = nullptr;
    inline ID3D11ShaderResourceView* champion = nullptr;
    inline ID3D11ShaderResourceView* unreal = nullptr;
}

class radar_t {
public:
    bool enable = true;
    bool rect = false;
    bool vischeck = false;
    bool distance = true;
    bool show_arrows = true;
    bool show_dots = true;
    bool show_cross = false;
    bool show_local_player = false;
    bool show_distance_circles = true;
    bool show_distance_labels = false;

    int pos_x = 20;      
    int pos_y = 50;      
    int size = 200;      
    int range = 5;       
    int scale = 1.0f;

    ImColor enemy_visible_color = ImColor(0, 255, 0, 255);
    ImColor enemy_invisible_color = ImColor(255, 0, 0, 255);
    ImColor teammate_visible_color = ImColor(0, 0, 255, 255);
    ImColor teammate_invisible_color = ImColor(100, 100, 255, 255);
    ImColor background_color = ImColor(0, 0, 0, 150);
    ImColor border_color = ImColor(255, 0, 0, 255);
    ImColor cross_color = ImColor(255, 255, 255, 255);
    ImColor local_player_color = ImColor(255, 255, 0, 255);
    ImColor distance_circle_color = ImColor(0, 150, 255, 200);

    float dot_size = 3.0f;
    float arrow_size = 8.0f;
    float arrow_height = 32.0f;
    float arrow_long = 26.0f;
    float arrow_gap = 25.0f;
    float arrow_rounding = 1.5f;
    float outline_thickness = 2.5f;
    float border_thickness = 2.0f;
    float cross_thickness = 1.0f;
    float distance_circle_thickness = 1.5f;

    float arrow_outline_opacity = 1.0f;
    float arrow_fill_opacity = 0.30f;
    float fov_arrow_outline_opacity = 1.0f;
    float fov_arrow_fill_opacity = 0.30f;
    float radar_arrow_outline_opacity = 1.0f;
    float radar_arrow_fill_opacity = 0.30f;
    float background_opacity = 0.8f;
    float dot_opacity = 1.0f;

    int radar_style = 0;
    const char* radar_style_items[2] = { "Dots", "Arrows" };
};
inline radar_t radar;

class colors_tt {
public:
    ImColor BoxColor = ImColor(211, 3, 252);
    ImColor VisibleBoxColor = ImColor(0, 255, 0);
    ImColor InvisibleBoxColor = ImColor(255, 0, 0);
    ImColor VisibleSkeletonColor = ImColor(0, 255, 0);
    ImColor InvisibleSkeletonColor = ImColor(255, 0, 0);
    ImColor TeammateVisibleColor = ImColor(0, 0, 255);
    ImColor TeammateInvisibleColor = ImColor(100, 100, 255);
    ImColor TracersColor = ImColor(211, 3, 252);
    ImColor HeadBoxColor = ImColor(211, 3, 252);
    ImColor TextColor = ImColor(211, 3, 252);
    ImColor FovCircleColor = ImColor(255, 255, 255);
    ImColor AimLineColor = ImColor(255, 0, 0, 200);
};
inline colors_tt colorss;

class expoits_t {
public:
    bool fov_changer = false;
    float fov_value = 150.0f;
    int fovchanger_value = 90.0f;

    bool ziplinetp = false;
    bool speedhack = false;
	bool MagicBullets = false;
    int speedvalue = 100;
	bool rapidfire = false;
	int rapidfire_value = 175;
    bool aimwhilejumping = false;
	bool instant_reload = false;
    bool carfly = false;
	bool spinbot = false;
	bool Invisible = false;
	bool norecoil = false;
    bool airstuck = false;
    int airstuck_key = VK_MENU; // ALT key
    bool noweaponanim = false;
};
inline expoits_t exploits;

class visuals_t {
public:
    int box_tickness = 3.0f;
    int skeleton_tickness = 3.0f;
    int tracers_tickness = 2.0f;
    int head_box_tickness = 3.0f;
    float TextSize = 1.4f;
    bool IsVisible = false;
    bool active = true;
    bool AimLines = false;
    bool box = true;
    bool Knocked = false;
    bool corner = false;
    bool skeleton_joints = false;
    bool skeleton = true;
    bool head_box = false;
    bool tracers = false;
    bool name = false;
	bool level = false;
	bool rebootcount = false;
	bool outlinedtext = false;
	bool kdr = false;
    bool Weapon = false;
    bool FOVArrow = false;
    bool distance = false;
    bool team = false;
    bool rank = false;
    bool Penis = false;
    bool platform = false;
    bool killscore = false;
    bool Watermark = false;
    int renderer = 500;
    int platform_display_type = 0; // 0 = Text, 1 = Icons
    int rank_display_type = 1; // 0 = Text, 1 = Icons

};
inline visuals_t visuals;

inline const char* items[3] = { "Normal", "Cornered", "Filled" };
inline int current_item = 0;

class globals_t {
public:
    HWND window_handle;
    bool show_menu = true;
    bool vsync = false;
    int ScreenWidth;
    int ScreenHeight;
};
inline globals_t globals;

class aimbot_t {
public:
    bool Active = true;
    bool TargetLine = false;
    int AimDistance = 100;
    bool TriggerBot = false;
    bool Prediction = false;
    bool SilentAim = false;
    bool FovCircle = false;
    bool ignoreteam = false;
    bool MagicBullet = false;
    bool crosshair = false;
    bool ControllerSupport = false;
    int TriggerDelay = 5;
    bool Humanize = false;
    int HumanizeSpeed = 100;
    int HumanizeAngle = 2;
    int HumanizeSmoothness = 50;
    int HumanizeReactionTime = 10;
    bool HumanizeJitter = false;
    int HumanizeJitterAmount = 10;
    bool HumanizeRandomization = true;
    int HumanizeRandomizationAmount = 30;
    bool ignoreBots = false;
    bool ignoreDowned = false;
    int FovSize = 75;
    int Smooth = 7;
    int Mode = 0;
    int KeyBind = 28;
    int KeyBindMode = 0;
    int GetHitboxBone() {
        switch (settings::select) {
        case 0: return 67;
        case 1: return 66;
        case 2: return 2;
        default: return 67;
        }
    }
};
inline aimbot_t aimbot;

class structs
{
public:
    uintptr_t
        WeaponData,
        UWorld,
        GameInstance,
        GameState,
        LocalPlayer,
        AcknownledgedPawn,
        PlayerState,
        PlayerController,
        RootComponent,
        Mesh,
        PlayerArray,
        LocalWeapon,
        cameraIDK,
        LocalVehicle;

    std::string Platform;
    int32_t AmmoCount;
    int TeamIndex,
        PlayerArraySize,
        player_count;
};
inline structs CachePointers;

class entity {
public:
    uintptr_t
        entity,
        skeletal_mesh,
        root_component,
        player_state;

    char IgnoreDeads;
    int team_index, kills;
    char team_number;
    float lastrendertime;
    bool is_visible;
    std::string platform;
};
