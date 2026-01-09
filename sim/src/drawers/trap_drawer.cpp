#include "drawers/trap_drawer.hpp"

#include <fmt/core.h>

#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "views/trap_view.hpp"

static constexpr inline int TRAP_CAUSE_INST_ADDR_MISALIGNED = 0;
static constexpr inline int TRAP_CAUSE_INST_ACCESS_FAULT = 1;
static constexpr inline int TRAP_CAUSE_ILLEGAL_INST = 2;
static constexpr inline int TRAP_CAUSE_BREAKPOINT = 3;
static constexpr inline int TRAP_CAUSE_LOAD_ADDR_MISALIGNED = 4;
static constexpr inline int TRAP_CAUSE_LOAD_ACCESS_FAULT = 5;
static constexpr inline int TRAP_CAUSE_STORE_ADDR_MISALIGNED = 6;
static constexpr inline int TRAP_CAUSE_STORE_ACCESS_FAULT = 7;
static constexpr inline int TRAP_CAUSE_ECALL_M = 11;

void TrapDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "trap", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Trap View", ImGuiTreeNodeFlags_DefaultOpen)) {
            TrapView trap(sim.v());

            auto trap_cause = trap.cause();
            auto trap_val = trap.tvalue();
            bool trap_mode = trap.in_trap();
            bool trap_req = trap.requested();
            bool trap_mret = trap.mret();

            if (trap_req) {
                ImGui::TextUnformatted("Entering trap mode next tick");
                ImGui::Dummy(
                    ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + ImGui::GetTextLineHeight())
                );
            } else if (trap_mode) {
                if (trap_mret) {
                    ImGui::Text("In trap: %d (exiting)", trap_cause);
                } else {
                    ImGui::Text("In trap: %d", trap_cause);
                }

                const char* msg;
                const char* desc;

                switch (trap_cause) {
                    case TRAP_CAUSE_INST_ADDR_MISALIGNED:
                        msg = "Instruction address misaligned";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_INST_ACCESS_FAULT:
                        msg = "Instruction access fault";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_ILLEGAL_INST:
                        msg = "Illegal instruction";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_BREAKPOINT:
                        msg = "Breakpoint";
                        desc = nullptr;
                        break;
                    case TRAP_CAUSE_LOAD_ADDR_MISALIGNED:
                        msg = "Load address misaligned";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_LOAD_ACCESS_FAULT:
                        msg = "Load access fault";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_STORE_ADDR_MISALIGNED:
                        msg = "Store address misaligned";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_STORE_ACCESS_FAULT:
                        msg = "Store access fault";
                        desc = fmt::format("-> 0x{:08x}", trap_val).c_str();
                        break;
                    case TRAP_CAUSE_ECALL_M:
                        msg = "M-mode environment call";
                        desc = fmt::format("a7 = 0x{:08x}", trap.reg_a7()).c_str();
                        break;
                    default:
                        msg = "Unknown trap cause";
                        desc = nullptr;
                        break;
                }
                ImGui::Text("Cause  : %s", msg);
                ImGui::TextUnformatted(!!desc ? desc : "No description");
            } else {
                ImGui::TextUnformatted("Not in trap");
                ImGui::Dummy(
                    ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + ImGui::GetTextLineHeight())
                );
            }
            ImGui::Separator();

            draw_signal(
                "In trap mode",
                trap_mode,
                IM_COL32(255, 40, 40, 255),
                IM_COL32(40, 40, 40, 255),
                false
            );
            draw_signal(
                "Trap requested",
                trap_req,
                IM_COL32(255, 255, 40, 255),
                IM_COL32(40, 40, 40, 255),
                false
            );
            draw_signal(
                "Trap return",
                trap_mret,
                IM_COL32(40, 255, 40, 255),
                IM_COL32(40, 40, 40, 255),
                false
            );
        }
    }
    ImGui::EndChild();
}
