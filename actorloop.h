#define IMGUI_DEFINE_MATH_OPERATORS
#define NOMINMAX

#include <D3DX11core.h>
#include <D3DX11tex.h>
#include <d3d9types.h>
#include <Uxtheme.h>
#include <algorithm>
#include <dwmapi.h>
#include <D3D11.h>
#include <stdio.h>
#include <thread>
#include <string>
#include <vector>
#include <cfloat>
#include <cmath>
#include <mutex>
#include <chrono>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "offsets.h"
#include "drawing.h"
#include "../Loading/driver.hpp"
#include "sdk.h"
#include "settings.h"
#include "gui.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

std::mutex base;
std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;
float     TargetDistance = FLT_MAX;
uintptr_t TargetEntity = NULL;
uintptr_t DesyncTargetEntity = NULL;
static uintptr_t lockedTarget = NULL;
static bool wasKeyPressed = false;
static bool aimbot_keybind_enabled = false;
static int aimbot_key = 0;
static bool aimbot_active = false;


struct AimLineAnimation {
    ImVec2 current_target{ 0, 0 };
    ImVec2 target_position{ 0, 0 };
    float fade_alpha = 0.0f;
    float line_thickness = 1.0f;
    bool is_active = false;
    bool was_shooting = false;
    float shooting_animation_time = 0.0f;

    void update(float delta_time, bool should_show, const ImVec2& new_target, bool is_shooting = false) {
        if (should_show) {
            target_position = new_target;
            is_active = true;
            fade_alpha = std::min(fade_alpha + delta_time * 5.0f, 1.0f);
            if (is_shooting && !was_shooting) {
                shooting_animation_time = 0.5f;
            }
            was_shooting = is_shooting;

            if (shooting_animation_time > 0.0f) {
                shooting_animation_time -= delta_time;
                float pulse = sin(shooting_animation_time * 15.0f) * 0.5f + 0.5f;
                line_thickness = 1.5f + pulse * 2.0f;
            }
            else {
                line_thickness = std::min(line_thickness + delta_time * 4.0f, 2.0f);
            }
        }
        else {
            fade_alpha = std::max(fade_alpha - delta_time * 8.0f, 0.0f);
            line_thickness = std::max(line_thickness - delta_time * 6.0f, 1.0f);
            shooting_animation_time = 0.0f;
            if (fade_alpha <= 0.0f) {
                is_active = false;
            }
        }
        const float lerp_speed = 15.0f;
        float lerp_factor = 1.0f - exp(-lerp_speed * delta_time);
        current_target.x += (target_position.x - current_target.x) * lerp_factor;
        current_target.y += (target_position.y - current_target.y) * lerp_factor;
    }
};
static AimLineAnimation aim_line_anim{};


void DrawOutlinedText(ImDrawList* draw_list, const char* text, ImVec2 pos, float font_size, ImU32 text_color, ImU32 outline_color = IM_COL32(0, 0, 0, 255), float outline_thickness = 1.0f) {
    if (!draw_list || !text) return;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            ImVec2 outline_pos = ImVec2(pos.x + x * outline_thickness, pos.y + y * outline_thickness);
            draw_list->AddText(ImGui::GetFont(), font_size, outline_pos, outline_color, text);
        }
    }

    draw_list->AddText(ImGui::GetFont(), font_size, pos, text_color, text);
}

void DrawThickOutlinedText(ImDrawList* draw_list, const char* text, ImVec2 pos, float font_size, ImU32 text_color, ImU32 outline_color = IM_COL32(0, 0, 0, 255)) {
    if (!draw_list || !text) return;
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            if (x == 0 && y == 0) continue;
            ImVec2 outline_pos = ImVec2(pos.x + x, pos.y + y);
            draw_list->AddText(ImGui::GetFont(), font_size, outline_pos, outline_color, text);
        }
    }
    draw_list->AddText(ImGui::GetFont(), font_size, pos, text_color, text);
}

std::string get_weapon_name(uintptr_t current_actor) {
    if (!current_actor) return "No Item";

    uintptr_t current_weapon = kernel->read_t<uintptr_t>(current_actor + CURRENT_WEAPON);
    if (!current_weapon) return "No Item";

    uintptr_t weapon_data = kernel->read_t<uintptr_t>(current_weapon + WEAPON_DATA);
    if (!weapon_data) return "No Item";

    uintptr_t item_name = kernel->read_t<uintptr_t>(weapon_data + ITEM_NAME);
    if (!item_name) return "No Item";

    uintptr_t fdata = kernel->read_t<uintptr_t>(item_name + 0x20);
    int flength = kernel->read_t<int>(item_name + 0x28);

    if (flength > 0 && flength < 50) {
        wchar_t* weapon_buffer = new wchar_t[flength];
        if (kernel->read(fdata, weapon_buffer, flength * sizeof(wchar_t))) {
            std::wstring wstr_buf(weapon_buffer, flength);
            std::string weapon_name(wstr_buf.begin(), wstr_buf.end());
            delete[] weapon_buffer;

            if (!weapon_name.empty()) {
                return weapon_name;
            }
        }

        delete[] weapon_buffer;
    }
    return "No Item";
}

std::string get_rank_name(int tier)
{
    switch (tier)
    {
    case 0: return "Bronze 1";
    case 1: return "Bronze 2";
    case 2: return "Bronze 3";
    case 3: return "Silver 1";
    case 4: return "Silver 2";
    case 5: return "Silver 3";
    case 6: return "Gold 1";
    case 7: return "Gold 2";
    case 8: return "Gold 3";
    case 9: return "Platinum 1";
    case 10: return "Platinum 2";
    case 11: return "Platinum 3";
    case 12: return "Diamond 1";
    case 13: return "Diamond 2";
    case 14: return "Diamond 3";
    case 15: return "Elite";
    case 16: return "Champion";
    case 17: return "Unreal";
    default: return "Unranked";
    }
}

