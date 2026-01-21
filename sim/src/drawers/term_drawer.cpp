#include "drawers/term_drawer.hpp"

#include <fmt/core.h>

#include "colors.hpp"
#include "drawers/draw_utils.hpp"
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
        ImGui::Text("Cursor pos  : row %ld, col %ld", y + 1, x + 1);
        ImGui::Separator();

        auto* draw_list = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetCursorScreenPos();

        for (size_t row = 0; row < h; row++) {
            for (size_t col = 0; col < w; col++) {
                auto curr_pos = ImVec2(
                    pos.x + col * text_size.x, pos.y + row * ImGui::GetTextLineHeightWithSpacing()
                );
                auto curr_pos_max = ImVec2(
                    curr_pos.x + text_size.x, curr_pos.y + ImGui::GetTextLineHeightWithSpacing()
                );

                Symbol sym;
                ImU32 prev_col;
                if (this->m_ctrl.symbol_at_viewport(row, col, &sym)) {
                    auto text = fmt::format("{:c}", sym.glyph);
                    draw_list->AddRectFilled(curr_pos, curr_pos_max, sym.bg_color);
                    draw_list->AddText(curr_pos, sym.fg_color, text.c_str());
                    prev_col = sym.bg_color;
                } else {
                    draw_list->AddRectFilled(curr_pos, curr_pos_max, prev_col);
                }
            }
        }

        ImGui::Dummy(ImVec2(w * text_size.x, h * ImGui::GetTextLineHeightWithSpacing()));

        ImGui::Separator();
        cotm32::draw_utils::draw_signal(
            "Type to input",
            ImGui::IsWindowFocused(),
            cotm32::colors::GREEN,
            cotm32::colors::OFF,
            false
        );
        ImGui::Text("Input queue : %ld characters", this->m_ctrl.input_queue_size());

        if (ImGui::IsWindowFocused()) {
            auto& io = ImGui::GetIO();

            int scroll = -(int)io.MouseWheel;
            if (scroll != 0) {
                this->m_ctrl.scroll(scroll);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                this->m_ctrl.enqueue_char('\n');
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Tab)) {
                this->m_ctrl.enqueue_char('\t');
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
                this->m_ctrl.enqueue_char('\b');
            }

            for (int i = 0; i < io.InputQueueCharacters.Size; i++) {
                auto c = io.InputQueueCharacters[i];
                if (0 < c && c < 0x10000) {
                    this->m_ctrl.enqueue_char((char)c);
                }
            }
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
