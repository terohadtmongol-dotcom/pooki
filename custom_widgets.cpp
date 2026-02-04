#include "custom_widgets.hpp"
#pragma comment(lib, "Winmm.lib")
#include <cmath>
#include "font_defines.h"

namespace particle {

	struct Particle {
		float posX, posY, velocityX, velocityY, alpha;
		int lifespan, seed, flag, delay;
		ImU32 color;  // Добавлен цвет частиц (ARGB)

		Particle() : posX(0), posY(0), velocityX(0), velocityY(0), alpha(1.0),
			lifespan(0), seed(0), flag(0), delay(0), color(IM_COL32(255, 255, 255, 255)) {}  // Конструктор по умолчанию с белым цветом
	};

	const int MAX_PARTICLES = 2048;
	Particle particles[MAX_PARTICLES];

	void AddParticle(ImVec2 origin, ImVec2 size, float flag, ImU32 color = c::anim::active) {  // Добавлен аргумент color
		Particle newParticle;
		newParticle.posX = origin.x + size.x / 2;
		newParticle.posY = origin.y;
		newParticle.velocityX = ((float)rand() / 32767) * 3 - 1.5;
		newParticle.velocityY = ((float)rand() / 32767) * 1.5 - (flag ? 4 : 1);
		newParticle.lifespan = rand() % (flag ? 250 : 299);
		newParticle.seed = rand();
		newParticle.flag = static_cast<int>(flag);
		newParticle.alpha = 1.0;  // Явное указание альфы
		newParticle.color = color;  // Присвоение цвета

		for (int i = 0; i < MAX_PARTICLES; i++) {
			if (particles[i].lifespan == 0) {
				particles[i] = newParticle;
				break;
			}
		}
	}

	void RenderEffects(ImDrawList* drawList, ImVec2 renderSize, float timeOffset) {
		int activeParticles = 0;
		for (int i = 0; i < MAX_PARTICLES; i++) {
			Particle& particle = particles[i];
			if (particle.lifespan) {
				if (particle.delay) {
					particle.delay--;
				}
				else {
					particle.posX += particle.velocityX;
					particle.posY += particle.velocityY;
					particle.velocityY += 0.015;
					particle.lifespan -= (particle.velocityY > 0) ? 1 : 0;

					ImVec2 points[4];  // Массив для точек частицы
					float scale = (static_cast<float>(rand()) / RAND_MAX * 3 + 0.1) * (particle.flag + 1);
					float noise = (timeOffset * (particle.lifespan < 0 ? 0 : 1)) + (i * static_cast<float>(rand()) / RAND_MAX * 2.5 - 1.5);
					float sinAngle = sin(noise) * scale;
					float cosAngle = cos(noise) * scale;

					// Создание "порванных" частиц
					for (int j = 0; j < 4; j++) {
						float angle = 2 * IM_PI * j / 4 + (static_cast<float>(rand()) / RAND_MAX - 0.5) * IM_PI / 8;
						points[j].x = particle.posX + cosAngle * cos(angle) - sinAngle * sin(angle);
						points[j].y = particle.posY + sinAngle * cos(angle) + cosAngle * sin(angle);
					}

					drawList->AddShadowConvexPoly(points, 4, particle.color, 25.f, ImVec2(0, 0));  // Использование цвета
					drawList->AddConvexPolyFilled(points, 4, particle.color);  // Использование цвета

					if (!particle.lifespan && particle.flag) {
						// Обработка лишних действий по окончанию жизни частицы, если требуется.
					}
				}
				if (particle.flag)
					activeParticles++;
			}
		}
	}
}


namespace custom
{

	const char* keys[] =
	{
		"-",
		"Mouse 1",
		"Mouse 2",
		"CN",
		"Mouse 3",
		"Mouse 4",
		"Mouse 5",
		"-",
		"Back",
		"Tab",
		"-",
		"-",
		"CLR",
		"Enter",
		"-",
		"-",
		"Shift",
		"CTL",
		"Menu",
		"Pause",
		"Caps Lock",
		"KAN",
		"-",
		"JUN",
		"FIN",
		"KAN",
		"-",
		"Escape",
		"CON",
		"NCO",
		"ACC",
		"MAD",
		"Space",
		"PGU",
		"PGD",
		"End",
		"Home",
		"Left",
		"Up",
		"Right",
		"Down",
		"SEL",
		"PRI",
		"EXE",
		"PRI",
		"INS",
		"Delete",
		"HEL",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"WIN",
		"WIN",
		"APP",
		"-",
		"SLE",
		"Numpad 0",
		"Numpad 1",
		"Numpad 2",
		"Numpad 3",
		"Numpad 4",
		"Numpad 5",
		"Numpad 6",
		"Numpad 7",
		"Numpad 8",
		"Numpad 9",
		"MUL",
		"ADD",
		"SEP",
		"MIN",
		"Delete",
		"DIV",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",
		"F15",
		"F16",
		"F17",
		"F18",
		"F19",
		"F20",
		"F21",
		"F22",
		"F23",
		"F24",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"NUM",
		"SCR",
		"EQU",
		"MAS",
		"TOY",
		"OYA",
		"OYA",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"Shift",
		"Shift",
		"Ctrl",
		"Ctrl",
		"Alt",
		"Alt"
	};

#include <string>

	struct key_state
	{
		ImVec4 background, text, icon;
		bool active = false;
		bool hovered = false;
		float alpha = 0.f;
		float size_x;
	};


	bool Keybind(const char* label, int* key, int* mode)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;

		const ImGuiID id = window->GetID(label);
		const float width = (GetContentRegionMax().x - style.WindowPadding.x);