ImVec4 get_rank_color(int tier)
{
    switch (tier)
    {
    case 0: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    case 1:
    case 2: return ImVec4(0.902f, 0.580f, 0.227f, 1.0f);
    case 3:
    case 4:
    case 5: return ImVec4(0.843f, 0.843f, 0.843f, 1.0f);
    case 6:
    case 7:
    case 8: return ImVec4(1.0f, 0.871f, 0.0f, 1.0f);
    case 9:
    case 10:
    case 11: return ImVec4(0.0f, 0.7f, 0.7f, 1.0f);
    case 12:
    case 13:
    case 14: return ImVec4(0.1686f, 0.3294f, 0.8235f, 1.0f);
    case 15: return ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    case 16: return ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
    case 17: return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);
    default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

bool is_dead(uintptr_t pawn_Slotted)
{
    return (kernel->read_t<char>(pawn_Slotted + B_IS_DYING) >> 5) & 1;
}

bool is_downed(uintptr_t player_state)
{
    if (!player_state) return false;
    return (kernel->read_t<char>(player_state + B_IS_DBNO) >> 0) & 1;
}

bool is_bot(uintptr_t player_state)
{
    if (!player_state) return false;
    return (kernel->read_t<char>(player_state + ITSBOT) >> 3) & 1;
}

std::string get_platform(uintptr_t player_state)
{
    if (is_bot(player_state)) {
        return "";
    }
    uintptr_t platform_ptr = kernel->read_t<uintptr_t>(player_state + PLATFORM);
    if (!platform_ptr) return "";

    wchar_t platform[64];
    if (kernel->read(platform_ptr, platform, sizeof(platform))) {
        std::wstring platform_wstr(platform);
        std::string platform_str(platform_wstr.begin(), platform_wstr.end());
        return platform_str; // "WIN", "PSN", "XBL", etc. 
    }
    return "";
}

static inline float clampf(float v, float lo, float hi) {
    return std::max(lo, std::min(v, hi));
}
static inline ImU32 AdjustAlpha(ImU32 col, float alpha01) {
    ImVec4 c = ImGui::ColorConvertU32ToFloat4(col);
    c.w = clampf(alpha01, 0.0f, 1.0f);
    return ImGui::ColorConvertFloat4ToU32(c);
}

static inline float AlphaFromThickness(float t) { return clampf(t / 10.0f, 0.0f, 1.0f); }
static inline float StrokeFromThickness(float t) { return clampf(t, 0.5f, 10.0f); }
static inline float TextScaleFromThickness(float t) { return clampf(t, 0.5f, 3.0f); }

bool memory_event(fvector newpos)
{
    kernel->write_t<fvector>(CachePointers.PlayerController + RotationInput, newpos);
    return true;
}

fvector GetLocation(fvector Location)
{
    fvector Loc = fvector(Location.x, Location.y, Location.z);
    return Loc;
}

fvector CalcRotation(fvector& zaz, fvector& daz) {
    fvector dalte = zaz - daz;
    fvector ongle;
    float hpm = sqrtf(dalte.x * dalte.x + dalte.y * dalte.y);
    ongle.y = atan(dalte.y / dalte.x) * 57.295779513082f;
    ongle.x = (atan(dalte.z / hpm) * 57.295779513082f) * -1.f;
    if (dalte.x >= 0.f) ongle.y += 180.f;
    return ongle;
}

bool IsVisible(uintptr_t mesh)
{
    auto Seconds = kernel->read_t<double>(CachePointers.UWorld + WORLD_TIME_SECONDS);
    auto LastRenderTime = kernel->read_t<float>(mesh + LAST_SUBMIT_TIME_ON_SCREEN);
    return Seconds - LastRenderTime <= 0.06f;
}

void memory_move(fvector head3d)
{
    fvector2d head2d = game_helper.ProjectWorldToScreen(head3d);

    if (aimbot.Mode == 0) {

        fvector2d target{};
        const float screencx = static_cast<float>(globals.ScreenWidth) * 0.5f;
        const float screency = static_cast<float>(globals.ScreenHeight) * 0.5f;

        float deltaX = 0.0f;
        float deltaY = 0.0f;

        if (head2d.x != 0) {
            if (head2d.x > screencx) {
                deltaX = -(screencx - head2d.x);
            }
            else if (head2d.x < screencx) {
                deltaX = head2d.x - screencx;
            }
        }

        if (head2d.y != 0) {
            if (head2d.y > screency) {
                deltaY = -(screency - head2d.y);
            }
            else if (head2d.y < screency) {
                deltaY = head2d.y - screency;
            }
        }

        float adjustedSmooth = aimbot.Smooth + 3.0f;
        if (adjustedSmooth < 4.0f) adjustedSmooth = 4.0f;
        if (adjustedSmooth > 23.0f) adjustedSmooth = 23.0f;

        float baseSmooth = adjustedSmooth;

        float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
        float distanceMultiplier = 1.0f;
        if (distance > 0.0f) {
            if (distance < 50.0f) {
                distanceMultiplier = 0.6f + (50.0f - distance) / 100.0f;
            }
            else if (distance > 100.0f) {
                distanceMultiplier = 1.3f + (distance - 100.0f) / 500.0f;
                if (distanceMultiplier > 2.0f) distanceMultiplier = 2.0f;
            }
        }
        float effectiveSmooth = baseSmooth * distanceMultiplier;
        target.x = deltaX / effectiveSmooth;
        target.y = deltaY / effectiveSmooth;
        static fvector2d lastTarget = { 0.0f, 0.0f };
        static fvector2d velocity = { 0.0f, 0.0f };
        static fvector2d acceleration = { 0.0f, 0.0f };

        float dynamicSmoothFactor = 0.4f / (adjustedSmooth * 0.1f + 0.5f);
        if (dynamicSmoothFactor > 0.4f) dynamicSmoothFactor = 0.4f;
        if (dynamicSmoothFactor < 0.05f) dynamicSmoothFactor = 0.05f;

        fvector2d targetVelocity = {
            (target.x - lastTarget.x) * dynamicSmoothFactor,
            (target.y - lastTarget.y) * dynamicSmoothFactor
        };

        acceleration.x = (targetVelocity.x - velocity.x) * 0.3f;
        acceleration.y = (targetVelocity.y - velocity.y) * 0.3f;

        velocity.x = velocity.x * 0.85f + acceleration.x;
        velocity.y = velocity.y * 0.85f + acceleration.y;

        target.x = lastTarget.x + (target.x - lastTarget.x) * dynamicSmoothFactor + velocity.x * 0.4f;
        target.y = lastTarget.y + (target.y - lastTarget.y) * dynamicSmoothFactor + velocity.y * 0.4f;

        lastTarget = target;
        float maxMovement = 30.0f + (distance / 20.0f);
        if (maxMovement > 60.0f) maxMovement = 60.0f;

        target.x = clampf(target.x, -maxMovement, maxMovement);
        target.y = clampf(target.y, -maxMovement, maxMovement);
        if (target.x + screencx > screencx * 2) target.x = 0;
        if (target.x + screencx < 0) target.x = 0;
        if (target.y + screency > screency * 2) target.y = 0;
        if (target.y + screency < 0) target.y = 0;

        float sensitivityScale = 2.0f + (adjustedSmooth * 0.8f);
        memory_event(fvector(-target.y / sensitivityScale, target.x / sensitivityScale, 0.0f));
    }
}


void cache_loop() {
    temporary_entity_list.clear();
    std::lock_guard<std::mutex> lock(base);

    uintptr_t encryptedUWorld = kernel->read_t<uintptr_t>(kernel->BaseAddress + UWORLD);
    uintptr_t decryptedUWorld = decryptWorld(encryptedUWorld);
    CachePointers.UWorld = decryptedUWorld;

    CachePointers.GameInstance = kernel->read_t<uintptr_t>(CachePointers.UWorld + GAME_INSTANCE);
    CachePointers.LocalPlayer = kernel->read_t<uintptr_t>(kernel->read_t<uintptr_t>(CachePointers.GameInstance + LOCAL_PLAYERS));
    CachePointers.PlayerController = kernel->read_t<uintptr_t>(CachePointers.LocalPlayer + PLAYER_CONTROLLER);
    CachePointers.AcknownledgedPawn = kernel->read_t<uintptr_t>(CachePointers.PlayerController + ACKNOWLEDGED_PAWN);
    CachePointers.Mesh = CachePointers.AcknownledgedPawn ? kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + MESH) : 0;
    CachePointers.RootComponent = CachePointers.AcknownledgedPawn ? kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + ROOT_COMPONENT) : 0;
    CachePointers.GameState = kernel->read_t<uintptr_t>(CachePointers.UWorld + GAME_STATE);
    CachePointers.PlayerArray = kernel->read_t<uintptr_t>(CachePointers.GameState + PLAYER_ARRAY);
    CachePointers.PlayerArraySize = kernel->read_t<int>(CachePointers.GameState + (PLAYER_ARRAY + sizeof(uintptr_t)));
    CachePointers.cameraIDK = kernel->read_t<uintptr_t>(CachePointers.PlayerController + PLAYERCAMERAMANAGER);
    CachePointers.TeamIndex = 0;

    for (int i = 0; i < CachePointers.PlayerArraySize; ++i) {
        uintptr_t ps = kernel->read_t<uintptr_t>(CachePointers.PlayerArray + i * sizeof(uintptr_t));
        if (!ps) continue;
        uintptr_t pawn_private = kernel->read_t<uintptr_t>(ps + PAWN_PRIVATE);
        if (pawn_private == CachePointers.AcknownledgedPawn) {
            CachePointers.TeamIndex = kernel->read_t<char>(ps + TEAM_INDEX);
            break;
        }
    }
    entity_list.clear();
    entity_list = temporary_entity_list;
}

