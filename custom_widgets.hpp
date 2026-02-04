#define IMGUI_DEFINE_MATH_OPERATORS


#include <string>
#include <wtypes.h>

#include "font_defines.h"
#include <iostream>
#include <Windows.h>

#include <imgui_internal.h>

#include "notifications.h"	
#include "imstb_textedit.h"	

#include <cstdlib>
#include <imgui.h>

#include <map>

using namespace ImGui;



namespace custom
{
	static const char* selected_category = "Aimbot";

	void				LogotypeHeader(const char* text, const char* version);
	bool				CategoryText(const char* label);
	bool				CategoryTab(const char* label, const char* icon, std::vector<std::string>, int* v, int number);
	bool				SubTab(const char* label, int* v, int number, const char* hint);
	bool				Tab(const char* label, int* v, int number);


	bool				ThemeButton(const char* id_theme, bool dark, const ImVec2& size_arg);
	bool				Button(const char* label, const ImVec2& size_arg);

	bool				ChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool cap = false, ImGuiWindowFlags flags = 0, int* combo = 0, std::vector<std::string> combo_name = {});
	bool				Child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool cap = false, ImGuiWindowFlags flags = 0, int* combo = 0, std::vector<std::string> combo_name = {});
	bool				ChildID(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool cap = false, ImGuiWindowFlags flags = 0);
	void				EndChild();

	void				BeginGroup();
	void				EndGroup();

	bool				Checkbox(const char* label, bool* v, const char* hint = "", int* key = 0);
	bool				CheckboxClicked(const char* label, bool* v);

	bool			    Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0), const char* description = "");
	bool				Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

	bool				BeginCombo(const char* label, const char* preview_value, int val = 0, bool multi = false, ImGuiComboFlags flags = 0);
	void				EndCombo();
	void				MultiCombo(const char* label, bool variable[], const char* labels[], int count);
	bool				Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	bool				Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
	bool				Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);

	bool				ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool				ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
	bool			    ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);

	bool				KnobScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 0);
	bool				KnobFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags = 0);
	bool				KnobInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags = 0);

	bool				SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 0);
	bool				SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool				SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

	bool                ImConfig(const char* label, const char* date, const char* description, const char* creator);

	bool				Keybind(const char* label, int* key, int* mode);
	bool				MiniBind(const char* label, int* key, int* mode);

	void				Separator_line();

	void				SeparatorEx(ImGuiSeparatorFlags flags, float thickness);
	void				Separator();



}