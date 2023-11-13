
#include <vector>
#include <string>

#include "imgui.h"
#include "imgui_internal.h"


class radialMenu
{
private:
	bool IsActive;
    ImVec2 center;
    std::vector<std::string> items;
    int items_count = 0;

public:
	radialMenu(std::initializer_list<std::string> menulist) : items(menulist), IsActive(false)
    {
        items_count = items.size();
    }
	~radialMenu() = default;

    void setActive() { IsActive = true; }
    bool checkActive() { return IsActive; }

    void setCenter(float x, float y) { center = { x,y }; }

	int DrawRadialMenu() {
        int ret = -1;
		if (ImGui::Begin("radialMenu", &IsActive, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar) )
		{
            // Determine Mouse Pos
            const ImVec2 drag_delta = ImVec2(ImGui::GetIO().MousePos.x - center.x, ImGui::GetIO().MousePos.y - center.y);
            const float drag_dist2 = drag_delta.x * drag_delta.x + drag_delta.y * drag_delta.y;
            // Draw Parameters
            const ImGuiStyle& style = ImGui::GetStyle();
            const float RADIUS_MIN = 30.0f;
            const float RADIUS_MAX = 120.0f;
            const float RADIUS_INTERACT_MIN = 20.0f;
            const int ITEMS_MIN = 6;
            // Drawing
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->PushClipRectFullScreen();
            draw_list->PathArcTo(center, (RADIUS_MIN + RADIUS_MAX) * 0.5f, 0.0f, IM_PI * 2.0f * 0.99f, 32);   // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
            draw_list->PathStroke(ImColor(0, 0, 0), true, RADIUS_MAX - RADIUS_MIN);

            const float item_arc_span = 2 * IM_PI / ImMax(ITEMS_MIN, items_count);
            float drag_angle = atan2f(drag_delta.y, drag_delta.x);
            if (drag_angle < -0.5f * item_arc_span)
                drag_angle += 2.0f * IM_PI;
            //ImGui::Text("%f", drag_angle);    // [Debug]

            int item_hovered = -1;
            for (int item_n = 0; item_n < items_count; item_n++)
            {
                const char* item_label = items[item_n].c_str();
                const float item_ang_min = item_arc_span * (item_n + 0.02f) - item_arc_span * 0.5f; // FIXME: Could calculate padding angle based on how many pixels they'll take
                const float item_ang_max = item_arc_span * (item_n + 0.98f) - item_arc_span * 0.5f;

                bool hovered = false;
                if (drag_dist2 >= RADIUS_INTERACT_MIN * RADIUS_INTERACT_MIN)
                {
                    if (drag_angle >= item_ang_min && drag_angle < item_ang_max)
                        hovered = true;
                }

                int arc_segments = (int)(32 * item_arc_span / (2 * IM_PI)) + 1;
                draw_list->PathArcTo(center, RADIUS_MAX - style.ItemInnerSpacing.x, item_ang_min, item_ang_max, arc_segments);
                draw_list->PathArcTo(center, RADIUS_MIN + style.ItemInnerSpacing.x, item_ang_max, item_ang_min, arc_segments);
                //draw_list->PathFill(window->Color(hovered ? ImGuiCol_HeaderHovered : ImGuiCol_FrameBg));
                draw_list->PathFillConvex(hovered ? ImColor(100, 100, 150) : ImColor(70, 70, 70));

                ImVec2 text_size = ImGui::CalcTextSize(item_label);
                ImVec2 text_pos = ImVec2(
                    center.x + cosf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.x * 0.5f,
                    center.y + sinf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.y * 0.5f);
                draw_list->AddText(text_pos, ImColor(255, 255, 255), item_label);

                if (hovered)
                    item_hovered = item_n;
            }
            draw_list->PopClipRect();

            if (ImGui::IsMouseReleased(0))
            {
                IsActive = false;
                ret = item_hovered;
            }
		}
		ImGui::End();
		return ret;
	}
};