void render_crosshair() {
    if (!aimbot.crosshair) return;

    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    ImVec2 screen_center(globals.ScreenWidth / 2.0f, globals.ScreenHeight / 2.0f);
    ImU32 crosshair_color = IM_COL32(255, 255, 255, 255);
    float size = 8.0f;
    float thickness = 1.5f;

    switch (settings::crosshair_type) {
    case 0:
    {
        draw_list->AddLine(ImVec2(screen_center.x - size, screen_center.y), ImVec2(screen_center.x + size, screen_center.y), crosshair_color, thickness);
        draw_list->AddLine(ImVec2(screen_center.x, screen_center.y - size), ImVec2(screen_center.x, screen_center.y + size), crosshair_color, thickness);
        break;
    }
    case 1:
    {
        draw_list->AddCircleFilled(screen_center, size * 0.3f, crosshair_color);
        draw_list->AddCircle(screen_center, size * 0.3f, IM_COL32(0, 0, 0, 150), 12, thickness * 0.5f);
        break;
    }
    case 2:
    {
        float gap = size * 0.5f;
        draw_list->AddLine(ImVec2(screen_center.x, screen_center.y - size), ImVec2(screen_center.x, screen_center.y - gap), crosshair_color, thickness);
        draw_list->AddLine(ImVec2(screen_center.x, screen_center.y + gap), ImVec2(screen_center.x, screen_center.y + size), crosshair_color, thickness);
        draw_list->AddLine(ImVec2(screen_center.x - size, screen_center.y), ImVec2(screen_center.x - gap, screen_center.y), crosshair_color, thickness);
        draw_list->AddLine(ImVec2(screen_center.x + gap, screen_center.y), ImVec2(screen_center.x + size, screen_center.y), crosshair_color, thickness);
       break;
    }
    }
}

