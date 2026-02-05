#include "imgui.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

static bool checkboxes[60];

inline namespace font
{
	inline ImFont* icomoon_logo = nullptr;
	inline ImFont* regular_m = nullptr;
	inline ImFont* regular_s = nullptr;
	inline ImFont* regular_l = nullptr;
	inline ImFont* icomoon_page_f = nullptr;
	inline ImFont* inter_semibold_f = nullptr;
	inline ImFont* s_inter_semibold = nullptr;
	inline ImFont* b_inter_semibold = nullptr;
	inline ImFont* inter_medium = nullptr;
}

inline namespace tab_info
{
	inline bool is_subtab;
	inline bool tab_changing;
	inline int wanted_id = 0;
	inline int wanted_subid = 0;

}

inline namespace utils
{
	inline ImColor GetColorWithAlpha(ImColor color, float alpha)
	{
		return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
	}

	inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
	{
		return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
	}

	inline ImColor GetDarkColor(const ImColor& color)
	{
		float r, g, b, a;
		r = color.Value.x;
		g = color.Value.y;
		b = color.Value.z;
		a = 255;

		float darkPercentage = 0.6f;
		float darkR = r * darkPercentage;
		float darkG = g * darkPercentage;
		float darkB = b * darkPercentage;

		return ImColor(darkR, darkG, darkB, a);
	}
	inline ImVec4 ImColorToImVec4(const ImColor& color)
	{
		return ImVec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
	}

}


inline namespace skeleton_col {
	inline float visible[4] = { 118 / 255.f, 187 / 255.f, 117 / 255.f, 0.5f };
	inline float invisible[4] = { 118 / 255.f, 187 / 255.f, 117 / 255.f, 0.5f };
	inline float visible_team[4] = { 118 / 255.f, 187 / 255.f, 117 / 255.f, 0.5f };
	inline float invisible_team[4] = { 118 / 255.f, 187 / 255.f, 117 / 255.f, 0.5f };
}


inline namespace c
{
    inline ImColor dark_color(8, 8, 12, 255);                  // Almost black with slight red undertone
    inline ImColor second_color(12, 10, 14, 255);
    inline ImColor background_color(18, 5, 5, 255);            // Deep bloody dark
    inline ImColor stroke_color(180, 20, 20, 80);              // Faint blood red stroke
    inline ImColor window_bg_color(5, 5, 8, 255);              // Pitch black-red

    inline ImVec4 accent = ImColor(200, 30, 30, 255);          // Bright blood red accent
    inline ImVec4 separator = ImColor(60, 10, 10, 255);        // Dark red separator

    inline namespace anim
    {
        inline float speed = 0.25f;                            // Slightly faster for aggression
        inline ImColor active = ImColor(220, 40, 40, 255);     // Hot blood red active
        inline ImColor default = ImColor(20, 15, 18, 255);
    }

    inline namespace bg
    {
        inline ImVec2 padding = ImVec2(12, 12);
        inline ImVec4 background = ImColor(25, 8, 8, 180);     // Semi-transparent bloody bg
        inline ImVec2 size = ImVec2(750, 700);
        inline float rounding = 8.f;                           // Slightly sharper corners
    }

    inline namespace child
    {
        inline ImVec4 background = ImColor(10, 8, 12, 255);    // Darker child windows
        inline ImVec4 cap = ImColor(8, 6, 10, 255);
        inline ImVec4 stroke = ImColor(120, 20, 20, 120);      // Visible blood red stroke
        inline float rounding = 6.f;
    }

    namespace page
    {
        inline ImVec4 background_active = ImColor(35, 10, 10, 255);  // Active tab/page red-tinted
        inline ImVec4 background = ImColor(18, 8, 8, 255);
        inline ImVec4 text_hov = ImColor(220, 40, 40, 255);         // Hover text blood red
        inline ImVec4 text = ImColor(235, 235, 240, 255);           // Clean white-ish
        inline float rounding = 5.f;
    }

    inline namespace elements
    {
        inline ImVec4 background_hovered = ImColor(80, 15, 15, 255);  // Hover = dark blood
        inline ImVec4 background = ImColor(25, 10, 12, 255);
        inline float rounding = 4.f;
    }

    inline namespace checkbox
    {
        inline ImVec4 mark = ImColor(220, 30, 30, 255);               // Red checkmark
    }

    inline namespace text
    {
        inline namespace label
        {
            inline ImColor active = ImColor(255, 240, 240, 255);      // Bright on active
            inline ImColor hovered = ImColor(255, 220, 220, 255);
            inline ImColor default = ImColor(200, 200, 205, 255);
        }

        inline namespace description
        {
            inline ImColor active = ImColor(220, 180, 180, 255);
            inline ImColor hovered = ImColor(240, 80, 80, 255);       // Red hover desc
            inline ImColor default = ImColor(160, 160, 165, 255);
        }
    }
}