		const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width, 19));

		ItemSize(ImRect(rect.Min, rect.Max));
		if (!ImGui::ItemAdd(rect, id)) return false;

		char buf_display[64] = "None";

		bool value_changed = false;
		int k = *key;

		std::string active_key = "";
		active_key += keys[*key];

		if (*key != 0 && g.ActiveId != id) {
			strcpy_s(buf_display, active_key.c_str());
		}
		else if (g.ActiveId == id) {
			strcpy_s(buf_display, "...");
		}

		const ImVec2 label_size = CalcTextSize(buf_display, NULL, true);

		ImRect clickable(ImVec2(rect.Max.x - 25 - label_size.x, rect.Min.y), rect.Max - ImVec2(5.f, 0));
		bool hovered = ItemHoverable(clickable, id, NULL);

		static std::map<ImGuiID, key_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, key_state() });
			it_anim = anim.find(id);
		}
		it_anim->second.text = ImLerp(it_anim->second.text, it_anim->second.active ? c::text::label::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());

		it_anim->second.background = ImLerp(it_anim->second.background, g.ActiveId == id ? c::anim::active : c::anim::default, GetAnimSpeed());
		it_anim->second.icon = ImLerp(it_anim->second.icon, g.ActiveId == id ? c::anim::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());

		window->DrawList->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(it_anim->second.background), c::elements::rounding);

		PushFont(font::icomoon_page_f);
		window->DrawList->AddText(ImVec2(rect.Min.x + 7.f, utils::center_text(rect.Min, rect.Max, "k").y), GetColorU32(it_anim->second.icon), "k");
		PopFont();

		window->DrawList->AddText(ImVec2(rect.Min.x, utils::center_text(rect.Min, rect.Max, label).y), GetColorU32(it_anim->second.text), label);

		PushStyleColor(ImGuiCol_Text, c::label::active.Value);
		RenderTextClipped(clickable.Min, clickable.Max, buf_display, NULL, &label_size, ImVec2(0.5f, 0.5f));
		PopStyleColor();

		if (hovered && io.MouseClicked[0])
		{
			if (g.ActiveId != id) {
				*key = 0;
			}
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		}
		else if (io.MouseClicked[0]) {
			if (g.ActiveId == id)
				ImGui::ClearActiveID();
		}

		if (g.ActiveId == id) {
			// VERWENDE GetAsyncKeyState DIREKT!
			bool key_detected = false;

			// ESC zum Abbrechen
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				*key = 0;
				ImGui::ClearActiveID();
				return false;
			}

			// Maus-Buttons (VK_LBUTTON bis VK_XBUTTON2)
			for (int i = VK_LBUTTON; i <= VK_XBUTTON2; i++) {
				if (GetAsyncKeyState(i) & 0x8000) {
					k = i;
					value_changed = true;
					key_detected = true;
					break;
				}
			}

			if (!key_detected) {
				for (int i = 0x08; i <= 0xA5; i++) {
					if (i >= VK_LBUTTON && i <= VK_XBUTTON2) continue; // Skip mouse buttons

					if (GetAsyncKeyState(i) & 0x8000) {
						k = i;
						value_changed = true;
						key_detected = true;
						break;
					}
				}
			}

			if (key_detected) {
				*key = k;
				ImGui::ClearActiveID();
			}
		}

		return value_changed;
	}


	bool MiniBind(const char* label, int* key, int* mode)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;

		const ImGuiID id = window->GetID(label);
		const float width = (GetContentRegionMax().x - style.WindowPadding.x) - 40;

		static std::map<ImGuiID, key_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, key_state() });
			it_anim = anim.find(id);
		}

		const ImRect rect(window->DC.CursorPos + ImVec2(width - 20 - it_anim->second.size_x, 0), window->DC.CursorPos + ImVec2(width, 19));

		ItemSize(ImRect(rect.Min, rect.Max));
		if (!ImGui::ItemAdd(rect, id)) return false;

		char buf_display[64] = "None";


		bool value_changed = false;
		int k = *key;

		std::string active_key = "";
		active_key += keys[*key];

		if (*key != 0 && g.ActiveId != id) {
			strcpy_s(buf_display, active_key.c_str());
		}
		else if (g.ActiveId == id) {
			strcpy_s(buf_display, "...");
		}

		const ImVec2 label_size = CalcTextSize(buf_display, NULL, true);

		ImRect clickable(rect.Min, rect.Max);
		bool hovered = ItemHoverable(clickable, id, NULL);


		it_anim->second.background = ImLerp(it_anim->second.background, g.ActiveId == id ? c::anim::active : c::anim::default, GetAnimSpeed());
		it_anim->second.icon = ImLerp(it_anim->second.icon, g.ActiveId == id ? c::anim::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());
		it_anim->second.size_x = ImLerp(it_anim->second.size_x, CalcTextSize(buf_display).x, GetAnimSpeed());

		window->DrawList->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(it_anim->second.background), c::elements::rounding);

		PushStyleColor(ImGuiCol_Text, utils::ImColorToImVec4(c::label::active));
		RenderTextClipped(clickable.Min, clickable.Max, buf_display, NULL, &label_size, ImVec2(0.5f, 0.5f));
		PopStyleColor();

		if (hovered && io.MouseClicked[0])
		{
			if (g.ActiveId != id) {

				memset(io.MouseDown, 0, sizeof(io.MouseDown));
				memset(io.KeysDown, 0, sizeof(io.KeysDown));
				*key = 0;
			}
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		}
		else if (io.MouseClicked[0]) {

			if (g.ActiveId == id)
				ImGui::ClearActiveID();
		}

		if (g.ActiveId == id) {
			for (auto i = 0; i < 5; i++) {
				if (io.MouseDown[i]) {
					switch (i) {
					case 0:
						k = 0x01;
						break;
					case 1:
						k = 0x02;
						break;
					case 2:
						k = 0x04;
						break;
					case 3:
						k = 0x05;
						break;
					case 4:
						k = 0x06;
						break;
					}
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
			if (!value_changed) {
				for (auto i = 0x08; i <= 0xA5; i++) {
					if (io.KeysDown[i]) {
						k = i;
						value_changed = true;
						ImGui::ClearActiveID();
					}
				}
			}

			if (IsKeyPressedMap(ImGuiKey_Escape)) {
				*key = 0;
				ImGui::ClearActiveID();
			}
			else {
				*key = k;
			}
		}

		return value_changed;
	}


	std::string split_string(const char* input, bool returnLeft) {
		std::string str = input;
		size_t dollarPos = str.find('$');

		if (dollarPos == std::string::npos) {
			// Если символ `$` не найден, возвращаем всю строку как есть
			return returnLeft ? str : "";
		}

		if (!returnLeft) {
			// Возвращаем часть строки слева от символа `$`
			return str.substr(0, dollarPos);
		}
		else {
			// Возвращаем часть строки справа от символа `$`
			return str.substr(dollarPos + 1);
		}
	}

	struct child_state
	{
		ImVec4 background, text, combo_text;
		float open, alpha, combo_size = 0.f, shadow_opticaly;
		bool opened_combo = false, hovered = false;
		float arrow_roll;
	};

	bool ChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool cap, ImGuiWindowFlags flags, int* combo, std::vector<std::string> combo_name)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* parent_window = g.CurrentWindow;

		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
		flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

		const ImVec2 content_avail = GetContentRegionAvail();
		ImVec2 size = ImFloor(size_arg) + ImVec2(0, 0);
		const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
		if (size.x <= 0.0f)
			size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too many issues)
		if (size.y <= 0.0f)
			size.y = ImMax(content_avail.y + size.y, 4.0f);



		SetNextWindowPos(ImVec2(parent_window->DC.CursorPos + ImVec2(0, 0)));
		SetNextWindowSize(size - ImVec2(0, 0));

		GetWindowDrawList()->AddRectFilled(parent_window->DC.CursorPos + ImVec2(1, 1), parent_window->DC.CursorPos + size - ImVec2(1, 1), GetColorU32(c::child::background), c::child::rounding, ImDrawFlags_RoundCornersAll);
		GetWindowDrawList()->AddRect(parent_window->DC.CursorPos + ImVec2(1, 1), parent_window->DC.CursorPos + size - ImVec2(1, 1), GetColorU32(c::child::stroke), c::child::rounding, ImDrawFlags_RoundCornersAll, 1.f);

		GetWindowDrawList()->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + size, GetColorU32(c::child::stroke), c::child::rounding, ImDrawFlags_RoundCornersAll, 1);

		const char* temp_window_name;

		if (name) ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);

		else ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

		const float backup_border_size = g.Style.ChildBorderSize;

		bool ret = Begin(temp_window_name, NULL, flags | ImGuiWindowFlags_NoBackground);

		ImGuiWindow* child_window = g.CurrentWindow;
		child_window->ChildId = id;
		child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

		if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

		const ImGuiID temp_id_for_activation = ImHashStr("##Child", 0, id);
		if (g.ActiveId == temp_id_for_activation) ClearActiveID();

		if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY))
		{
			FocusWindow(child_window);
			NavInitWindow(child_window, false);
			SetActiveID(temp_id_for_activation, child_window);
			g.ActiveIdSource = g.NavInputSource;
		}

		if (combo_name.size() > 0)
		{
			static std::map<ImGuiID, child_state> anim;
			auto it_anim = anim.find(id);

			if (it_anim == anim.end())
			{
				anim.insert({ id, child_state() });
				it_anim = anim.find(id);
			}


			it_anim->second.arrow_roll = ImLerp(it_anim->second.arrow_roll, it_anim->second.opened_combo ? -1.f : 1.f, g.IO.DeltaTime * 6.f);
			it_anim->second.background = ImLerp(it_anim->second.background, it_anim->second.opened_combo ? c::second_color : c::elements::background, g.IO.DeltaTime * 6.f);
			it_anim->second.combo_size = ImLerp(it_anim->second.combo_size, it_anim->second.opened_combo ? (combo_name.size() * 25) + 14 : 0.f, g.IO.DeltaTime * 12.f);


			ImRect total_bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + size);

			std::string combo_item_name = ICON_ALIGN_LEFT_LINE " " + combo_name[0];


			ImRect combo_bb(ImVec2(total_bb.Max.x, total_bb.Min.y) - (CalcTextSize(combo_item_name.c_str()) + ImVec2(25, -35)) - ImVec2(0.f, ImGui::GetScrollY()), ImVec2(total_bb.Max.x, total_bb.Min.y) - ImVec2(25, -35) - ImVec2(0.f, ImGui::GetScrollY()));

			it_anim->second.combo_text = ImLerp(it_anim->second.combo_text, it_anim->second.opened_combo ? c::label::active : combo_bb.Contains(GetMousePos()) ? c::label::hovered : c::label::default, g.IO.DeltaTime * 6.f);

			GetWindowDrawList()->AddText(utils::center_text(combo_bb.Min, combo_bb.Max, combo_item_name.c_str()), GetColorU32(it_anim->second.combo_text), combo_item_name.c_str());

			bool pressed_combo = IsMouseHoveringRect(combo_bb.Min, combo_bb.Max) && ImGui::IsMouseReleased(ImGuiMouseButton_Left);

			if (pressed_combo || (it_anim->second.opened_combo && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !it_anim->second.hovered)) it_anim->second.opened_combo = !it_anim->second.opened_combo;

			if (!it_anim->second.opened_combo && it_anim->second.combo_size < 2.f) return false;

			ImGui::SetNextWindowPos(ImVec2(combo_bb.Max.x - 205, combo_bb.Max.y + 5));
			ImGui::SetNextWindowSize(ImVec2(205, it_anim->second.combo_size));

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse;


			PushStyleColor(ImGuiCol_WindowBg, utils::ImColorToImVec4(c::anim::default));
			PushStyleVar(ImGuiStyleVar_WindowRounding, c::elements::rounding);
			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 15));
			PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

			bool ret = Begin(combo_item_name.c_str(), NULL, window_flags);
			{
				ImGui::SetWindowFocus();
				PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
				PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));

				ImGui::GetForegroundDrawList()->AddRect(ImGui::GetCurrentWindow()->Rect().Min, ImGui::GetCurrentWindow()->Rect().Max, GetColorU32(c::child::stroke), 1.f);

				for (auto i = 1; i < combo_name.size(); i++)
				{
					if (custom::Selectable(combo_name[i].c_str(), (*combo == i), ImGuiSelectableFlags_DontClosePopups, ImVec2(0, 20)))
					{
						*combo = i;
					}
				}
				PopStyleVar(2);
				it_anim->second.hovered = ImGui::IsWindowHovered();
			}
			End();

			PopStyleVar(3);
			PopStyleColor(1);

			ImGui::GetForegroundDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), GetColorU32(c::child::stroke), 1.f);


			if (it_anim->second.hovered && g.IO.MouseClicked[0]) it_anim->second.opened_combo = false;

		}


		return ret;
	}

	bool Child(const char* str_id, const ImVec2& size_arg, bool cap, ImGuiWindowFlags extra_flags, int* combo, std::vector<std::string> combo_name)
	{
		ImGuiWindow* window = GetCurrentWindow();

		if (cap) {
			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
			PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
		}
		return ChildEx(str_id, window->GetID(str_id), size_arg, cap, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding, combo, combo_name);
	}

	bool ChildID(ImGuiID id, const ImVec2& size_arg, bool cap, ImGuiWindowFlags extra_flags)
	{
		IM_ASSERT(id != 0);
		return ChildEx(NULL, id, size_arg, cap, extra_flags);
	}

	void EndChild()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		PopStyleVar(2);

		IM_ASSERT(g.WithinEndChild == false);
		IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);

		g.WithinEndChild = true;
		if (window->BeginCount > 1)
		{
			End();
		}
		else
		{
			ImVec2 sz = window->Size;

			if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) sz.x = ImMax(4.0f, sz.x);
			if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y)) sz.y = ImMax(4.0f, sz.y);

			End();

			ImGuiWindow* parent_window = g.CurrentWindow;
			ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
			ItemSize(sz);
			if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavWindowHasScrollY) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
			{
				ItemAdd(bb, window->ChildId);
			}
			else
			{
				ItemAdd(bb, 0);

				if (window->Flags & ImGuiWindowFlags_NavFlattened) parent_window->DC.NavLayersActiveMaskNext |= window->DC.NavLayersActiveMaskNext;
			}
			if (g.HoveredWindow == window) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
		}
		g.WithinEndChild = false;
		g.LogLinePosY = -FLT_MAX;
	}


	void BeginGroup()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		g.GroupStack.resize(g.GroupStack.Size + 1);
		ImGuiGroupData& group_data = g.GroupStack.back();
		group_data.WindowID = window->ID;
		group_data.BackupCursorPos = window->DC.CursorPos;
		group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
		group_data.BackupIndent = window->DC.Indent;
		group_data.BackupGroupOffset = window->DC.GroupOffset;
		group_data.BackupCurrLineSize = window->DC.CurrLineSize;
		group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
		group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
		group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
		group_data.BackupActiveIdPreviousFrameIsAlive = g.ActiveIdPreviousFrameIsAlive;
		group_data.EmitItem = true;

		window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
		window->DC.Indent = window->DC.GroupOffset;
		window->DC.CursorMaxPos = window->DC.CursorPos;
		window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
		if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;
	}

	void EndGroup()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		IM_ASSERT(g.GroupStack.Size > 0);

		ImGuiGroupData& group_data = g.GroupStack.back();
		IM_ASSERT(group_data.WindowID == window->ID);

		if (window->DC.IsSetPos) ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

		ImRect group_bb(group_data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, group_data.BackupCursorPos));

		window->DC.CursorPos = group_data.BackupCursorPos;
		window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
		window->DC.Indent = group_data.BackupIndent;
		window->DC.GroupOffset = group_data.BackupGroupOffset;
		window->DC.CurrLineSize = group_data.BackupCurrLineSize;
		window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
		if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;

		if (!group_data.EmitItem)
		{
			g.GroupStack.pop_back();
			return;
		}

		window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrLineTextBaseOffset);
		ItemSize(group_bb.GetSize());
		ItemAdd(group_bb, 0, NULL, ImGuiItemFlags_NoTabStop);

		const bool group_contains_curr_active_id = (group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
		const bool group_contains_prev_active_id = (group_data.BackupActiveIdPreviousFrameIsAlive == false) && (g.ActiveIdPreviousFrameIsAlive == true);
		if (group_contains_curr_active_id) g.LastItemData.ID = g.ActiveId;
		else if (group_contains_prev_active_id) g.LastItemData.ID = g.ActiveIdPreviousFrame;
		g.LastItemData.Rect = group_bb;

		const bool group_contains_curr_hovered_id = (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
		if (group_contains_curr_hovered_id) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;

		if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;

		g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;
		if (group_contains_prev_active_id && g.ActiveId != g.ActiveIdPreviousFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;

		g.GroupStack.pop_back();
	}




	void Separator_line()
	{
		GetWindowDrawList()->AddRectFilled(GetCursorScreenPos(), GetCursorScreenPos() + ImVec2(GetContentRegionMax().x - GetStyle().WindowPadding.x, 1), GetColorU32(c::separator));
		Spacing();
	}

	void SeparatorEx(ImGuiSeparatorFlags flags, float thickness)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return;

		ImGuiContext& g = *GImGui;
		IM_ASSERT(ImIsPowerOfTwo(flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical)));
		IM_ASSERT(thickness > 0.0f);

		if (flags & ImGuiSeparatorFlags_Vertical)
		{
			float y1 = window->DC.CursorPos.y;
			float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
			const ImRect bb(ImVec2(window->DC.CursorPos.x, y1 + (GetStyle().ItemSpacing.y / 2)), ImVec2(window->DC.CursorPos.x + thickness, y2 - (GetStyle().ItemSpacing.y / 2)));


			ItemSize(ImVec2(thickness, 0.0f));
			if (!ItemAdd(bb, 0)) return;

			window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::child::background));

			ImGui::SameLine();
		}
		else if (flags & ImGuiSeparatorFlags_Horizontal)
		{
			float x1 = window->Pos.x;
			float x2 = window->Pos.x + window->Size.x;

			if (g.GroupStack.Size > 0 && g.GroupStack.back().WindowID == window->ID) x1 += window->DC.Indent.x;

			if (ImGuiTable* table = g.CurrentTable)
			{
				x1 = table->Columns[table->CurrentColumn].MinX;
				x2 = table->Columns[table->CurrentColumn].MaxX;
			}

			ImGuiOldColumns* columns = (flags & ImGuiSeparatorFlags_SpanAllColumns) ? window->DC.CurrentColumns : NULL;
			if (columns) PushColumnsBackground();

			const float thickness_for_layout = (thickness == 1.0f) ? 0.0f : thickness;
			const ImRect bb(ImVec2(x1 + GetStyle().WindowPadding.x, window->DC.CursorPos.y), ImVec2(x2 - GetStyle().WindowPadding.x, window->DC.CursorPos.y + thickness));

			ItemSize(ImVec2(0.0f, thickness_for_layout));

			if (ItemAdd(bb, 0))
			{
				window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::separator));
			}
			if (columns)
			{
				PopColumnsBackground();
				columns->LineMinY = window->DC.CursorPos.y;
			}
		}
	}

	void Separator()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems) return;

		ImGuiSeparatorFlags flags = (window->DC.LayoutType == ImGuiLayoutType_Horizontal) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;
		flags |= ImGuiSeparatorFlags_SpanAllColumns;
		SeparatorEx(flags, 1.0f);
	}

	struct theme_state
	{
		ImVec4 background;
		float smooth_swap, alpha_line, line_size;
	};

	bool ThemeButton(const char* id_theme, bool dark, const ImVec2& size_arg)
	{
		ImGuiWindow* window = GetCurrentWindow();

		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(id_theme);
		const ImVec2 label_size = CalcTextSize(id_theme, NULL, true), pos = window->DC.CursorPos;

		ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

		const ImRect bb(pos, pos + size);

		ItemSize(size, 0.f);
		if (!ItemAdd(bb, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

		static std::map<ImGuiID, theme_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, theme_state() });
			it_anim = anim.find(id);
		}

		it_anim->second.background = ImLerp(it_anim->second.background, dark || hovered ? c::page::background_active : c::page::background, g.IO.DeltaTime * 6.f);

		it_anim->second.alpha_line = ImLerp(it_anim->second.alpha_line, dark ? 1.f : 0.f, g.IO.DeltaTime * 6.f);
		it_anim->second.line_size = ImLerp(it_anim->second.line_size, dark ? (size_arg.x / 4) : (size_arg.x / 2), g.IO.DeltaTime * 6.f);

		it_anim->second.smooth_swap = ImLerp(it_anim->second.smooth_swap, dark ? 26.f : 0, g.IO.DeltaTime * 12.f);

		GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(it_anim->second.background), c::page::rounding);

		PushClipRect(bb.Min, bb.Max, true);

		PushFont(font::icomoon_page_f);
		GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size_arg.x - CalcTextSize("k").x) / 2, bb.Max.y - CalcTextSize("k").y - (size.y - CalcTextSize("k").y) / 2 + it_anim->second.smooth_swap), GetColorU32(c::accent), "k");
		GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size_arg.x - CalcTextSize("a").x) / 2, bb.Max.y - CalcTextSize("a").y - (size.y - CalcTextSize("a").y) / 2 - 25 + it_anim->second.smooth_swap), GetColorU32(c::accent), "a");
		PopFont();

		PopClipRect();

		return pressed;
	}



	struct button_state
	{
		ImVec4 background, text;
	};

	bool Button(const char* label, const ImVec2& size_arg)
	{
		ImGuiWindow* window = GetCurrentWindow();

		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true), pos = window->DC.CursorPos;

		ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

		const ImRect bb(pos, pos + size);

		ItemSize(size, 0.f);
		if (!ItemAdd(bb, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

		const char* label_end = FindRenderedTextEnd(label);

		static std::map<ImGuiID, button_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, button_state() });
			it_anim = anim.find(id);
		}

		it_anim->second.background = ImLerp(it_anim->second.background, IsItemActive() || hovered ? ImColor(1.f, 1.f, 1.f, 0.05f) : ImColor(1.f, 1.f, 1.f, 0.0f), g.IO.DeltaTime * 6.f);
		it_anim->second.text = ImLerp(it_anim->second.text, IsItemActive() || hovered ? c::anim::active : c::label::default, g.IO.DeltaTime * 6.f);

		GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::child::background), c::elements::rounding);
		GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(it_anim->second.background), c::elements::rounding);
		GetWindowDrawList()->AddRect(bb.Min, bb.Max, GetColorU32(c::child::stroke), c::elements::rounding);

		PushClipRect(bb.Min, bb.Max, true);

		GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size_arg.x - CalcTextSize(label, label_end).x) / 2, bb.Max.y - CalcTextSize(label, label_end).y - (size.y - CalcTextSize(label, label_end).y) / 2), GetColorU32(it_anim->second.text), label, label_end);

		PopClipRect();

		return pressed;
	}

	struct tab_state
	{
		ImVec4 background, circle, text;
		float size_y;
		bool opened;
	};

	struct tabname_state
	{
		ImVec4 background, circle, text;
		float circle_radius;
	};

	bool CategoryTab(const char* label, const char* icon, std::vector<std::string> tab_names, int* v, int number)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const float square_sz = 15, y_size = 30;

		const ImVec2 pos = window->DC.CursorPos;

		const float w = GetContentRegionAvail().x - 18;

		const float total_width = w;

		static std::map<ImGuiID, tab_state> anim;
		tab_state& state = anim[id];

		const float tab_header_height = 35;  // Замена с 31 на 40
		const ImVec2 size = ImVec2(total_width, tab_header_height + state.size_y);
		const ImRect total_bb(pos, pos + size);
		ItemSize(total_bb, size.y);

		if (!ItemAdd(total_bb, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(ImRect(total_bb.Min, total_bb.Min + ImVec2(total_bb.GetSize().x, tab_header_height)), id, &hovered, &held);

		if (pressed)
			state.opened = !state.opened;

		state.size_y = ImLerp(state.size_y, state.opened ? 6 + (31 * tab_names.size()) : 0.f, g.IO.DeltaTime * 10.f);
		state.text = ImLerp(state.text, state.opened ? c::text::label::active : hovered ? c::text::label::hovered : c::text::label::default, g.IO.DeltaTime * 10.f);

		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorU32(c::child::background), c::elements::rounding);
		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Min + ImVec2(total_bb.GetSize().x, tab_header_height), utils::GetColorWithAlpha(c::anim::default, style.Alpha), c::elements::rounding);
		window->DrawList->AddRect(total_bb.Min, total_bb.Max, c::stroke_color, c::elements::rounding);

		if (state.opened == true)
			window->DrawList->AddRectFilled(total_bb.Min + ImVec2(0, tab_header_height), total_bb.Min + ImVec2(total_bb.GetSize().x, tab_header_height + 1), c::stroke_color, c::elements::rounding);


		PushFont(font::regular_m);
		window->DrawList->AddText(ImVec2(total_bb.Min.x + 42, total_bb.Min.y + (tab_header_height / 2) - CalcTextSize(label).y / 2), GetColorU32(state.text), label);
		ImRect value_bb(total_bb.Min + ImVec2(total_bb.GetSize().x - 25, 10), total_bb.Min + ImVec2(total_bb.GetSize().x - 15, tab_header_height - 10));
		PopFont();

		PushFont(font::regular_s);
		window->DrawList->AddText(value_bb.GetCenter() - CalcTextSize(ICON_DOWN_LINE) / 2, utils::GetColorWithAlpha(c::anim::active, style.Alpha), state.opened ? ICON_UP_LINE : ICON_DOWN_LINE);
		PopFont();

		PushFont(font::inter_semibold_f);
		window->DrawList->AddText(ImVec2(total_bb.Min.x + 12, total_bb.Min.y + (tab_header_height / 2) - CalcTextSize(icon).y / 2), utils::GetColorWithAlpha(c::anim::active, style.Alpha), icon);
		PopFont();

		window->DrawList->AddShadowCircle(ImVec2(total_bb.Min.x + 12, total_bb.Min.y + (tab_header_height / 2)), 4.f, utils::GetColorWithAlpha(c::anim::active, style.Alpha), 35.f, ImVec2(10, 0));

		PushClipRect(total_bb.Min, total_bb.Max, true);

		for (int i = 0; i < tab_names.size(); i++)
		{
			std::string name_for_id = tab_names[i] + "##" + std::to_string(i) + label;

			const ImGuiID name_id = window->GetID(name_for_id.c_str());

			static std::map<ImGuiID, tabname_state> anim;
			tabname_state& state = anim[name_id];

			ImRect text_block(total_bb.Min + ImVec2(6, tab_header_height + 7 + (31 * i)), total_bb.Min + ImVec2(total_width - 6, tab_header_height + 32 + (31 * i)));
			state.background = ImLerp(state.background, number + i == *v && selected_category == label ? c::anim::default: utils::GetColorWithAlpha(c::anim::default, 0.f), g.IO.DeltaTime * 8.f);


																														 state.text = ImLerp(state.text, number + i == *v && selected_category == label ? c::text::label::active : text_block.Contains(ImGui::GetMousePos()) ? c::text::label::hovered : c::text::label::default, g.IO.DeltaTime * 8.f);
																														 state.circle = ImLerp(state.circle, number + i == *v && selected_category == label ? c::anim::active : utils::GetColorWithAlpha(c::anim::active, 0.f), g.IO.DeltaTime * 8.f);
																														 state.circle_radius = ImLerp(state.circle_radius, number + i == *v && selected_category == label ? 2.f : 0.f, g.IO.DeltaTime * 8.f);

																														 window->DrawList->AddRectFilled(text_block.Min, text_block.Max, GetColorU32(state.background), c::elements::rounding);

																														 PushFont(font::regular_s);
																														 window->DrawList->AddText(ImVec2(text_block.Min.x + 25.f, text_block.GetCenter().y - CalcTextSize(tab_names[i].c_str()).y / 2), GetColorU32(state.text), tab_names[i].c_str());
																														 PopFont();

																														 window->DrawList->AddCircleFilled(ImVec2(text_block.Min.x + 12.f, text_block.GetCenter().y), state.circle_radius * 2, utils::GetColorWithAlpha(state.circle, state.circle.w / 2));
																														 window->DrawList->AddCircleFilled(ImVec2(text_block.Min.x + 12.f, text_block.GetCenter().y), state.circle_radius, GetColorU32(state.circle));

																														 ImGui::SetCursorScreenPos(text_block.Min);
																														 ImGui::InvisibleButton(tab_names[i].c_str(), text_block.GetSize());

																														 if (text_block.Contains(ImGui::GetMousePos()) && ImGui::IsItemClicked() && tab_info::wanted_id != number + i) {


																															 *v = number + i;
																															 selected_category = label;
																														 }
		}

		PopClipRect();

		ImGui::SetCursorScreenPos(ImVec2(total_bb.Min.x, total_bb.Max.y + style.ItemSpacing.y));

		return pressed;
	}


	bool CategoryText(const char* label)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);


		const ImVec2 pos = window->DC.CursorPos;
		PushFont(font::inter_semibold_f);
		const ImVec2 size = ImVec2(15, 10) + CalcTextSize(label);
		const ImRect total_bb(pos, pos + size);
		ItemSize(total_bb, size.y);

		ItemAdd(total_bb, id);

		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorWithAlpha(c::anim::active, c::anim::active.Value.w * GetStyle().Alpha / 3.5), c::elements::rounding / 2);
		window->DrawList->AddText(total_bb.GetCenter() - CalcTextSize(label) / 2, GetColorWithAlpha(c::anim::active, c::anim::active.Value.w * GetStyle().Alpha), label);
		PopFont();

		return ImGui::IsItemClicked();
	}


	struct subtab_state
	{
		ImVec4 background, circle, text;
		float alpha;
		bool opened;

		float size_offset, keyname_size, tooltip_alpha;
	};


	bool SubTab(const char* label, int* v, int number, const char* hint)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const float square_sz = 20;

		const ImVec2 pos = window->DC.CursorPos;

		const float total_width = 240;

		static std::map<ImGuiID, subtab_state> anim;
		subtab_state& state = anim[id];

		const float tab_header_height = 45;  // Замена с 31 на 40
		const ImVec2 size = ImVec2(45, 45);
		const ImRect total_bb(pos, pos + size);
		ItemSize(total_bb, size.y);

		if (!ItemAdd(total_bb, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(ImRect(total_bb.Min, total_bb.Min + ImVec2(total_bb.GetSize().x, tab_header_height)), id, &hovered, &held);

		if (pressed)
			*v = number;

		state.alpha = ImLerp(state.alpha, *v == number ? style.Alpha : 0.f, c::anim::speed);
		state.text = ImLerp(state.text, *v == number ? c::anim::active : hovered ? c::label::hovered : c::label::default, c::anim::speed);

		state.background = ImLerp(state.background, *v == number || hovered ? utils::GetColorWithAlpha(c::anim::active, 0.25f) : utils::GetColorWithAlpha(c::anim::active, 0.f), g.IO.DeltaTime * 6.f);
		state.size_offset = ImLerp(state.size_offset, *v == number ? 8.f : 0.f, g.IO.DeltaTime * 6.f);

		state.tooltip_alpha = ImLerp(state.tooltip_alpha, total_bb.Contains(ImGui::GetMousePos()) ? 1.f : 0.f, GetAnimSpeed() * 1.5);


		/*window->DrawList->AddRect(total_bb.Min, total_bb.Max, c::stroke_color, c::elements::rounding);*/

		
		const int vtx_idx_0 = GetWindowDrawList()->VtxBuffer.Size;
		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorU32(state.background), c::elements::rounding);

		const int vtx_idx_1 = GetWindowDrawList()->VtxBuffer.Size;
		ShadeVertsLinearColorGradientKeepAlpha(GetWindowDrawList(), vtx_idx_0, vtx_idx_1, total_bb.Min, total_bb.Max, utils::GetColorWithAlpha(GetDarkColor(GetColorU32(state.background)), state.background.w), utils::GetColorWithAlpha(GetColorU32(state.background), state.background.w));

		/*window->DrawList->AddShadowCircle(ImVec2(total_bb.Max.x - 18.f, total_bb.GetCenter().y), 3.f, utils::GetColorWithAlpha(c::anim::active, state.alpha), 25.f, ImVec2(0, 0), 0, 36);
		window->DrawList->AddCircleFilled(ImVec2(total_bb.Max.x - 18.f, total_bb.GetCenter().y), 3.f, utils::GetColorWithAlpha(c::anim::active, state.alpha), 36);*/

		window->DrawList->AddShadowCircle(total_bb.GetCenter(), 6.f, GetColorU32(state.text), 45.f, ImVec2(0, 0), 0, 36);

		window->DrawList->AddText(total_bb.GetCenter() - CalcTextSize(label) / 2 + ImVec2(0, 1), GetColorU32(state.text), label);

		ImRect value_bb(total_bb.Min + ImVec2(total_bb.GetSize().x - 25, 10), total_bb.Min + ImVec2(total_bb.GetSize().x - 15, tab_header_height - 10));


		if (state.tooltip_alpha > 0.1f)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, state.tooltip_alpha);

			ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
			ImGui::PushStyleColor(ImGuiCol_PopupBg, utils::ImColorToImVec4(c::anim::default));
			ImGui::PushStyleColor(ImGuiCol_Border, c::stroke);
			ImGui::BeginTooltip();

			ImGui::Text(hint);

			ImGui::EndTooltip();
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar(4);
		}

		return pressed;
	}

	struct tabstate_status
	{
		ImVec4 background, line, text_color;
		float alpha;
	};


	bool Tab(const char* label, int* v, int number)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const float square_sz = 15;

		const ImVec2 pos = window->DC.CursorPos;

		const float total_width = 60;

		static std::map<ImGuiID, tabstate_status> anim;
		tabstate_status& state = anim[id];

		const ImVec2 size = ImVec2(total_width, total_width);
		const ImRect total_bb(pos, pos + size);
		ItemSize(total_bb, size.y);

		if (!ItemAdd(total_bb, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(ImRect(total_bb.Min, total_bb.Min + ImVec2(total_bb.GetSize())), id, &hovered, &held);

		if (pressed)
			*v = number;

		state.alpha = ImLerp(state.alpha, *v == number ? style.Alpha : 0.f, c::anim::speed);
		state.text_color = ImLerp(state.text_color, *v == number ? c::anim::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());

		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorWithAlpha(c::anim::active, c::anim::active.Value.w * state.alpha / 4), c::elements::rounding);
		window->DrawList->AddRect(total_bb.Min, total_bb.Max, utils::GetColorWithAlpha(c::stroke_color, c::stroke_color.Value.w * state.alpha), c::elements::rounding);

		window->DrawList->AddShadowCircle(total_bb.GetCenter(), 10.f, GetColorWithAlpha(c::anim::active, anim::active.Value.w * state.alpha), 125.f, ImVec2(0,0));
		
		window->DrawList->AddText(total_bb.GetCenter() - CalcTextSize(label) / 2, GetColorU32(state.text_color), label);
		

		return pressed;
	}

	struct arrow_state {
		ImVec4 arrow_col;
		ImVec4 frame_col;
	};

	bool ArrowButton(const char* label, ImGuiDir dir)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const float square_sz = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect total_bb(pos, pos + ImVec2(20, 20));

		ItemSize(total_bb, style.FramePadding.y);
		ItemAdd(total_bb, id);

		static std::map<ImGuiID, arrow_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, arrow_state() });
			it_anim = anim.find(id);
		}

		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

		RenderNavHighlight(total_bb, id);

		it_anim->second.frame_col = ImLerp(it_anim->second.frame_col, hovered ? c::anim::active : utils::GetColorWithAlpha(c::anim::active, 0.f), GetAnimSpeed());
		it_anim->second.arrow_col = ImLerp(it_anim->second.arrow_col, hovered ? utils::GetDarkColor(c::anim::active) : c::anim::active, GetAnimSpeed());

		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorU32(it_anim->second.frame_col), c::elements::rounding);
		window->DrawList->AddRect(total_bb.Min, total_bb.Max, stroke_color, c::elements::rounding);

		PushFont(font::icomoon_page_f);
		window->DrawList->AddText(utils::center_text(total_bb.Min, total_bb.Max, dir == ImGuiDir_Right ? "r" : dir == ImGuiDir_Left ? "l" : dir == ImGuiDir_Up ? "u" : "d"), GetColorU32(it_anim->second.arrow_col), dir == ImGuiDir_Right ? "r" : dir == ImGuiDir_Left ? "l" : dir == ImGuiDir_Up ? "u" : "d");
		PopFont();

		return IsItemClicked();
	}

	struct check_state
	{
		ImVec4 text_color, checkbox_col, rect_color, keybind_color, settings_col, settings_text_col;
		float size_offset, keyname_size, tooltip_alpha;
		const char* keybind_name;
		int keybind_mode;
		bool settings_enabled;
	};

	enum KeybindModes
	{
		None = 0,
		Waiting,
		Used
	}key_modes;

	bool Checkbox(const char* label, bool* v, const char* hint, int* key)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		std::string label_str = label;
		std::string arrows_str[2] = { label_str + "left", label_str + "right" };

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const float square_sz = 10;
		bool keybind_used = (key != nullptr);

		const ImVec2 pos = window->DC.CursorPos;

		const float w = GetContentRegionAvail().x;

		const ImRect total_bb(pos, pos + ImVec2(w, 19));
		const ImRect item_bb(pos, pos + ImVec2(w, 19));

		ItemSize(item_bb, 0.f);

		if (!ItemAdd(item_bb, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(item_bb, id, &hovered, &held);

		static std::map<ImGuiID, check_state> anim;
		auto it_anim = anim.emplace(id, check_state()).first;

		ImRect check_bb(total_bb.Min, total_bb.Min + ImVec2(total_bb.GetSize().y, total_bb.GetSize().y));

		ImRect keybind_bb(ImRect(total_bb.Max - ImVec2(total_bb.GetSize().y + it_anim->second.keyname_size, total_bb.GetSize().y), total_bb.Max));

		it_anim->second.checkbox_col = ImLerp(it_anim->second.checkbox_col, *v ? ImColor(0.f, 0.f, 0.f, 1.f) : ImColor(0.f, 0.f, 0.f, 0.f), GetAnimSpeed());
		it_anim->second.rect_color = ImLerp(it_anim->second.rect_color, *v ? c::anim::active : GetColorWithAlpha(c::anim::active, 0.f), GetAnimSpeed());
		it_anim->second.text_color = ImLerp(it_anim->second.text_color, *v ? c::text::label::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());
		it_anim->second.size_offset = ImLerp(it_anim->second.size_offset, *v ? 1.f : 0.f, GetAnimSpeed());
		it_anim->second.keybind_color = ImLerp(it_anim->second.keybind_color, it_anim->second.keybind_mode == Waiting ? c::label::active : c::label::default, GetAnimSpeed());
		it_anim->second.tooltip_alpha = ImLerp(it_anim->second.tooltip_alpha, CalcTextSize(hint).x > 1 && total_bb.Contains(ImGui::GetMousePos()) ? 1.f : 0.f, GetAnimSpeed() * 1.5);

		GetWindowDrawList()->AddText(ImVec2(check_bb.Max.x + 10, utils::center_text(total_bb.Min, total_bb.Max, label).y), GetColorU32(it_anim->second.text_color), label);

		GetWindowDrawList()->AddRectFilled(check_bb.Min, check_bb.Max, GetColorWithAlpha(c::anim::default, c::anim::default.Value.w * style.Alpha), c::elements::rounding / 2);

		const int vtx_idx_0 = GetWindowDrawList()->VtxBuffer.Size;
		GetWindowDrawList()->AddRectFilled(check_bb.Min, check_bb.Max, GetColorU32(it_anim->second.rect_color), c::elements::rounding / 2);
		const int vtx_idx_1 = GetWindowDrawList()->VtxBuffer.Size;
		ShadeVertsLinearColorGradientKeepAlpha(GetWindowDrawList(), vtx_idx_0, vtx_idx_1, check_bb.Min, check_bb.Max, utils::GetColorWithAlpha(GetDarkColor(GetColorU32(it_anim->second.rect_color)), it_anim->second.rect_color.w), utils::GetColorWithAlpha(GetColorU32(it_anim->second.rect_color), it_anim->second.rect_color.w));

		GetWindowDrawList()->AddRect(check_bb.Min, check_bb.Max, GetColorU32(c::child::stroke), c::elements::rounding / 2);

		RenderCheckMark(GetWindowDrawList(), check_bb.GetCenter() - (ImVec2(9.f, 9.f) * it_anim->second.size_offset) / 2, GetColorU32(it_anim->second.checkbox_col), 9.f * it_anim->second.size_offset);

		if (pressed && !keybind_bb.Contains(GetMousePos()))
			*v = !(*v);

		if (CalcTextSize(hint).x && it_anim->second.tooltip_alpha > 0.1f)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, it_anim->second.tooltip_alpha);

			ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
			ImGui::PushStyleColor(ImGuiCol_PopupBg, utils::ImColorToImVec4(c::anim::default));
			ImGui::PushStyleColor(ImGuiCol_Border, c::stroke);
			ImGui::BeginTooltip();

			ImGui::Text(hint);

			ImGui::EndTooltip();
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar(4);
		}

		PushFont(font::regular_s);
		if (keybind_used)
		{
			it_anim->second.keyname_size = ImLerp(it_anim->second.keyname_size, CalcTextSize(it_anim->second.keybind_name).x, c::anim::speed);

			GetWindowDrawList()->AddRectFilled(keybind_bb.Min, keybind_bb.Max, c::anim::default, c::elements::rounding / 2);
			GetWindowDrawList()->AddRect(keybind_bb.Min, keybind_bb.Max, GetColorU32(c::child::stroke), c::elements::rounding / 2);

			switch (it_anim->second.keybind_mode) {
			case None:
				it_anim->second.keybind_name = ICON_KEYBOARD_FILL;
				break;
			case Waiting:
				it_anim->second.keybind_name = "...";
				break;
			case Used:
				it_anim->second.keybind_name = keys[*key];
				break;
			default:
				it_anim->second.keybind_name = "Unknown";
				break;
			}

			GetWindowDrawList()->AddText(keybind_bb.GetCenter() - CalcTextSize(it_anim->second.keybind_name) / 2, GetColorU32(it_anim->second.keybind_color), it_anim->second.keybind_name);

			if ((it_anim->second.keybind_mode == None || it_anim->second.keybind_mode == Used) && keybind_bb.Contains(GetMousePos()) && IsItemClicked())
			{
				it_anim->second.keybind_mode = Waiting;
			}

			if (it_anim->second.keybind_mode == Waiting)
			{
				// ESC zum Abbrechen
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					*key = 0;
					it_anim->second.keybind_mode = None;
				}
				else {
					bool key_detected = false;

					// Maus-Buttons (außer wenn auf Keybind-Box geklickt)
					for (int i = VK_LBUTTON; i <= VK_XBUTTON2; i++) {
						if ((GetAsyncKeyState(i) & 0x8000) && !keybind_bb.Contains(GetMousePos())) {
							*key = i;
							it_anim->second.keybind_mode = Used;
							key_detected = true;
							break;
						}
					}

					if (!key_detected) {
						for (int i = 0x08; i <= 0xA5; i++) {
							if (i >= VK_LBUTTON && i <= VK_XBUTTON2) continue; // Skip mouse buttons

							if (GetAsyncKeyState(i) & 0x8000) {
								*key = i;
								it_anim->second.keybind_mode = Used;
								break;
							}
						}
					}
				}
			}
		}
		PopFont();
		return pressed;
	}

	void	LogotypeHeader(const char* text, const char* version)
	{
		ImGuiWindow* window = GetCurrentWindow();

		if (window->SkipItems)
			return;

		const ImVec2 pos = window->DC.CursorPos;
		ImRect total_bb(pos, pos + ImVec2(220, 55));

		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorU32(c::child::background), c::elements::rounding);
		window->DrawList->AddRect(total_bb.Min, total_bb.Max, GetColorU32(c::child::stroke), c::elements::rounding);

		PushFont(font::s_inter_semibold);
		window->DrawList->AddText(ImVec2(total_bb.Min.x + 15.f, utils::center_text(total_bb.Min, total_bb.Max, text).y), c::label::active, text);

		ImGui::SetCursorScreenPos(ImVec2(total_bb.Min.x + 20.f + CalcTextSize(text).x, utils::center_text(total_bb.Min, total_bb.Max, text).y));
		custom::CategoryText(version);
		PopFont();

	}

	struct checkboxclicked_state
	{
		ImVec4 text_color_offset;
		ImVec4 rect_color;
		ImVec4 circle_color;
		float circle_offset;
	};

	bool CheckboxClicked(const char* label, bool* v)
	{
		ImGuiWindow* window = GetCurrentWindow();
		std::string name = label;

		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		static std::map<ImGuiID, checkboxclicked_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, checkboxclicked_state() });
			it_anim = anim.find(id);
		}

		const float square_sz = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		ImRect total_bb(pos, pos + ImVec2(75 + CalcTextSize(label).x, 50));

		ItemSize(total_bb, style.FramePadding.y);
		ItemAdd(total_bb, id);


		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed) *v = !(*v);

		it_anim->second.circle_offset = ImLerp(it_anim->second.circle_offset, *v ? 20.f : 0.f, GetAnimSpeed());
		it_anim->second.rect_color = ImLerp(it_anim->second.rect_color, *v ? c::anim::active : ImColor(0.1f, 0.1f, 0.1f, 0.5f), GetAnimSpeed());
		it_anim->second.circle_color = ImLerp(it_anim->second.circle_color, *v ? ImColor(1.f, 1.f, 1.f, 1.f) : ImColor(0.6f, 0.6f, 0.6f, 1.f), GetAnimSpeed());
		it_anim->second.text_color_offset = ImLerp(it_anim->second.text_color_offset, *v ? ImColor(0.f, 0.f, 0.f, 0.0f) : ImColor(0.f, 0.f, 0.f, 0.5f), GetAnimSpeed());

		window->DrawList->AddRectFilled(total_bb.Max - ImVec2(60, 35), total_bb.Max - ImVec2(20, 15), GetColorU32(it_anim->second.rect_color), 25);
		window->DrawList->AddRect(total_bb.Max - ImVec2(60, 35), total_bb.Max - ImVec2(20, 15), second_color, 25, 0, 1.5f);
		window->DrawList->AddCircleFilled(total_bb.Max - ImVec2(50 - it_anim->second.circle_offset, 25), 7.f, GetColorU32(it_anim->second.circle_color), 60);

		window->DrawList->AddText(ImVec2(total_bb.Min.x + 5.f, utils::center_text(total_bb.Min, total_bb.Max, label).y), c::label::active, label);
		return pressed;
	}

	static float CalcMaxPopupHeightFromItemCount(int items_count)
	{
		ImGuiContext& g = *GImGui;
		if (items_count <= 0)
			return FLT_MAX;
		return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}



	int rotation_start_index;
	void ImRotateStart()
	{
		rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
	}

	ImVec2 ImRotationCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

		const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotation_start_index; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
	{
		float s = sin(rad), c = cos(rad);
		center = ImRotate(center, s, c) - center;

		auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotation_start_index; i < buf.Size; i++)
			buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
	}

	struct begin_state
	{
		ImVec4 background, text;
		float open, alpha, combo_size = 0.f, shadow_opticaly;
		bool opened_combo = false, hovered = false;
		float arrow_roll;
	};

	static std::map<ImGuiID, begin_state> g_combo_state_map;


	bool BeginCombo(const char* label, const char* preview_value, int val, bool multi, ImGuiComboFlags flags)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();

		g.NextWindowData.ClearFlags();
		if (window->SkipItems) return false;

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const ImVec2 pos = window->DC.CursorPos;
		const ImVec2 label_size = CalcTextSize(label, NULL, true);
		const float w = GetContentRegionAvail().x;
		const ImRect total_bb(pos, pos + ImVec2(w, 35));
		const ImRect bb_box(total_bb);
		const ImRect bb(bb_box);

		ItemSize(total_bb, 0.f);
		if (!ItemAdd(total_bb, id, &bb)) return false;

		bool hovered, held, pressed = ButtonBehavior(total_bb, id, &hovered, &held);

		auto it_anim = g_combo_state_map.find(id);

		if (it_anim == g_combo_state_map.end())
		{
			g_combo_state_map.insert({ id, begin_state() });
			it_anim = g_combo_state_map.find(id);
		}

		if (hovered && g.IO.MouseClicked[0] || it_anim->second.opened_combo && g.IO.MouseClicked[0] && !it_anim->second.hovered)
			it_anim->second.opened_combo = !it_anim->second.opened_combo;

		it_anim->second.arrow_roll = ImLerp(it_anim->second.arrow_roll, it_anim->second.opened_combo ? -1.f : 1.f, g.IO.DeltaTime * 6.f);
		it_anim->second.text = ImLerp(it_anim->second.text, it_anim->second.opened_combo ? c::text::label::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());
		it_anim->second.background = ImLerp(it_anim->second.background, it_anim->second.opened_combo ? c::second_color : c::elements::background, g.IO.DeltaTime * 6.f);
		it_anim->second.combo_size = ImLerp(it_anim->second.combo_size, it_anim->second.opened_combo ? (val * 50) + 14 : 0.f, g.IO.DeltaTime * 12.f);

		GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, c::anim::default, c::elements::rounding);
		GetWindowDrawList()->AddRect(bb.Min, bb.Max, c::stroke_color, c::elements::rounding);

		GetWindowDrawList()->AddText(ImVec2(total_bb.Min.x + 10, utils::center_text(total_bb.Min, total_bb.Max, label).y), GetColorU32(it_anim->second.text), label);

		if (preview_value && preview_value[0] != '\0') {
			GetWindowDrawList()->AddText(ImVec2(bb.Max.x - 50.f - CalcTextSize(preview_value).x, utils::center_text(total_bb.Min, total_bb.Max, preview_value).y), c::label::active, preview_value);
		}

		ImRotateStart();
		PushClipRect(bb.Min, bb.Max, true);
		GetWindowDrawList()->AddText(ImVec2(bb.Max.x - 33.f, utils::center_text(total_bb.Min, total_bb.Max, ICON_DOWN_LINE).y), utils::GetColorWithAlpha(c::anim::active, style.Alpha), ICON_DOWN_LINE);
		PopClipRect();
		ImRotateEnd(1.57f * it_anim->second.arrow_roll);

		if (!IsRectVisible(bb.Min, bb.Max + ImVec2(0, 2)))
		{
			it_anim->second.opened_combo = false;
			it_anim->second.combo_size = 0.f;
		}

		if (!it_anim->second.opened_combo && it_anim->second.combo_size < 2.f) return false;

		ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + 5));
		ImGui::SetNextWindowSize(ImVec2(bb.GetWidth(), it_anim->second.combo_size));

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;

		PushStyleColor(ImGuiCol_WindowBg, utils::ImColorToImVec4(ImColor(15, 15, 18, 255)));
		PushStyleVar(ImGuiStyleVar_WindowRounding, c::elements::rounding);
		PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 15));
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		bool ret = Begin(label, NULL, window_flags);

		PopStyleVar(3);
		PopStyleColor(1);

		ImGui::PushClipRect(ImVec2(0, 0), ImGui::GetMainViewport()->Size, false);
		ImGui::GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), ImColor(15, 15, 18, 255), 10.f);
		ImGui::GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), GetColorU32(c::child::stroke), 10.f);
		ImGui::PopClipRect();

		it_anim->second.hovered = IsWindowHovered();

		return true;
	}

	void EndCombo()
	{
		End();
	}

	void MultiCombo(const char* label, bool variable[], const char* labels[], int count)
	{
		ImGuiContext& g = *GImGui;

		std::string preview = "None";

		for (auto i = 0, j = 0; i < count; i++)
		{
			if (variable[i])
			{
				if (j)
					preview += (", ") + (std::string)labels[i];
				else
					preview = labels[i];

				j++;
			}
		}

		if (BeginCombo(label, preview.c_str(), count))
		{
			for (auto i = 0; i < count; i++)
			{
				PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
				PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));

				custom::Selectable(split_string(labels[i], false).c_str(), &variable[i], ImGuiSelectableFlags_DontClosePopups, ImVec2(0, 30), split_string(labels[i], false).c_str());

				PopStyleVar(2);
			}
			End();
		}

		preview = ("None");
	}

	bool BeginComboPreview()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

		if (window->SkipItems || !(g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible)) return false;

		IM_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y);

		if (!window->ClipRect.Overlaps(preview_data->PreviewRect)) return false;

		preview_data->BackupCursorPos = window->DC.CursorPos;
		preview_data->BackupCursorMaxPos = window->DC.CursorMaxPos;
		preview_data->BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
		preview_data->BackupPrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
		preview_data->BackupLayout = window->DC.LayoutType;
		window->DC.CursorPos = preview_data->PreviewRect.Min + g.Style.FramePadding;
		window->DC.CursorMaxPos = window->DC.CursorPos;
		window->DC.LayoutType = ImGuiLayoutType_Horizontal;
		window->DC.IsSameLine = false;
		PushClipRect(preview_data->PreviewRect.Min, preview_data->PreviewRect.Max, true);

		return true;
	}

	void EndComboPreview()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

		ImDrawList* draw_list = window->DrawList;
		if (window->DC.CursorMaxPos.x < preview_data->PreviewRect.Max.x && window->DC.CursorMaxPos.y < preview_data->PreviewRect.Max.y)
			if (draw_list->CmdBuffer.Size > 1)
			{
				draw_list->_CmdHeader.ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 2].ClipRect;
				draw_list->_TryMergeDrawCmds();
			}
		PopClipRect();
		window->DC.CursorPos = preview_data->BackupCursorPos;
		window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, preview_data->BackupCursorMaxPos);
		window->DC.CursorPosPrevLine = preview_data->BackupCursorPosPrevLine;
		window->DC.PrevLineTextBaseOffset = preview_data->BackupPrevLineTextBaseOffset;
		window->DC.LayoutType = preview_data->BackupLayout;
		window->DC.IsSameLine = false;
		preview_data->PreviewRect = ImRect();
	}

	static const char* Items_ArrayGetter(void* data, int idx)
	{
		const char* const* items = (const char* const*)data;
		return items[idx];
	}

	static const char* Items_SingleStringGetter(void* data, int idx)
	{
		const char* items_separated_by_zeros = (const char*)data;
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p)
		{
			if (idx == items_count)
				break;
			p += strlen(p) + 1;
			items_count++;
		}
		return *p ? p : NULL;
	}

	bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items)
	{
		ImGuiContext& g = *GImGui;

		const char* preview_value = NULL;
		if (*current_item >= 0 && *current_item < items_count)
			preview_value = getter(user_data, *current_item);

		std::string preivew_value_str = preview_value;

		if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
			SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

		if (!BeginCombo(label, preivew_value_str.find("$") != std::string::npos ? split_string(preview_value, false).c_str() : preview_value, items_count, false, ImGuiComboFlags_None)) return false;

		bool value_changed = false;
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
		for (int i = 0; i < items_count; i++)
		{
			const char* item_text = getter(user_data, i);
			if (item_text == NULL)
				item_text = "*Unknown item*";

			PushID(i);
			const bool item_selected = (i == *current_item);

			std::string item_text_str = item_text;

			if (item_text_str.find("$") != std::string::npos) {
				if (custom::Selectable(split_string(item_text, false).c_str(), item_selected, 0, ImVec2(0, 35), split_string(item_text, true).c_str()))
				{
					if (*current_item != i) {
						value_changed = true;
						*current_item = i;
					}
				}
			}
			else
			{
				if (custom::Selectable(item_text, item_selected, 0, ImVec2(0, 35)))
				{
					if (*current_item != i) {
						value_changed = true;
						*current_item = i;
					}
				}
			}
			if (item_selected)
				SetItemDefaultFocus();
			PopID();
		}
		PopStyleVar();

		EndCombo();

		if (value_changed) {
			ImGuiID id = ImGui::GetID(label);
			auto it_anim = g_combo_state_map.find(id);
			if (it_anim != g_combo_state_map.end()) {
				it_anim->second.opened_combo = false;
			}
			MarkItemEdited(g.LastItemData.ID);
		}

		return value_changed;
	}

	bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
	{
		const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
		return value_changed;
	}

	bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
	{
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p)
		{
			p += strlen(p) + 1;
			items_count++;
		}
		bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
		return value_changed;
	}



	struct select_state
	{
		ImVec4 text, background, stroke;
		float circle_radius, text_offset;
	};

	bool Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg, const char* description)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		ImGuiID id = window->GetID(label);
		ImVec2 label_size = CalcTextSize(label, NULL, true);
		ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y);
		ImVec2 pos = window->DC.CursorPos;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ItemSize(size, 0.0f);

		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth)) size.x = ImMax(label_size.x, max_x - min_x);

		const ImVec2 text_min = pos;
		const ImVec2 text_max(min_x + size.x, pos.y + size.y);

		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		{
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
			const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}

		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns)
		{
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
		const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
		if (span_all_columns)
		{
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add) return false;

		const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
		if (disabled_item && !disabled_global) BeginDisabled();

		if (span_all_columns && window->DC.CurrentColumns) PushColumnsBackground();
		else if (span_all_columns && g.CurrentTable) TablePushBackgroundChannel();

		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
		if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

		const bool was_selected = selected;
		bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

		if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
			if (g.NavJustMovedToId == id)  selected = pressed = true;

		// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		{
			if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
			{
				SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
				g.NavDisableHighlight = true;
			}
		}
		if (pressed) MarkItemEdited(id);

		if (selected != was_selected)  g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;


		if (g.NavId == id) RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

		if (span_all_columns && window->DC.CurrentColumns) PopColumnsBackground();
		else if (span_all_columns && g.CurrentTable) TablePopBackgroundChannel();

		static std::map<ImGuiID, select_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, select_state() });
			it_anim = anim.find(id);
		}

		it_anim->second.text = ImLerp(it_anim->second.text, selected ? c::label::active : c::label::default, GetAnimSpeed());
		it_anim->second.circle_radius = ImLerp(it_anim->second.circle_radius, selected ? 3.f : 0.f, GetAnimSpeed());
		it_anim->second.text_offset = ImLerp(it_anim->second.text_offset, selected ? 20.f : 4.5f, GetAnimSpeed());
		it_anim->second.background = ImLerp(it_anim->second.background, selected ? c::background_color : utils::GetColorWithAlpha(c::background_color, 0.f), GetAnimSpeed());

		it_anim->second.stroke = ImLerp(it_anim->second.stroke, selected ? ImColor(1.f, 1.f, 1.f, 0.05f) :
			ImColor(1.f, 1.f, 1.f, 0.0f), GetAnimSpeed());

		window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(it_anim->second.stroke), c::elements::rounding);

		window->DrawList->AddText(ImVec2(bb.Min.x + 18.5f, utils::center_text(bb.Min, bb.Max, label).y), GetColorU32(it_anim->second.text), label);

		window->DrawList->AddText(ImVec2(bb.Max.x - 18.5f - CalcTextSize(description).x, utils::center_text(bb.Min, bb.Max, description).y), c::anim::active, description);

		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup)) CloseCurrentPopup();

		if (disabled_item && !disabled_global) EndDisabled();

		return pressed;
	}

	bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		if (Selectable(label, *p_selected, flags, size_arg))
		{
			*p_selected = !*p_selected;
			return true;
		}
		return false;
	}

	static void ColorEditRestoreH(const float* col, float* H)
	{
		ImGuiContext& g = *GImGui;
		IM_ASSERT(g.ColorEditCurrentID != 0);
		if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
			return;
		*H = g.ColorEditSavedHue;
	}

	static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
	{
		ImGuiContext& g = *GImGui;
		IM_ASSERT(g.ColorEditCurrentID != 0);
		if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0))) return;

		if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
			*H = g.ColorEditSavedHue;

		if (*V == 0.0f) *S = g.ColorEditSavedSat;
	}


	struct edit_state
	{
		ImVec4 text;
		bool active;
		float alpha;
		ImVec4 icon;
	};

	bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const float square_sz = GetFrameHeight();
		const float w_full = CalcItemWidth();
		const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
		const float w_inputs = w_full - w_button;
		const char* label_display_end = FindRenderedTextEnd(label);
		g.NextItemData.ClearFlags();

		BeginGroup();
		PushID(label);
		const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
		if (set_current_color_edit_id)
			g.ColorEditCurrentID = window->IDStack.back();

		// If we're not showing any slider there's no point in doing any HSV conversions
		const ImGuiColorEditFlags flags_untouched = flags;
		if (flags & ImGuiColorEditFlags_NoInputs)
			flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

		// Context menu: display and modify options (before defaults are applied)
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ColorEditOptionsPopup(col, flags);

		// Read stored options
		if (!(flags & ImGuiColorEditFlags_DisplayMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
		if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
		if (!(flags & ImGuiColorEditFlags_PickerMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
		if (!(flags & ImGuiColorEditFlags_InputMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
		flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

		const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
		const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
		const int components = alpha ? 4 : 3;

		// Convert to the formats we need
		float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
		if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
			ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
		{
			// Hue is lost when converting from grayImVec2 rgb (saturation=0). Restore it.
			ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
			ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
		}
		int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

		bool value_changed = false;
		bool value_changed_as_float = false;

		const ImVec2 pos = window->DC.CursorPos;
		const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
		window->DC.CursorPos.x = pos.x + inputs_offset_x;

		if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
		{
			// RGB/HSV 0..255 Sliders
			const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
			const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

			const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
			static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
			static const char* fmt_table_int[3][4] =
			{
				{   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
				{ "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
				{ "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
			};
			static const char* fmt_table_float[3][4] =
			{
				{   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
				{ "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
				{ "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
			};
			const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

			for (int n = 0; n < components; n++)
			{
				if (n > 0)
					SameLine(0, style.ItemInnerSpacing.x);
				SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

				// FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
				if (flags & ImGuiColorEditFlags_Float)
				{
					value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
					value_changed_as_float |= value_changed;
				}
				else
				{
					value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
				}
				if (!(flags & ImGuiColorEditFlags_NoOptions))
					OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
			}
		}
		else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
		{

			if (!(flags & ImGuiColorEditFlags_NoOptions))
				OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		}

		// RGB Hexadecimal Input
		char buf[64];
		ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));


		static std::map<ImGuiID, edit_state> anim;
		auto it_anim = anim.find(ImGui::GetID(label));

		if (it_anim == anim.end())
		{
			anim.insert({ ImGui::GetID(label), edit_state() });
			it_anim = anim.find(ImGui::GetID(label));
		}

		ImGuiWindow* picker_active_window = NULL;
		if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
		{
			const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
			window->DC.CursorPos = ImVec2(pos.x, pos.y);

			const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
			if (ColorButton("##ColorButton", col_v4, flags))
			{
				if (!(flags & ImGuiColorEditFlags_NoPicker))
				{
					g.ColorPickerRef = col_v4;
					OpenPopup("picker");

					// Зададим позицию для всплывающего окна
					ImVec2 popup_position = g.LastItemData.Rect.GetBR();
					ImGui::SetNextWindowPos(popup_position);
				}
			}

			if (!(flags & ImGuiColorEditFlags_NoOptions))
				OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

			PushStyleColor(ImGuiCol_PopupBg, utils::ImColorToImVec4(c::window_bg_color));
			PushStyleVar(ImGuiStyleVar_PopupRounding, c::child::rounding);


			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, it_anim->second.alpha);
			// Текущая позиция окна будет установлена перед вызовом BeginPopup
			if (BeginPopup("picker"))
			{
				if (g.CurrentWindow->BeginCount == 1)
				{

					it_anim->second.active = true;
					picker_active_window = g.CurrentWindow;

					ImGuiColorEditFlags picker_flags_to_forward =
						ImGuiColorEditFlags_DataTypeMask_ |
						ImGuiColorEditFlags_PickerMask_ |
						ImGuiColorEditFlags_InputMask_ |
						ImGuiColorEditFlags_HDR |
						ImGuiColorEditFlags_NoAlpha |
						ImGuiColorEditFlags_AlphaBar;

					ImGuiColorEditFlags picker_flags =
						(flags_untouched & picker_flags_to_forward) |
						ImGuiColorEditFlags_DisplayMask_ |
						ImGuiColorEditFlags_NoLabel |
						ImGuiColorEditFlags_AlphaPreviewHalf;

					SetNextItemWidth(0.5f);
					value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
				}
				EndPopup();
			}
			else
				it_anim->second.active = false;
			PopStyleColor(1);
			PopStyleVar(2);

			it_anim->second.alpha = ImLerp(it_anim->second.alpha, it_anim->second.active ? 1.f : 0.3f, c::anim::speed);

		}

		if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
		{
			const float width = (GetContentRegionMax().x - style.WindowPadding.x);

			SameLine(0.0f, style.ItemInnerSpacing.x);
			window->DC.CursorPos.x = pos.x - w_button + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full);

			const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width, 19));

			it_anim->second.text = ImLerp(it_anim->second.text, ImGui::IsPopupOpen("picker") ? c::text::label::active : IsMouseHoveringRect(rect.Min, rect.Max, true) ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());

			it_anim->second.icon = ImLerp(it_anim->second.icon, ImGui::IsPopupOpen("picker") ? c::anim::active : IsMouseHoveringRect(rect.Min, rect.Max, true) ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());


			//window->DC.CursorPos + ImVec2(0, (32 - CalcTextSize(buf).y) / 2)

			window->DrawList->AddText(ImVec2(rect.Min.x, utils::center_text(rect.Min, rect.Max, label).y), GetColorU32(it_anim->second.text), label);

		}

		// Convert back
		if (value_changed && picker_active_window == NULL)
		{
			if (!value_changed_as_float)
				for (int n = 0; n < 4; n++)
					f[n] = i[n] / 255.0f;
			if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
			{
				g.ColorEditSavedHue = f[0];
				g.ColorEditSavedSat = f[1];
				ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
				g.ColorEditSavedID = g.ColorEditCurrentID;
				g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
			}
			if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
				ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}

		if (set_current_color_edit_id)
			g.ColorEditCurrentID = 0;
		PopID();
		EndGroup();

		// Drag and Drop Target
		// NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
		if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
		{
			bool accepted_drag_drop = false;
			if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
			{
				memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
				value_changed = accepted_drag_drop = true;
			}
			if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
			{
				memcpy((float*)col, payload->Data, sizeof(float) * components);
				value_changed = accepted_drag_drop = true;
			}

			// Drag-drop payloads are always RGB
			if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
				ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
			EndDragDropTarget();
		}

		// When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
		if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
			g.LastItemData.ID = g.ActiveId;

		if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
			MarkItemEdited(g.LastItemData.ID);

		return value_changed;
	}


	// Helper for ColorPicker4()
	static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
	{
		ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
		ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32(0, 0, 0, alpha8));
		ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32(255, 255, 255, alpha8));
		ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32(0, 0, 0, alpha8));
		ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32(255, 255, 255, alpha8));
	}

	struct picker_state
	{
		float hue_bar;
		float alpha_bar;
		float circle;
		ImVec2 circle_move;
	};

	static void color_edit_restore_h(const float* col, float* H)
	{
		ImGuiContext& g = *GImGui;
		IM_ASSERT(g.ColorEditCurrentID != 0);
		if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
			return;
		*H = g.ColorEditSavedHue;
	}

	static void color_edit_restore_hs(const float* col, float* H, float* S, float* V)
	{
		ImGuiContext& g = *GImGui;
		IM_ASSERT(g.ColorEditCurrentID != 0);
		if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
			return;

		// When S == 0, H is undefined.
		// When H == 1 it wraps around to 0.
		if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
			*H = g.ColorEditSavedHue;

		// When V == 0, S is undefined.
		if (*V == 0.0f)
			*S = g.ColorEditSavedSat;
	}


	bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImDrawList* draw_list = window->DrawList;
		ImGuiStyle& style = g.Style;
		ImGuiIO& io = g.IO;

		const float width = CalcItemWidth();
		g.NextItemData.ClearFlags();

		PushID(label);
		BeginGroup();

		if (!(flags & ImGuiColorEditFlags_NoSidePreview))
			flags |= ImGuiColorEditFlags_NoSmallPreview;

		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ColorPickerOptionsPopup(col, flags);

		// Read stored options
		if (!(flags & ImGuiColorEditFlags_PickerMask_))
			flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
		if (!(flags & ImGuiColorEditFlags_InputMask_))
			flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_)); // Check that only 1 is selected
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));  // Check that only 1 is selected
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

		// Setup
		int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
		bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
		ImVec2 picker_pos = window->DC.CursorPos;
		ImVec2 bar_pos = window->DC.CursorPos + ImVec2(0, 133);
		float square_sz = GetFrameHeight();
		float bars_width = 209.f; // Arbitrary smallish width of Hue/Alpha picking bars
		float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)) + 0; // Saturation/Value picking box
		float sv_bar_size = 20; // Saturation/Value picking box
		float bar0_pos_x = GetWindowPos().x + style.WindowPadding.x;
		float bar1_pos_x = bar0_pos_x;
		float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

		float backup_initial_col[4];
		memcpy(backup_initial_col, col, components * sizeof(float));

		float H = col[0], S = col[1], V = col[2];
		float R = col[0], G = col[1], B = col[2];
		if (flags & ImGuiColorEditFlags_InputRGB)
		{
			// Hue is lost when converting from greyImVec2 rgb (saturation=0). Restore it.
			ColorConvertRGBtoHSV(R, G, B, H, S, V);
			ColorEditRestoreHS(col, &H, &S, &V);
		}
		else if (flags & ImGuiColorEditFlags_InputHSV)
		{
			ColorConvertHSVtoRGB(H, S, V, R, G, B);
		}

		bool value_changed = false, value_changed_h = false, value_changed_sv = false;

		PushItemFlag(ImGuiItemFlags_NoNav, true);
		// SV rectangle logic
		InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size - 80));
		if (IsItemActive())
		{
			S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
			V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 81));
			color_edit_restore_h(col, &H); // Greatly reduces hue jitter and reset to 0 when hue == 255 and color is rapidly modified using SV square.
			value_changed = value_changed_sv = true;
		}

		SetCursorScreenPos(ImVec2(bar0_pos_x, bar_pos.y));
		InvisibleButton("hue", ImVec2(bars_width, sv_bar_size));
		if (IsItemActive())
		{
			H = 1.0f - ImSaturate((io.MousePos.x - picker_pos.x) / (bars_width - 1));
			value_changed = value_changed_h = true;
		}

		// Alpha bar logic
		if (alpha_bar)
		{
			SetCursorScreenPos(ImVec2(bar1_pos_x, bar_pos.y + 16));
			InvisibleButton("alpha", ImVec2(bars_width, sv_bar_size));
			if (IsItemActive())
			{
				col[3] = ImSaturate((io.MousePos.x - bar_pos.x) / (bars_width - 1));
				value_changed = true;
			}
		}
		PopItemFlag(); // ImGuiItemFlags_NoNav

		if (value_changed_h || value_changed_sv)
		{
			if (flags & ImGuiColorEditFlags_InputRGB)
			{
				ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
				g.ColorEditSavedHue = H;
				g.ColorEditSavedSat = S;
				g.ColorEditSavedID = g.ColorEditCurrentID;
				g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
			}
			else if (flags & ImGuiColorEditFlags_InputHSV)
			{
				col[0] = H;
				col[1] = S;
				col[2] = V;
			}
		}

		// R,G,B and H,S,V slider color editor
		bool value_changed_fix_hue_wrap = false;

		// Try to cancel hue wrap (after ColorEdit4 call), if any
		if (value_changed_fix_hue_wrap && (flags & ImGuiColorEditFlags_InputRGB))
		{
			float new_H, new_S, new_V;
			ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
			if (new_H <= 0 && H > 0)
			{
				if (new_V <= 0 && V != new_V)
					ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
				else if (new_S <= 0)
					ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
			}
		}

		if (value_changed)
		{
			if (flags & ImGuiColorEditFlags_InputRGB)
			{
				R = col[0];
				G = col[1];
				B = col[2];
				ColorConvertRGBtoHSV(R, G, B, H, S, V);
				color_edit_restore_hs(col, &H, &S, &V);   // Fix local Hue as display below will use it immediately.
			}
			else if (flags & ImGuiColorEditFlags_InputHSV)
			{
				H = col[0];
				S = col[1];
				V = col[2];
				ColorConvertHSVtoRGB(H, S, V, R, G, B);
			}
		}
		ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha)); // Important: this is still including the main rendering/style alpha!!

		const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
		const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
		const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
		const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
		const ImU32 col_hues[7] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(0,0,255,style_alpha8),IM_COL32(0,255,255,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

		ImVec4 hue_color_f(1, 1, 1, style.Alpha); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
		ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);

		ImVec2 sv_cursor_pos;

		// Render SV Square
		const int vtx_idx_0 = draw_list->VtxBuffer.Size;
		draw_list->AddRectFilled(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size - 2 - 80), col_white, 4.0f);
		const int vtx_idx_1 = draw_list->VtxBuffer.Size;
		ShadeVertsLinearColorGradientKeepAlpha(draw_list, vtx_idx_0, vtx_idx_1, picker_pos, picker_pos + ImVec2(sv_picker_size, 0.0f), col_white, hue_color32);

		draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size - 80), 0, 0, col_black, col_black, 4.f);

		sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size), picker_pos.x, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
		sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * (sv_picker_size - 80)), picker_pos.y + 2, picker_pos.y + sv_picker_size - 80);

		static std::map<ImGuiID, picker_state> anim;
		auto it_anim = anim.find(ImGui::GetID(label));

		if (it_anim == anim.end())
		{
			anim.insert({ ImGui::GetID(label), picker_state() });
			it_anim = anim.find(ImGui::GetID(label));
		}

		for (int i = 0; i < 6; ++i)
			GetForegroundDrawList()->AddRectFilledMultiColor(ImVec2(picker_pos.x + i * (bars_width / 6) - (i == 5 ? 1 : 0), picker_pos.y + 139), ImVec2(picker_pos.x + (i + 1) * (bars_width / 6) + (i == 0 ? 1 : 0), picker_pos.y + 132 + sv_bar_size - 7), col_hues[i], col_hues[i + 1], col_hues[i + 1], col_hues[i], 10.f, i == 0 ? ImDrawFlags_RoundCornersLeft : i == 5 ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);

		float bar0_line_x = IM_ROUND(bar_pos.x + (1.f - H) * bars_width);

		bar0_line_x = ImClamp(bar0_line_x, picker_pos.x + 3.f, picker_pos.x + 204.f);

		it_anim->second.hue_bar = ImLerp(it_anim->second.hue_bar, bar0_line_x - bar_pos.x, g.IO.DeltaTime * 24.f);

		GetForegroundDrawList()->AddCircleFilled(ImVec2(it_anim->second.hue_bar + bar_pos.x, bar_pos.y + 9), 6.5f, ImColor(255, 255, 255, 255), 30.f);

		it_anim->second.circle_move = ImLerp(it_anim->second.circle_move, sv_cursor_pos - bar_pos, g.IO.DeltaTime * 24.f);
		it_anim->second.circle = ImLerp(it_anim->second.circle, value_changed_sv ? 4.f : 7.f, g.IO.DeltaTime * 24.f);

		GetForegroundDrawList()->AddCircle(it_anim->second.circle_move + bar_pos + ImVec2(0, 1), it_anim->second.circle, ImColor(255, 255, 255, 255), 32);

		if (alpha_bar)
		{
			float alpha = ImSaturate(col[3]);
			ImRect bar1_bb(bar1_pos_x, bar_pos.y + 20, bar1_pos_x + bars_width, bar_pos.y + 20 + sv_bar_size);

			draw_list->AddRectFilledMultiColor(picker_pos + ImVec2(0, 161), picker_pos + ImVec2(bars_width, 147 + sv_bar_size), col_black, user_col32_striped_of_alpha, user_col32_striped_of_alpha, col_black, 10.f);

			float bar1_line_x = IM_ROUND(bar_pos.x + alpha * bars_width);

			bar1_line_x = ImClamp(bar1_line_x, bar_pos.x, picker_pos.x + 200.f);
			it_anim->second.alpha_bar = ImLerp(it_anim->second.alpha_bar, bar1_line_x - bar_pos.x + 5.f, g.IO.DeltaTime * 24.f);
			GetForegroundDrawList()->AddCircleFilled(ImVec2(it_anim->second.alpha_bar + bar_pos.x, bar1_bb.Min.y + 11.0f), 6.5f, ImColor(255, 255, 255, 255), 100.f);
		}

		EndGroup();

		if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0) value_changed = false;
		if (value_changed) MarkItemEdited(g.LastItemData.ID);

		PopID();
		return value_changed;
	}

	bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size_arg)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiID id = window->GetID(desc_id);
		const float default_size = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const float width = GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x;
		const ImRect rect(pos, pos + ImVec2(width, 19));

		const ImRect clickable(rect.Min + ImVec2(width - 25, 0), rect.Max - ImVec2(7, 0));

		ItemSize(ImRect(rect.Min, rect.Max - ImVec2(0, 0)));
		if (!ItemAdd(rect, id)) return false;

		bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

		if (flags & ImGuiColorEditFlags_NoAlpha) flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

		ImVec4 col_rgb = col;
		if (flags & ImGuiColorEditFlags_InputHSV) ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

		GetWindowDrawList()->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(col_rgb), c::elements::rounding);

		RenderColorRectWithAlphaCheckerboard(window->DrawList, clickable.Min, clickable.Max, GetColorU32(col_rgb), ImMin(36, 29) / 2.99f, ImVec2(0.f, 0.f), c::elements::rounding);


		return pressed;
	}

	struct knob_state {
		float plus_float;
		int plus_int;
		ImVec4 background, circle, text;
		float slow_anim, circle_anim;
		float position;
	};


	bool KnobScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = GetContentRegionMax().x - style.WindowPadding.x;

		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const ImRect frame_bb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(w, 32));

		const ImRect slider_bb(window->DC.CursorPos + ImVec2(w - 30, 0), window->DC.CursorPos + ImVec2(w, 100));

		const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? label_size.x : 0.0f, 0.0f));

		const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
		ItemSize(ImRect(total_bb.Min, total_bb.Max - ImVec2(0, 0)));

		if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

		if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

		bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags), held, pressed = ButtonBehavior(frame_bb, id, &hovered, &held, NULL);

		ImRect grab_bb;

		static std::map<ImGuiID, knob_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, knob_state() });
			it_anim = anim.find(id);
		}

		it_anim->second.circle_anim = ImLerp(it_anim->second.circle_anim, IsItemActive() ? 11.f : 10.f, g.IO.DeltaTime * 6.f);

		if ((flags & ImGuiSliderFlags_Integer) == 0) {
			it_anim->second.plus_float = ImLerp(it_anim->second.plus_float, *(float*)p_data <= *(float*)p_max && hovered && GetAsyncKeyState(VK_OEM_PLUS) & 0x01 ? *(float*)p_data += 0.05f : *(float*)p_data >= *(float*)p_min && hovered && GetAsyncKeyState(VK_OEM_MINUS) & 0x01 ? *(float*)p_data -= 0.05f : 0, g.IO.DeltaTime * 6.f);
			if (*(float*)p_data > *(float*)p_max) *(float*)p_data = *(float*)p_max;
			if (*(float*)p_data < *(float*)p_min) *(float*)p_data = *(float*)p_min;
		}
		else
		{
			it_anim->second.plus_int = ImLerp(it_anim->second.plus_int, *(int*)p_data <= *(int*)p_max && hovered && GetAsyncKeyState(VK_OEM_PLUS) & 0x01 ? *(int*)p_data += 1 : *(int*)p_data >= *(int*)p_min && hovered && GetAsyncKeyState(VK_OEM_MINUS) & 0x01 ? *(int*)p_data -= 1 : 0, g.IO.DeltaTime * 6.f);
			if (*(int*)p_data > *(int*)p_max) *(int*)p_data = *(int*)p_max;
			if (*(int*)p_data < *(int*)p_min) *(int*)p_data = *(int*)p_min;
		}

		it_anim->second.text = ImLerp(it_anim->second.text, g.ActiveId == id ? c::label::active : hovered ? c::label::hovered : c::label::default, g.IO.DeltaTime * 6.f);

		const bool value_changed = DragBehavior(id, data_type, p_data, 0.f, p_min, p_max, format, NULL);


		if (value_changed) MarkItemEdited(id);

		char value_buf[64];
		const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

		float radius = 10.f;
		float thickness = 3.f;

		it_anim->second.position = ImLerp(it_anim->second.position, *static_cast<float*>(p_data) / *reinterpret_cast<const float*>(p_max) * 6.25f, ImGui::GetIO().DeltaTime * 18.f);

		GetWindowDrawList()->PathClear();
		GetWindowDrawList()->PathArcTo(ImVec2(frame_bb.Max.x + radius - 22.f, frame_bb.Min.y + (32 / 2)), radius, 0.f, 2.f * IM_PI, 40.f);
		GetWindowDrawList()->PathStroke(GetColorU32(c::elements::background), 0, thickness);

		GetWindowDrawList()->PathClear();
		GetWindowDrawList()->PathArcTo(ImVec2(frame_bb.Max.x + radius - 22.f, frame_bb.Min.y + (32 / 2)), radius, IM_PI * 1.5f, IM_PI * 1.5f + it_anim->second.position, 40.f);
		GetWindowDrawList()->PathStroke(GetColorU32(c::accent), 0, thickness);

		GetWindowDrawList()->AddCircleFilled(ImVec2(frame_bb.Max.x + radius - 22.f + ImCos(IM_PI * 1.5f + it_anim->second.position) * radius, frame_bb.Min.y + (32 / 2) + ImSin(IM_PI * 1.5f + it_anim->second.position) * radius), 2.f, GetColorU32(c::label::active.Value));

		GetWindowDrawList()->AddText(ImVec2(frame_bb.Max.x - (40 + CalcTextSize(value_buf).x), frame_bb.Min.y + (32 - CalcTextSize(value_buf).y) / 2), GetColorU32(c::label::default.Value), value_buf);

		GetWindowDrawList()->AddText(ImVec2(frame_bb.Max.x - w, frame_bb.Min.y + (32 - CalcTextSize(value_buf).y) / 2), GetColorU32(it_anim->second.text), label);

		return value_changed;
	}

	bool KnobFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return KnobScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
	}

	bool KnobInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		return KnobScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags | ImGuiSliderFlags_Integer);
	}


	struct slider_state {
		ImVec4 background, circle, text_color, grab_col, value_text_col, value_box_col;
		float position, slow;
		float shadow_thinkess;
		float current_value;
	};


	bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		std::string label_str = label;
		std::string arrows_str[2] = { label_str + "left", label_str + "right" };

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = GetContentRegionAvail().x;

		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const ImVec2 pos = window->DC.CursorPos;

		const ImRect total_bb(pos, pos + ImVec2(w, 35));
		const ImRect item_bb(total_bb.Min, total_bb.Max);


		const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;


		ItemSize(ImRect(total_bb.Min, total_bb.Max));

		if (!ItemAdd(total_bb, id, &total_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

		if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

		bool hovered = ItemHoverable(total_bb, id, g.LastItemData.InFlags), held, pressed = ButtonBehavior(total_bb, id, &hovered, &held, NULL);

		ImRect grab_bb;

		static std::map<ImGuiID, slider_state> anim;
		auto it_anim = anim.find(id);

		if (it_anim == anim.end())
		{
			anim.insert({ id, slider_state() });
			it_anim = anim.find(id);
		}

		char value_buf[64];
		const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

		const ImRect value_bb(total_bb.Max - ImVec2(16 + CalcTextSize(value_buf).x, total_bb.GetSize().y), total_bb.Min + ImVec2(total_bb.GetSize().x, CalcTextSize(label).y + 3));

		const ImRect slider_bb(total_bb.Min + ImVec2(0, CalcTextSize(label).y + 10), total_bb.Max - ImVec2(0, 1));


		const bool value_changed = SliderBehavior(ImRect(slider_bb.Min - ImVec2(0, 0), slider_bb.Max + ImVec2(10, 0)), id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);

		if (value_changed) MarkItemEdited(id);




		it_anim->second.shadow_thinkess = ImLerp(it_anim->second.shadow_thinkess, it_anim->second.slow > 3.f ? 40.f : 0.f, GetAnimSpeed());
		it_anim->second.slow = ImLerp(it_anim->second.slow, grab_bb.Min.x - (slider_bb.Min.x), g.IO.DeltaTime * 25.f);
		it_anim->second.text_color = ImLerp(it_anim->second.text_color, held ? c::text::label::active : hovered ? c::text::label::hovered : c::text::label::default, GetAnimSpeed());
		it_anim->second.current_value = ImLerp(it_anim->second.current_value, (float)std::stoi(value_buf), GetAnimSpeed());
		it_anim->second.grab_col = ImLerp(it_anim->second.text_color, ImGui::GetIO().MouseDelta.x != 0 && g.ActiveId == id ? c::anim::active : hovered ? c::text::label::active : c::text::label::hovered, GetAnimSpeed());

		it_anim->second.value_text_col = ImLerp(it_anim->second.value_text_col, g.ActiveId == id ? c::anim::active : it_anim->second.text_color, GetAnimSpeed());
		it_anim->second.value_box_col = ImLerp(it_anim->second.value_box_col, g.ActiveId == id ? GetColorWithAlpha(c::anim::active, 0.25f) : c::anim::default, GetAnimSpeed());
		it_anim->second.grab_col = ImLerp(it_anim->second.text_color, ImGui::GetIO().MouseDelta.x != 0 && g.ActiveId == id ? c::anim::active : hovered ? c::text::label::active : c::text::label::hovered, GetAnimSpeed());

		int new_value = std::stoi(value_buf);

		if (std::abs(new_value - it_anim->second.current_value) <= 1) {
			it_anim->second.current_value = new_value;
		}

		GetWindowDrawList()->AddRectFilled(slider_bb.Min, slider_bb.Max, utils::GetColorWithAlpha(c::anim::default, style.Alpha), c::elements::rounding / 2);
		GetWindowDrawList()->AddRect(slider_bb.Min, slider_bb.Max, GetColorU32(c::child::stroke), c::elements::rounding);



		GetWindowDrawList()->AddRectFilled(value_bb.Min, value_bb.Max, GetColorU32(it_anim->second.value_box_col), c::elements::rounding / 2);

		GetWindowDrawList()->AddRect(value_bb.Min, value_bb.Max, utils::GetColorWithAlpha(c::child::stroke, c::child::stroke.w * style.Alpha), c::elements::rounding / 2);

		const int vtx_idx_0 = GetWindowDrawList()->VtxBuffer.Size;
		GetWindowDrawList()->AddRectFilled(slider_bb.Min, ImVec2(it_anim->second.slow + (slider_bb.Min.x + 6), slider_bb.Max.y), utils::GetColorWithAlpha(c::anim::active, style.Alpha), c::elements::rounding / 2);
		const int vtx_idx_1 = GetWindowDrawList()->VtxBuffer.Size;
		ShadeVertsLinearColorGradientKeepAlpha(GetWindowDrawList(), vtx_idx_0, vtx_idx_1, slider_bb.Min, ImVec2(it_anim->second.slow + (slider_bb.Min.x + 6), slider_bb.Max.y), utils::GetColorWithAlpha(GetDarkColor(c::anim::active), style.Alpha), utils::GetColorWithAlpha(c::anim::active, style.Alpha));

		ImRect grab_rect(ImVec2(it_anim->second.slow + (slider_bb.Min.x - 4), slider_bb.Min.y - 2.f), ImVec2(it_anim->second.slow + (slider_bb.Min.x + 6), slider_bb.Max.y + 2.f));
		GetWindowDrawList()->AddCircle(grab_rect.GetCenter(), 6.f, c::text::description::default); // grab
		GetWindowDrawList()->AddCircleFilled(grab_rect.GetCenter(), 5.f, GetColorU32(it_anim->second.grab_col)); // grab


		GetWindowDrawList()->AddText(total_bb.Min, GetColorU32(it_anim->second.text_color), label);

		PushFont(font::regular_s);
		if (data_type == ImGuiDataType_S32)
			GetWindowDrawList()->AddText(value_bb.GetCenter() - CalcTextSize(std::to_string((int)it_anim->second.current_value).c_str()) / 2, GetColorU32(it_anim->second.value_text_col), std::to_string((int)it_anim->second.current_value).c_str());
		else
			GetWindowDrawList()->AddText(value_bb.GetCenter() - CalcTextSize(std::to_string((float)it_anim->second.current_value).c_str()) / 2, GetColorU32(it_anim->second.value_text_col), std::to_string((float)it_anim->second.current_value).c_str());
		PopFont();

		return value_changed;
	}

	bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
	}

	bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
	}

	bool ImConfig(const char* label, const char* date, const char* description, const char* creator)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const float square_sz = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;

		const float w = GetContentRegionAvail().x;

		const ImRect total_bb(pos, pos + ImVec2(w, 103));

		ItemSize(total_bb, style.FramePadding.y);
		ItemAdd(total_bb, id);

		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

		RenderNavHighlight(total_bb, id);

		const char* label_end = FindRenderedTextEnd(label);

		ImRect execute_bb(total_bb.Max - ImVec2(45, 51), total_bb.Max - ImVec2(45, 51) + ImVec2(35, 35));
		ImRect delete_bb(total_bb.Max - ImVec2(85, 51), total_bb.Max - ImVec2(85, 51) + ImVec2(35, 35));

		window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, utils::GetColorWithAlpha(c::window_bg_color, style.Alpha), c::elements::rounding);

		window->DrawList->AddRectFilled(total_bb.Min, ImVec2(total_bb.Max.x, total_bb.Min.y + 35.f), utils::GetColorWithAlpha(c::anim::default, style.Alpha), c::elements::rounding, ImDrawFlags_RoundCornersTop);

		window->DrawList->AddRect(total_bb.Min, total_bb.Max, stroke_color, c::elements::rounding);

		window->DrawList->AddText(ImVec2(total_bb.Min.x + 10.f, utils::center_text(total_bb.Min, ImVec2(total_bb.Max.x, total_bb.Min.y + 35.f), label).y), utils::GetColorWithAlpha(c::text::label::default, style.Alpha), label, label_end);

		window->DrawList->AddText(ImVec2(total_bb.Min.x + 10.f, total_bb.Min.y + 45.f), utils::GetColorWithAlpha(c::text::label::active, style.Alpha), "Creation date: ");

		window->DrawList->AddText(ImVec2(total_bb.Min.x + 10.f + CalcTextSize("Creation date: ").x, total_bb.Min.y + 45.f), utils::GetColorWithAlpha(c::anim::active, style.Alpha), date);

		window->DrawList->AddText(ImVec2(total_bb.Min.x + 10.f, total_bb.Min.y + 75.f), utils::GetColorWithAlpha(c::text::label::default, style.Alpha), description);

		window->DrawList->AddText(ImVec2(total_bb.Max.x - 10.f - CalcTextSize("Creator: ").x - CalcTextSize(creator).x, utils::center_text(total_bb.Min, ImVec2(total_bb.Max.x, total_bb.Min.y + 35.f), "Creator: ").y), utils::GetColorWithAlpha(c::text::label::active, style.Alpha), "Creator: ");
		window->DrawList->AddText(ImVec2(total_bb.Max.x - 10.f - CalcTextSize(creator).x, utils::center_text(total_bb.Min, ImVec2(total_bb.Max.x, total_bb.Min.y + 35.f), creator).y), utils::GetColorWithAlpha(c::text::label::active, style.Alpha), creator);

		window->DrawList->AddRectFilled(execute_bb.Min, execute_bb.Max, utils::GetColorWithAlpha(c::anim::active, style.Alpha), c::elements::rounding);

		window->DrawList->AddRect(execute_bb.Min, execute_bb.Max, utils::GetColorWithAlpha(c::stroke_color, style.Alpha / 20), c::elements::rounding, 0, 1.5f);

		window->DrawList->AddText(utils::center_text(execute_bb.Min, execute_bb.Max, ICON_PLAY_FILL), utils::GetColorWithAlpha(c::window_bg_color, style.Alpha), ICON_PLAY_FILL);

		bool hovered_del = IsMouseHoveringRect(delete_bb.Min, delete_bb.Max);

		window->DrawList->AddText(utils::center_text(delete_bb.Min, delete_bb.Max, ICON_DELETE_2_LINE), hovered_del ? ImColor(1.f, 0.5f, 0.5f, 1.f) : utils::GetColorWithAlpha(c::text::label::default, style.Alpha), ICON_DELETE_2_LINE);



		return pressed;
	}

}

