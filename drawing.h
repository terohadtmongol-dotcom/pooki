namespace drawing {
    void DrawFilledRect(int x, int y, int w, int h, ImU32 color)
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
    }
    void DrawBox(ImDrawList* draw_list, const ImVec2 a, const ImVec2 b, ImColor color) {
        draw_list->AddRect(a, b, color, 0.0f, ImDrawFlags_RoundCornersAll, 1.f);

    }

    void DrawFilledBox(ImDrawList* draw_list, const ImVec2& a, const ImVec2& b, ImU32 color, float thickness) {
        draw_list->AddRectFilled(a, b, ImColor(0, 0, 0, (int)(0.6f * 255)));
        draw_list->AddRect(a, b, color, 0.0f, ImDrawFlags_RoundCornersAll, thickness);
    }

    void DrawCornerBox(ImDrawList* draw_list, ImVec2 topLeft, ImVec2 bottomRight, ImU32 color, float thickness) {
        float width = bottomRight.x - topLeft.x;
        float height = bottomRight.y - topLeft.y;

        float lineLength = width * 0.25f;
        float lineHeight = height * 0.25f;

        draw_list->AddLine(topLeft, ImVec2(topLeft.x + lineLength, topLeft.y), color, thickness);
        draw_list->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + lineHeight), color, thickness);

        draw_list->AddLine(ImVec2(bottomRight.x - lineLength, topLeft.y), ImVec2(bottomRight.x, topLeft.y), color, thickness);
        draw_list->AddLine(ImVec2(bottomRight.x, topLeft.y), ImVec2(bottomRight.x, topLeft.y + lineHeight), color, thickness);

        draw_list->AddLine(ImVec2(topLeft.x, bottomRight.y - lineHeight), ImVec2(topLeft.x, bottomRight.y), color, thickness);
        draw_list->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(topLeft.x + lineLength, bottomRight.y), color, thickness);

        draw_list->AddLine(ImVec2(bottomRight.x - lineLength, bottomRight.y), ImVec2(bottomRight.x, bottomRight.y), color, thickness);
        draw_list->AddLine(ImVec2(bottomRight.x, bottomRight.y - lineHeight), ImVec2(bottomRight.x, bottomRight.y), color, thickness);
    }

    void draw_text(ImFont* font, float fontSize, ImVec2 position, ImColor color, const char* text)
    {
        //ImGui::GetBackgroundDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y - 1), ImColor(0, 0, 0), text);
        //ImGui::GetBackgroundDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y - 1), ImColor(0, 0, 0), text);
        //ImGui::GetBackgroundDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y + 1), ImColor(0, 0, 0), text);
        //ImGui::GetBackgroundDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0), text);
        ImGui::GetBackgroundDrawList()->AddText(font, fontSize, position, color, text);
    }
    inline void DrawFilledBox(int x, int y, int w, int h, const ImU32& color, int borderPx, bool outlined) {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color);

        if (borderPx > 0) {
            DrawFilledRect(x + borderPx, y, w - 2 * borderPx, borderPx, ImColor(10, 10, 10, 180));
        }

        if (outlined) {
            ImU32 outlineColor = ImColor(1, 1, 1, 255);
            ImGui::GetForegroundDrawList()->AddRect(
                ImVec2(x, y),
                ImVec2(x + w, y + h),
                outlineColor,
                0.0f,
                0,
                1.0f
            );
        }
    }

    void draw_regular_box(int x, int y, int w, int h, ImColor ESPColor)
    {
        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ESPColor, 0, 2, 1.f);
        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ESPColor, 0, 0.50f, 1.f);
    }

}