void actor_loop() {
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    uintptr_t game_state = kernel->read_t<uintptr_t>(CachePointers.UWorld + GAME_STATE);
    uintptr_t player_array = kernel->read_t<uintptr_t>(game_state + PLAYER_ARRAY);
    CachePointers.player_count = kernel->read_t<int>(game_state + (PLAYER_ARRAY + sizeof(uintptr_t)));

    bool isAimbotKeyHeld = (GetAsyncKeyState(aimbot.KeyBind) & 0x8000) != 0;
    bool isRightMousePressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    float delta_time = std::chrono::duration<float>(current_time - last_time).count();
    last_time = current_time;

    ImVec2 screen_center(globals.ScreenWidth / 2.0f, globals.ScreenHeight / 2.0f);
    ImVec2 my_position(globals.ScreenWidth / 2.0f, globals.ScreenHeight - 0.0f);

    ImVec2 closest_target_pos{ 0, 0 };
    float closest_distance = FLT_MAX;
    bool target_found = false;
    bool is_shooting = false;

    if (!isAimbotKeyHeld && wasKeyPressed) {
        lockedTarget = NULL;
    }
    wasKeyPressed = isAimbotKeyHeld;
  
    /*if (exploits.fov_changer)
    {
        kernel->write_t<float>(CachePointers.PlayerController + 0x2B20, exploits.fovchanger_value); // FOVMinimum
        kernel->write_t<float>(CachePointers.PlayerController + 0x2B24, exploits.fovchanger_value); // FOVMaximum
    }
    if (exploits.rapidfire && CachePointers.AcknownledgedPawn) {
        uintptr_t current_weapon = kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + CURRENT_WEAPON);
        if (current_weapon) {
            float LastFireTime = kernel->read_t<float>(current_weapon + 0x1404); // LastFireTime
            float LastFireTimeVerified = kernel->read_t<float>(current_weapon + 0x1408); // LastFireTimeVerified

            if (LastFireTime > 0.0f && exploits.rapidfire_value > 0) {
                float reduction_factor = 0.02f + ((exploits.rapidfire_value - 50) / 250.0f) * 0.13f;

                if (reduction_factor < 0.02f) reduction_factor = 0.02f;
                if (reduction_factor > 0.15f) reduction_factor = 0.15f;

                kernel->write_t<float>(current_weapon + 0x1404, LastFireTime - reduction_factor);
                kernel->write_t<float>(current_weapon + 0x1408, LastFireTimeVerified - reduction_factor);
            }
        }
    }

    /*if (exploits.speedhack && CachePointers.AcknownledgedPawn) {
        kernel->write_t<uint8_t>(CachePointers.AcknownledgedPawn + CURRENT_MOVEMENT_STYLE, 5);
        float speed_multiplier = 0.8f + ((exploits.speedvalue - 50) / 100.0f) * 1.2f;
        if (speed_multiplier < 0.8f) speed_multiplier = 0.8f;
        if (speed_multiplier > 2.0f) speed_multiplier = 2.0f;
        kernel->write_t<float>(CachePointers.AcknownledgedPawn + 0x848, speed_multiplier);
    }*/
    /*else if (!exploits.speedhack && CachePointers.AcknownledgedPawn) {
        kernel->write_t<uint8_t>(CachePointers.AcknownledgedPawn + CURRENT_MOVEMENT_STYLE, 2);
    }

    if(exploits.norecoil && CachePointers.AcknownledgedPawn) {
        kernel->write_t<float>(CachePointers.AcknownledgedPawn + 0x68, 0.8f);
    }
    else if (CachePointers.AcknownledgedPawn) {
        kernel->write_t<float>(CachePointers.AcknownledgedPawn + 0x68, 1.0f);
    }
    if (exploits.aimwhilejumping && CachePointers.AcknownledgedPawn) {
        kernel->write_t<bool>(CachePointers.AcknownledgedPawn + 0x5851, true); // bADSWhileNotOnGround
    }
    else if (!exploits.aimwhilejumping && CachePointers.AcknownledgedPawn) {
        kernel->write_t<bool>(CachePointers.AcknownledgedPawn + 0x5851, false); // bADSWhileNotOnGround
    }
    if (exploits.instant_reload && CachePointers.AcknownledgedPawn) {
        uintptr_t current_weapon = kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + CURRENT_WEAPON);
        if (current_weapon) {
            bool is_reloading = kernel->read_t<bool>(current_weapon + bIsReloadingWeapon);

            if (is_reloading) {
                float current_time = kernel->read_t<float>(CachePointers.UWorld + WORLD_TIME_SECONDS);
                kernel->write_t<float>(current_weapon + 0x1498, current_time - 1.0f);
                kernel->write_t<bool>(current_weapon + bIsReloadingWeapon, false);
            }
        }
    }
    if (exploits.spinbot)
    {
        auto Mesh = kernel->read_t<uint64_t>(CachePointers.AcknownledgedPawn + MESH);
		static auto Cached = kernel->read_t<Vector3>(Mesh + 0x158); // RelativeRotation
        kernel->write_t<Vector3>(Mesh + 0x158, Vector3(1, rand() % 361, 1)); // RelativeRotation
    }
    if (exploits.Invisible)
    {
        auto Mesh = kernel->read_t<uint64_t>(CachePointers.AcknownledgedPawn + MESH);
		static auto Cached = kernel->read_t<Vector3>(Mesh + 0x139);
    }
    if (exploits.carfly && CachePointers.AcknownledgedPawn) {
        if (GetAsyncKeyState(VK_F4) & 0x8000) {
            uintptr_t CurrentVehicle = kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + CURRENT_VEHICLE);
            if (CurrentVehicle) {
                kernel->write_t<bool>(CurrentVehicle + 0x813, false); // Keep car active
                kernel->write_t<bool>(CurrentVehicle + 0x8A2, false); // Gravity OFF
            }
        }
    }
    if (exploits.airstuck && CachePointers.AcknownledgedPawn) {
        if (GetAsyncKeyState(exploits.airstuck_key) & 0x8000) { // ALT Key
            kernel->write_t<float>(CachePointers.AcknownledgedPawn + 0x68, 0.0f); // CustomTimeDilation
        }
        else {
            kernel->write_t<float>(CachePointers.AcknownledgedPawn + 0x68, 1.0f); // CustomTimeDilation
        }
    }
    if (exploits.noweaponanim && CachePointers.AcknownledgedPawn) {
        uintptr_t CurrentWeapon = kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + CURRENT_WEAPON);
        if (CurrentWeapon) {
        kernel->write_t<bool>(CurrentWeapon + 0x3C1, true); // bDisableEquipAnimation / 0x1
        }
    }
    else if (!exploits.noweaponanim && CachePointers.AcknownledgedPawn) {
        uintptr_t CurrentWeapon = kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + CURRENT_WEAPON);
        if (CurrentWeapon) {
        kernel->write_t<bool>(CurrentWeapon + 0x3C1, false); // bDisableEquipAnimation / 0x2
        }
    }*/ 

    for (int i = 0; i < CachePointers.player_count; i++) {
        uintptr_t player_state = kernel->read_t<uintptr_t>(player_array + i * sizeof(uintptr_t));
        if (!player_state) continue;
        uintptr_t current_actor = kernel->read_t<uintptr_t>(player_state + PAWN_PRIVATE);
        if (!current_actor) continue;
        uintptr_t skeletalmesh = kernel->read_t<uintptr_t>(current_actor + MESH);
        if (!skeletalmesh) continue;
        if (current_actor == CachePointers.AcknownledgedPawn) {
            continue;
        }
        char enemy_team_index = kernel->read_t<char>(player_state + TEAM_INDEX);
        if (aimbot.ignoreteam && enemy_team_index == CachePointers.TeamIndex) {
            continue;
        }

        fvector head3d = game_helper.getsocketlocation(skeletalmesh, aimbot.GetHitboxBone());
        fvector2d head2d = game_helper.ProjectWorldToScreen(fvector(head3d.x, head3d.y, head3d.z + 20));

        fvector bottom3d = game_helper.getsocketlocation(skeletalmesh, 0);
        fvector2d bottom2d = game_helper.ProjectWorldToScreen(bottom3d);
        fvector2d head_box = game_helper.ProjectWorldToScreen(fvector(head3d.x, head3d.y, head3d.z + 15));
        float  distance = camera_postion.location.distance(bottom3d) / 100.f;
        float  box_height = fabsf(head2d.y - bottom2d.y);
        float  box_width = box_height * 0.40f;
        fvector  root_bone = game_helper.getsocketlocation(skeletalmesh, 0);
        fvector2d root_box = game_helper.ProjectWorldToScreen(fvector(root_bone.x, root_bone.y, root_bone.z - 10));
        const float halfWidth = box_width * 0.5f;
        ImVec2 topLeft(head_box.x - halfWidth, head_box.y);
        ImVec2 bottomRight(root_box.x + halfWidth, root_box.y);
        bool withinAimDistance = (distance <= aimbot.AimDistance);

        if (aimbot.MagicBullet && aimbot.Active && isAimbotKeyHeld && withinAimDistance) {
            fvector targetPosition = game_helper.getsocketlocation(skeletalmesh, aimbot.GetHitboxBone());
            fvector cameraLocation = camera_postion.location;
            auto currentweapon = kernel->read_t<uintptr_t>(CachePointers.AcknownledgedPawn + CURRENT_WEAPON);
            fvector weaponLocation = camera_postion.location;
            fvector direction = {
            targetPosition.x - weaponLocation.x,
            targetPosition.y - weaponLocation.y,
            targetPosition.z - weaponLocation.z
            };
            kernel->write_t<fvector>(currentweapon + 0x1048, direction);
            kernel->write_t<fvector>(currentweapon + 0x1060, direction);
            kernel->write_t<fvector>(currentweapon + 0x1078, direction);
        }
        if (aimbot.Active && isAimbotKeyHeld && withinAimDistance) {
            if (visuals.Knocked) {
                bool is_knocked = (kernel->read_t<char>(current_actor + B_IS_DBNO) >> 7) & 1;
                if (is_knocked) {
                    continue;
                }
            }
            if (aimbot.ignoreBots && is_bot(player_state))
            continue;
            if (aimbot.ignoreDowned && is_downed(player_state))
            continue;
            fvector head3d_aim = game_helper.getsocketlocation(skeletalmesh, aimbot.GetHitboxBone());
            fvector2d head2d_aim = game_helper.ProjectWorldToScreen(head3d_aim);
            const float screencx = static_cast<float>(globals.ScreenWidth) * 0.5f;
            const float screency = static_cast<float>(globals.ScreenHeight) * 0.5f;
            float crossDist = static_cast<float>(get_cross_distance(head2d_aim.x, head2d_aim.y, screencx, screency));
            float fovRadius = aimbot.FovSize * 1.0f;
            bool canAim = true;

            if (visuals.IsVisible) {
                canAim = IsVisible(skeletalmesh);
            }

            if (lockedTarget != NULL) {
                if (current_actor == lockedTarget && canAim) {
                    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((float)head2d_aim.x, (float)head2d_aim.y), 4.0f, IM_COL32(255, 0, 0, 255));
                    ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)head2d_aim.x, (float)head2d_aim.y), 3.0f, IM_COL32(255, 255, 255, 255));
                    memory_move(head3d_aim);
                    if (isRightMousePressed) {
                        is_shooting = true;
                        target_found = true;
                        closest_target_pos = ImVec2(head2d_aim.x, head2d_aim.y);
                    }
                }
            }
            else {
                if (head2d_aim.x > 0 && head2d_aim.y > 0 && crossDist <= fovRadius && canAim) {
                    if (crossDist < closest_distance) {
                        closest_distance = crossDist;
                        if (isRightMousePressed) {
                            closest_target_pos = ImVec2(head2d_aim.x, head2d_aim.y);
                            target_found = true;
                        }
                    }

                    lockedTarget = current_actor;
                    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((float)head2d_aim.x, (float)head2d_aim.y), 4.0f, IM_COL32(0, 255, 0, 255));
                    ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)head2d_aim.x, (float)head2d_aim.y), 3.0f, IM_COL32(255, 255, 255, 255));
                    memory_move(head3d_aim);
                    if (isRightMousePressed) {
                        is_shooting = true;
                    }
                }
            }
        }
        static auto lastTriggerTime = std::chrono::high_resolution_clock::now();
        static bool canShoot = true;

        if (aimbot.TriggerBot) {
            auto targetedFortPawn = kernel->read_t<uintptr_t>(CachePointers.PlayerController + TARGETED_FORT_PAWN);
            if (targetedFortPawn != 0 && targetedFortPawn != CachePointers.AcknownledgedPawn) {

                static auto lastTriggerTime = std::chrono::high_resolution_clock::now();
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                    currentTime - lastTriggerTime).count();

                if (elapsedMs >= aimbot.TriggerDelay) {
                    INPUT inputs[2] = {};
                    inputs[0].type = INPUT_MOUSE;
                    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                    inputs[1].type = INPUT_MOUSE;
                    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                    SendInput(2, inputs, sizeof(INPUT));
                    lastTriggerTime = currentTime;
                }
            }
        }
        bool isPlayerVisible = IsVisible(skeletalmesh);
        
        if (visuals.active && visuals.box)
        {
            ImVec2 tl = topLeft;
            ImVec2 br = bottomRight;

            auto finite = [](const ImVec2& p) { return std::isfinite(p.x) && std::isfinite(p.y); };
            if (!finite(tl) || !finite(br))
                continue;

            if (tl.x > br.x) std::swap(tl.x, br.x);
            if (tl.y > br.y) std::swap(tl.y, br.y);

            const ImVec2 screen = ImGui::GetIO().DisplaySize;
            tl.x = std::clamp(tl.x, 0.0f, screen.x - 1.0f);
            tl.y = std::clamp(tl.y, 0.0f, screen.y - 1.0f);
            br.x = std::clamp(br.x, 0.0f, screen.x - 1.0f);
            br.y = std::clamp(br.y, 0.0f, screen.y - 1.0f);

            float bw = br.x - tl.x;
            float bh = br.y - tl.y;
            if (bw < 2.0f || bh < 2.0f || bw > screen.x * 1.5f || bh > screen.y * 1.5f)
                continue;

            tl.x = IM_FLOOR(tl.x) + 0.5f;
            tl.y = IM_FLOOR(tl.y) + 0.5f;
            br.x = IM_FLOOR(br.x) + 0.5f;
            br.y = IM_FLOOR(br.y) + 0.5f;

            float box_thickness = StrokeFromThickness(visuals.box_tickness);
            box_thickness = std::clamp(box_thickness, 1.0f, 6.0f);

            ImU32 box_color = isPlayerVisible ? colorss.VisibleBoxColor : colorss.InvisibleBoxColor;

            if (current_item == 0)
            {
                draw_list->AddRect(tl, br, box_color, 0.0f, 0, box_thickness);
            }
            else if (current_item == 1)
            {
                drawing::DrawCornerBox(draw_list, tl, br, box_color, box_thickness);
            }
            else if (current_item == 2)
            {
                drawing::DrawFilledBox(draw_list, tl, br, box_color, box_thickness);
            }
        }
        if (visuals.active && visuals.skeleton) {
            float sk_thickness = StrokeFromThickness(visuals.skeleton_tickness);

            ImU32 sk_color = isPlayerVisible ? colorss.VisibleSkeletonColor : colorss.InvisibleSkeletonColor;

            fvector head3d = game_helper.getsocketlocation(skeletalmesh, 110);
            fvector neck3d = game_helper.getsocketlocation(skeletalmesh, 67);
            fvector chest3d = game_helper.getsocketlocation(skeletalmesh, 66);
            fvector spine3d = game_helper.getsocketlocation(skeletalmesh, 3);
            fvector pelvis3d = game_helper.getsocketlocation(skeletalmesh, 2);

            fvector left_shoulder3d = game_helper.getsocketlocation(skeletalmesh, 9);
            fvector left_elbow3d = game_helper.getsocketlocation(skeletalmesh, 10);
            fvector left_hand3d = game_helper.getsocketlocation(skeletalmesh, 11);

            fvector right_shoulder3d = game_helper.getsocketlocation(skeletalmesh, 38);
            fvector right_elbow3d = game_helper.getsocketlocation(skeletalmesh, 39);
            fvector right_hand3d = game_helper.getsocketlocation(skeletalmesh, 40);

            fvector left_hip3d = game_helper.getsocketlocation(skeletalmesh, 71);
            fvector left_knee3d = game_helper.getsocketlocation(skeletalmesh, 72);
            fvector left_foot3d = game_helper.getsocketlocation(skeletalmesh, 75);

            fvector right_hip3d = game_helper.getsocketlocation(skeletalmesh, 78);
            fvector right_knee3d = game_helper.getsocketlocation(skeletalmesh, 79);
            fvector right_foot3d = game_helper.getsocketlocation(skeletalmesh, 82);

            fvector2d head2d = game_helper.ProjectWorldToScreen(head3d);
            fvector2d neck2d = game_helper.ProjectWorldToScreen(neck3d);
            fvector2d chest2d = game_helper.ProjectWorldToScreen(chest3d);
            fvector2d spine2d = game_helper.ProjectWorldToScreen(spine3d);
            fvector2d pelvis2d = game_helper.ProjectWorldToScreen(pelvis3d);

            fvector2d left_shoulder2d = game_helper.ProjectWorldToScreen(left_shoulder3d);
            fvector2d left_elbow2d = game_helper.ProjectWorldToScreen(left_elbow3d);
            fvector2d left_hand2d = game_helper.ProjectWorldToScreen(left_hand3d);

            fvector2d right_shoulder2d = game_helper.ProjectWorldToScreen(right_shoulder3d);
            fvector2d right_elbow2d = game_helper.ProjectWorldToScreen(right_elbow3d);
            fvector2d right_hand2d = game_helper.ProjectWorldToScreen(right_hand3d);

            fvector2d left_hip2d = game_helper.ProjectWorldToScreen(left_hip3d);
            fvector2d left_knee2d = game_helper.ProjectWorldToScreen(left_knee3d);
            fvector2d left_foot2d = game_helper.ProjectWorldToScreen(left_foot3d);

            fvector2d right_hip2d = game_helper.ProjectWorldToScreen(right_hip3d);
            fvector2d right_knee2d = game_helper.ProjectWorldToScreen(right_knee3d);
            fvector2d right_foot2d = game_helper.ProjectWorldToScreen(right_foot3d);

            auto isValidBone = [](const fvector2d& bone) -> bool {
                return bone.x != 0 && bone.y != 0 &&
                    bone.x > -300 && bone.x < globals.ScreenWidth + 300 &&
                    bone.y > -300 && bone.y < globals.ScreenHeight + 300;
                };

            auto drawBone = [&](const fvector2d& from, const fvector2d& to) {
                if (isValidBone(from) && isValidBone(to)) {
                    float lineLength = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));
                    if (lineLength > 1.0f && lineLength < 500.0f) {
                        draw_list->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), sk_color, sk_thickness);
                    }
                }
            };

            drawBone(head2d, neck2d);
            drawBone(neck2d, chest2d);
            drawBone(chest2d, spine2d);
            drawBone(spine2d, pelvis2d);
            drawBone(left_shoulder2d, neck2d);
            drawBone(right_shoulder2d, neck2d);
            drawBone(left_shoulder2d, left_elbow2d);  
            drawBone(left_elbow2d, left_hand2d);
            drawBone(right_shoulder2d, right_elbow2d);
            drawBone(right_elbow2d, right_hand2d);
            drawBone(pelvis2d, left_hip2d);
            drawBone(pelvis2d, right_hip2d);
            drawBone(left_hip2d, left_knee2d);
            drawBone(left_knee2d, left_foot2d);
            drawBone(right_hip2d, right_knee2d);
            drawBone(right_knee2d, right_foot2d);
        }

        if (visuals.active && visuals.tracers) {
            float tr_thickness = StrokeFromThickness(visuals.tracers_tickness);
            ImU32 tr_color = colorss.TracersColor;
            float boxCenterX = (bottomRight.x + topLeft.x) * 0.5f;
            ImVec2 p1 = ImVec2(globals.ScreenWidth / 2.0f, globals.ScreenHeight);
            ImVec2 p2 = ImVec2(boxCenterX, bottomRight.y);
            draw_list->AddLine(p1, p2, tr_color, tr_thickness);
        }

        /*
           if (exploits.ziplinetp && CachePointers.RootComponent && (GetAsyncKeyState(VK_F1) & 0x8000)) {
            static bool f1_was_pressed = false;
            if (!f1_was_pressed) {
                fvector target_pos = { 0.0f, 0.0f, 0.0f };
                kernel->write_t<fvector>(CachePointers.RootComponent + 0x1E0, target_pos);
                f1_was_pressed = true;
            }
        }
        else {
            static bool f1_was_pressed = false;
            f1_was_pressed = false;
        }*/ 

        if (visuals.active && visuals.head_box) {
            float head_box_thickness = StrokeFromThickness(visuals.head_box_tickness);
            ImU32 head_box_color = colorss.HeadBoxColor;

            fvector head3d_hb = game_helper.getsocketlocation(skeletalmesh, 110);
            fvector2d head2d_hb = game_helper.ProjectWorldToScreen(head3d_hb);

            if (head2d_hb.x == 0 || head2d_hb.y == 0)
                continue;

            float base_size = 15.0f;
            float distance_scale = std::clamp(50.0f / distance, 0.5f, 1.5f);
            float head_radius = base_size / distance_scale;

            draw_list->AddCircle(ImVec2(head2d_hb.x, head2d_hb.y), head_radius, head_box_color, 32, head_box_thickness);
        }


        
        float standard_font_size = ImGui::GetFontSize() * TextScaleFromThickness(visuals.TextSize * 0.67f);
        float boxCenterX = (bottomRight.x + topLeft.x) * 0.5f;
        ImU32 white_text_color = IM_COL32(255, 255, 255, 255);


        if (visuals.active && (visuals.name || visuals.team)) {
            std::string displayText;
            if (visuals.name) {
                displayText = decryption.GetPlayerName(player_state);
            }
            if (visuals.team) {
                if (!displayText.empty()) {
                    displayText += " ";
                }
                displayText += "TEAM ID :" + std::to_string(CachePointers.TeamIndex) + "";
            }
            ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, displayText.c_str(), nullptr);
            const float yOffsetTop = 10.0f;
            ImVec2 pos(head_box.x - text_size.x * 0.5f, head_box.y - yOffsetTop - text_size.y * 0.5f);

            if (visuals.outlinedtext) {
            DrawOutlinedText(draw_list, displayText.c_str(), pos, standard_font_size, white_text_color);
            }
            else {
                draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, white_text_color, displayText.c_str());
            }
        }
        if (visuals.active && visuals.platform) {
            std::string platform_str = get_platform(player_state);

            if (!platform_str.empty()) {
                const float yOffsetPlatform = -35.0f;
                float iconSize = 60.0f;
                float scale = std::clamp(distance / 1000.0f, 0.4f, 1.0f);
                ImVec2 imageSize(iconSize * scale, iconSize * scale);

                ImVec2 imagePos(
                    boxCenterX - imageSize.x * 0.5f,
                    head_box.y + yOffsetPlatform
                );

                if (visuals.platform_display_type == 1) { // Icons
                    std::string platform_for_icon;

                    if (platform_str == "WIN") platform_for_icon = "WINDOWS";
                    else if (platform_str == "XBL" || platform_str == "XSX" || platform_str == "XSS") platform_for_icon = "XBOX SERIES S/X";
                    else if (platform_str == "PSN" || platform_str == "PS5") platform_for_icon = "PLAYSTATION 5";
                    else if (platform_str == "SWT") platform_for_icon = "NINTENDO";
                    else if (platform_str == "AND") platform_for_icon = "ANROID";
                    else if (platform_str == "IOS") platform_for_icon = "IOS";
                    else platform_for_icon = platform_str;

                    if (platform_for_icon == "WINDOWS" && images::windows_logo) {
                        draw_list->AddImage((void*)images::windows_logo, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                    }
                    else if ((platform_for_icon == "XBOX SERIES S/X" || platform_for_icon == "XBOX ONE") && images::xbox_logo) {
                        draw_list->AddImage((void*)images::xbox_logo, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                    }
                    else if ((platform_for_icon == "PLAYSTATION 4" || platform_for_icon == "PLAYSTATION 5") && images::ps_logo) {
                        draw_list->AddImage((void*)images::ps_logo, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                    }
                    else if (platform_for_icon == "NINTENDO" && images::nintendo_logo) {
                        draw_list->AddImage((void*)images::nintendo_logo, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                    }
                    else if (platform_for_icon == "ANROID" && images::android_logo) {
                        draw_list->AddImage((void*)images::android_logo, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                    }
                    else if (platform_for_icon == "IOS" && images::ios_logo) {
                        draw_list->AddImage((void*)images::ios_logo, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                    }
                }
                else { // Text
                    std::string display_text;
                    if (platform_str == "WIN") display_text = "Windows";
                    else if (platform_str == "PSN" || platform_str == "PS5") display_text = "PlayStation";
                    else if (platform_str == "XBL" || platform_str == "XSX" || platform_str == "XSS") display_text = "Xbox";
                    else if (platform_str == "SWT") display_text = "Switch";
                    else if (platform_str == "AND") display_text = "Android";
                    else if (platform_str == "IOS") display_text = "iOS";
                    else display_text = platform_str;

                    ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, display_text.c_str());
                    ImVec2 text_pos(boxCenterX - text_size.x * 0.5f, head_box.y + yOffsetPlatform - text_size.y * 0.5f);

                    if (visuals.outlinedtext) {
                        DrawOutlinedText(draw_list, display_text.c_str(), text_pos, standard_font_size, white_text_color);
                    }
                    else {
                        draw_list->AddText(ImGui::GetFont(), standard_font_size, text_pos, white_text_color, display_text.c_str());
                    }
                }
            }
        }
        if (visuals.active && visuals.Weapon) {
            std::string weapon_str = get_weapon_name(current_actor);
            ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, weapon_str.c_str(), nullptr);
            const float yOffsetWeapon = 55.0f;
            ImVec2 pos(boxCenterX - text_size.x * 0.5f, bottomRight.y + yOffsetWeapon);

            if (visuals.outlinedtext) {
                DrawOutlinedText(draw_list, weapon_str.c_str(), pos, standard_font_size, white_text_color);
            }
            else {
                draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, white_text_color, weapon_str.c_str());
            }
        }
        if (visuals.active && visuals.killscore) {
            int32_t kill_count = kernel->read_t<int32_t>(player_state + KILL_SCORE);
            std::string kill_text = "Kills: " + std::to_string(kill_count);
            ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, kill_text.c_str(), nullptr);
            const float yOffsetKills = 70.0f;
            ImVec2 pos(boxCenterX - text_size.x * 0.5f, bottomRight.y + yOffsetKills);

            if (visuals.outlinedtext) {
                DrawOutlinedText(draw_list, kill_text.c_str(), pos, standard_font_size, white_text_color);
            }
            else {
                draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, white_text_color, kill_text.c_str());
            }
        }
        if (visuals.active && visuals.rebootcount) {
            int rebootCount = kernel->read_t<int32_t>(player_state + REBOOTS);
            char rebootBuffer[32];
            snprintf(rebootBuffer, sizeof(rebootBuffer), "Reboots: %d", rebootCount);
            ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, rebootBuffer);
            const float yOffsetReboots = 85.0f;
            ImVec2 pos(boxCenterX - text_size.x * 0.5f, bottomRight.y + yOffsetReboots);

            if (visuals.outlinedtext) {
                DrawOutlinedText(draw_list, rebootBuffer, pos, standard_font_size, white_text_color);
            }
            else {
                draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, white_text_color, rebootBuffer);
            }
        }
        /*if (visuals.active && visuals.level) {
            int level = kernel->read_t<int>(player_state + LEVELS);
            char levelBuffer[32];
            snprintf(levelBuffer, sizeof(levelBuffer), "Level: %d", level);
            ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, levelBuffer);
            const float yOffsetLevel = 100.0f;
            ImVec2 pos(boxCenterX - text_size.x * 0.5f, bottomRight.y + yOffsetLevel);
            if (visuals.outlinedtext) {
            DrawOutlinedText(draw_list, levelBuffer, pos, standard_font_size, white_text_color);
            }
            else {
                draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, white_text_color, levelBuffer);
            }
        }*/
        if (visuals.active && visuals.rank) {
            uintptr_t habenaro = kernel->read_t<uintptr_t>(player_state + HABANERO_COMPONENT);
            int32_t ranked_progress = kernel->read_t<int32_t>(habenaro + 0xD8 + 0x10);

            if (visuals.rank_display_type == 1) {
                const float yOffsetRank = -60.0f;
                float iconSize = 40.0f;
                float scale = std::clamp(distance / 1000.0f, 0.4f, 1.0f);
                ImVec2 imageSize(iconSize * scale, iconSize * scale);

                ImVec2 imagePos(
                    boxCenterX - imageSize.x * 0.5f,
                    head_box.y + yOffsetRank
                );

                ID3D11ShaderResourceView* rank_texture = nullptr;

                switch (ranked_progress) {
                case 0: rank_texture = images::bronze1; break;
                case 1: rank_texture = images::bronze2; break;
                case 2: rank_texture = images::bronze3; break;
                case 3: rank_texture = images::silver1; break;
                case 4: rank_texture = images::silver2; break;
                case 5: rank_texture = images::silver3; break;
                case 6: rank_texture = images::gold1; break;
                case 7: rank_texture = images::gold2; break;
                case 8: rank_texture = images::gold3; break;
                case 9: rank_texture = images::platinum1; break;
                case 10: rank_texture = images::platinum2; break;
                case 11: rank_texture = images::platinum3; break;
                case 12: rank_texture = images::diamond1; break;
                case 13: rank_texture = images::diamond2; break;
                case 14: rank_texture = images::diamond3; break;
                case 15: rank_texture = images::elite; break;
                case 16: rank_texture = images::champion; break;
                case 17: rank_texture = images::unreal; break;
                default: rank_texture = images::unranked; break;
                }

                if (rank_texture) {
                    draw_list->AddImage((void*)rank_texture, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
                }
            }
            else {
                std::string rank_name = get_rank_name(ranked_progress);
                ImVec4 rank_color = get_rank_color(ranked_progress);
                ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, rank_name.c_str(), nullptr);
                ImVec2 pos = ImVec2(boxCenterX - text_size.x / 2, bottomRight.y + 34.0f);
                ImU32 rank_color_u32 = ImGui::ColorConvertFloat4ToU32(rank_color);

                if (visuals.outlinedtext) {
                    DrawOutlinedText(draw_list, rank_name.c_str(), pos, standard_font_size, rank_color_u32);
                }
                else {
                    draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, rank_color_u32, rank_name.c_str());
                }
            }
        }
        if (visuals.active && visuals.distance) {
            std::string distanceText = "[ " + std::to_string(static_cast<int>(distance)) + "m" + " ]";
            ImVec2 text_size = ImGui::GetFont()->CalcTextSizeA(standard_font_size, FLT_MAX, 0.0f, distanceText.c_str(), nullptr);
            ImVec2 pos(boxCenterX - text_size.x * 0.5f, bottomRight.y + 10.0f);

            if (visuals.outlinedtext) {
                DrawOutlinedText(draw_list, distanceText.c_str(), pos, standard_font_size, white_text_color);
            }
            else {
                draw_list->AddText(ImGui::GetFont(), standard_font_size, pos, white_text_color, distanceText.c_str());
            }
        }
        if (visuals.active && visuals.FOVArrow) {
            fvector root_bone_arrow = game_helper.getsocketlocation(skeletalmesh, 0);
            bool isVisible = IsVisible(skeletalmesh);
            decryption.AddPlayerToFovCircle(root_bone_arrow, distance, isVisible, ImColor(255, 0, 0));
        }
    }
    aim_line_anim.update(delta_time, target_found&& isAimbotKeyHeld, closest_target_pos, is_shooting);
    if (aimbot.Active && aimbot.FovCircle) {
        ImVec2 screen_center(globals.ScreenWidth / 2.0f, globals.ScreenHeight / 2.0f);
        ImU32 color = colorss.FovCircleColor;
        float radius = aimbot.FovSize * 1.5;
        ImGui::GetBackgroundDrawList()->AddCircle(screen_center, radius, color, 64, 1.5f);
    }
}
