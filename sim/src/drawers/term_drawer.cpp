#include "drawers/term_drawer.hpp"

#include "imgui.h"

TerminalDrawer::TerminalDrawer(TerminalController& c_term) : m_ctrl(c_term) {
    this->m_buf[0] = '\0';
}

void TerminalDrawer::render(const Simulator& sim) {
    ImGui::SetNextWindowPos(ImVec2(355, 331), ImGuiCond_Once, ImVec2(0, 0));
    if (ImGui::Begin(
            "Terminal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
        )) {
        auto text_size = ImGui::CalcTextSize(" ");

        auto w = this->m_ctrl.buf_width();
        auto h = this->m_ctrl.buf_height();
        auto x = this->m_ctrl.cursor_col();
        auto y = this->m_ctrl.cursor_row();

        ImGui::Text("Buffer size : %ldx%ld", w, h);
        ImGui::Separator();

        auto* draw_list = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetCursorScreenPos();

        auto cursor_screen_pos = ImVec2(
            pos.x + x * text_size.x,
            pos.y + ImGui::GetTextLineHeight() + y * ImGui::GetTextLineHeightWithSpacing()
        );
        draw_list->AddLine(
            cursor_screen_pos,
            ImVec2(cursor_screen_pos.x + text_size.x, cursor_screen_pos.y),
            IM_COL32(127, 127, 127, 255)
        );

        for (size_t y = 0; y < h; y++) {
            auto row = this->m_ctrl.buf().substr(y * w, w);
            ImGui::TextUnformatted(row.c_str());
        }

        ImGui::Separator();
        ImGui::PushItemWidth(w * text_size.x);
        if (ImGui::InputText(
                "##Input",
                this->m_buf,
                BUF_SIZE,
                ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_ElideLeft
            )) {
            this->send_input();
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Send")) {
            this->send_input();
        }
    }

    ImGui::End();
}

void TerminalDrawer::send_input() {
    for (char* c = this->m_buf; *c; c++) {
        this->m_ctrl.enqueue_char(*c);
    }
    this->m_ctrl.enqueue_char('\n');
    this->m_buf[0] = '\0';
}
