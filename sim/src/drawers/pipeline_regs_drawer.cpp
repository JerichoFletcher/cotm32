#include "drawers/pipeline_regs_drawer.hpp"

#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "views/pipeline_regs_view.hpp"

void PipelineRegsDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "pipeline_regs",
            ImVec2(ImGui::GetContentRegionAvail().x, 0),
            ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Pipeline Registers", ImGuiTreeNodeFlags_DefaultOpen)) {
            PipelineRegsView view(sim.v());

            ImGui::SeparatorText("IF/ID");
            auto ifid_view = view.if_id();
            draw_signal(
                "Valid",
                ifid_view.valid,
                IM_COL32(40, 255, 40, 255),
                IM_COL32(255, 40, 40, 255),
                false
            );
            ImGui::SameLine();
            draw_signal(
                "Stall",
                ifid_view.stall,
                IM_COL32(255, 255, 40, 255),
                IM_COL32(40, 40, 40, 255),
                false
            );
            ImGui::SameLine();
            draw_signal(
                "Flush",
                ifid_view.flush,
                IM_COL32(255, 40, 40, 255),
                IM_COL32(40, 40, 40, 255),
                false
            );

            ImGui::Text("PC          : 0x%08x", ifid_view.pc);
            ImGui::Text("PC+4        : 0x%08x", ifid_view.pc_4);
            ImGui::Text("Instruction : 0x%08x", ifid_view.inst);
        }
    }
    ImGui::EndChild();
}
