#include "drawers/term_drawer.hpp"

#include <fmt/core.h>

#include "colors.hpp"
#include "drawers/draw_utils.hpp"
#include "imgui.h"

TerminalDrawer::TerminalDrawer(TerminalController& c_term) : m_ctrl(c_term) {}

void TerminalDrawer::render(const Simulator& sim) {
    ImGui::SetNextWindowPos(ImVec2(355, 331), ImGuiCond_Once, ImVec2(0, 0));
    if (ImGui::Begin(
            "Terminal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
        )) {
        auto text_size = ImGui::CalcTextSize(" ");

        auto buf_n = this->m_ctrl.buffer_size();
        auto buf_c = this->m_ctrl.buffer_cap();

        auto vp_w = this->m_ctrl.viewport_width();
        auto vp_h = this->m_ctrl.viewport_height();
        auto vp_start = this->m_ctrl.viewport_start_row();

        auto c_row = this->m_ctrl.cursor_row();
        auto c_col = this->m_ctrl.cursor_col();

        ImGui::Text("Buffer size   : %ld/%ld lines", buf_n, buf_c);
        ImGui::Text("Viewport size : %ldx%ld", vp_w, vp_h);
        ImGui::Text("Viewport pos  : line %ld - %ld", vp_start + 1, vp_start + vp_h);
        ImGui::Text("Cursor pos    : row %ld, col %ld", c_row + 1, c_col + 1);
        ImGui::Separator();

        auto* draw_list = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetCursorScreenPos();

        for (size_t row = 0; row < vp_h; row++) {
            for (size_t col = 0; col < vp_w; col++) {
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

        ImGui::Dummy(ImVec2(vp_w * text_size.x, vp_h * ImGui::GetTextLineHeightWithSpacing()));

        ImGui::Separator();
        cotm32::draw_utils::draw_signal(
            "Type to input",
            ImGui::IsWindowFocused(),
            cotm32::colors::GREEN,
            cotm32::colors::OFF,
            false
        );
        ImGui::Text("Input queue   : %ld characters", this->m_ctrl.input_queue_size());

        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
            auto& io = ImGui::GetIO();

            int scroll = (int)io.MouseWheel;
            if (scroll != 0) {
                this->m_ctrl.scroll(scroll);
            }

            if (ImGui::IsWindowFocused()) {
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
    }

    ImGui::End();
}
