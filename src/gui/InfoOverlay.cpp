#include "InfoOverlay.h"
#include <imgui/imgui.h>
#include <ostream>

void GUI::Debug::showOverlay(bool* open) {
    showOverlay(open, nullptr);
}

void GUI::Debug::showOverlay(bool* open, std::vector<Printable*>* props) {
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;

    if (location >= 0) {
        const float PAD = 5.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        //ImVec2 window_size = ImGui::GetWindowSize();
        //window_size.y = 100;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        //ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

    if (ImGui::Begin("Debug info overlay", open, window_flags)) {
        ImGui::Text("Debug info");
        ImGui::Separator();

        if (ImGui::IsMousePosValid()) {
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        }
        else {
            ImGui::Text("Mouse Position: <invalid>");
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        for (auto p : *props) {
            ImGui::Text("%s", p->toString().c_str());
        }

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Custom", NULL, location == -1)) { location = -1; }
            if (ImGui::MenuItem("Top-left", NULL, location == 0)) { location = 0; }
            if (ImGui::MenuItem("Top-right", NULL, location == 1)) { location = 1; }
            if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) { location = 2; }
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) { location = 3; }
            if (open && ImGui::MenuItem("Close")) { *open = false; }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}