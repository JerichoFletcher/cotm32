#include "drawers/dump_drawer.hpp"

#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"

DumpDrawer::DumpDrawer(DumpController& c_dump)
    : m_c_dump(c_dump),
      m_length(0),
      m_dump_name("trace.vcd") {}

void DumpDrawer::render(const Simulator& sim) {
    ImGui::SetNextWindowPos(ImVec2(355, 209), ImGuiCond_Once, ImVec2(0, 0));
    if (ImGui::Begin(
            "Dump", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
        )) {
        if (ImGui::InputText(
                "Dump name",
                &this->m_dump_name,
                ImGuiInputTextFlags_CallbackCharFilter,
                [](ImGuiInputTextCallbackData* d) -> int {
                    if (d->EventFlag == ImGuiInputTextFlags_CallbackCharFilter) {
                        std::string& curr = *(std::string*)d->UserData;
                        if (curr.length() > DUMP_NAME_BUF_SIZE) return 1;
                    }
                    return 0;
                },
                (void*)&this->m_dump_name
            )) {
            if (this->m_dump_name.rfind(".vcd") != (this->m_dump_name.length() - 4)) {
                this->m_dump_name.append(".vcd");
            }
        }
        ImGui::SliderInt(
            "Length",
            &this->m_length,
            0,
            this->m_c_dump.MAX_DUMP_LENGTH,
            "%d",
            ImGuiSliderFlags_AlwaysClamp
        );

        if (this->m_c_dump.is_dumping()) {
            ImGui::Text(
                "Dumping %d/%d", sim.v().dump_processed_steps(), sim.v().dump_total_steps()
            );
        } else {
            ImGui::TextUnformatted("Not dumping");
        }

        if (ImGui::Button("Start dump")) {
            this->m_c_dump.request_dump(this->m_dump_name, this->m_length);
        }
        ImGui::SameLine();
        draw_signal(
            "Dumping",
            this->m_c_dump.is_dumping(),
            IM_COL32(255, 255, 40, 255),
            IM_COL32(40, 40, 40, 255),
            true
        );
    }

    ImGui::End();
}
