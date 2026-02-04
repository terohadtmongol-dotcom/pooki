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
	inline ImColor dark_color(15, 16, 20, 255);
	inline ImColor second_color(17, 18, 22, 255);
	inline ImColor background_color(22, 30, 25, 255);
	inline ImColor stroke_color(255, 255, 255, 0);
	inline ImColor window_bg_color(10, 10, 13, 255);

	inline ImVec4 accent = ImColor(0, 250, 180, 255);
	inline ImVec4 separator = ImColor(38, 42, 52, 255);

	inline namespace anim
	{
		inline float  speed = 0.3f;
		inline ImColor active = ImColor(50, 128, 230, 255);
		inline ImColor default = ImColor(18, 18, 23, 255);
	}

	inline namespace bg
	{
		inline ImVec2 padding = ImVec2(10, 10);
		inline ImVec4 background = ImColor(21, 37, 35, 155);
		inline ImVec2 size = ImVec2(750, 700);
		inline float   rounding = 9.f;
	}

	inline namespace child
	{
		inline ImVec4 background = ImColor(13, 13, 17, 255);
		inline ImVec4 cap = ImColor(12, 12, 15, 255);
		inline ImVec4 stroke = ImColor(38, 42, 52, 0);
		inline float   rounding = 7.f;
	}

	namespace page
	{
		inline ImVec4 background_active = ImColor(17, 18, 22, 255);
		inline ImVec4 background = ImColor(15, 16, 20, 255);
		inline ImVec4 text_hov = ImColor(21, 37, 35, 255);
		inline ImVec4 text = ImColor(235, 239, 241, 255);
		inline float   rounding = 5.f;
	}

	inline namespace elements
	{
		inline ImVec4 background_hovered = ImColor(38, 42, 52, 255);
		inline ImVec4 background = ImColor(17, 18, 22, 255);
		inline float   rounding = 4.f;
	}

	inline namespace checkbox
	{
		inline ImVec4 mark = ImColor(21, 37, 35, 255);
	}

	inline namespace text
	{
		inline namespace label
		{
			inline ImColor active = ImColor(240, 240, 255, 255);
			inline ImColor hovered = ImColor(240, 240, 240, 255);
			inline ImColor default = ImColor(151, 151, 151, 255);
		}

		inline namespace description
		{
			inline ImColor active = ImColor(180, 180, 180, 255);
			inline ImColor hovered = ImColor(21, 37, 35, 255);
			inline ImColor default = ImColor(151, 155, 151, 255);
		}
	}
}
