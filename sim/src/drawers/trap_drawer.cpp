#include "drawers/trap_drawer.hpp"

#include <fmt/core.h>

static const int REG_A7_IDX = 17;
static const int MCAUSE_IDX = 2;
static const int MTVAL_IDX = 3;

static const int TRAP_CAUSE_INST_ADDR_MISALIGNED  = 0;
static const int TRAP_CAUSE_ILLEGAL_INST          = 2;
static const int TRAP_CAUSE_BREAKPOINT            = 3;
static const int TRAP_CAUSE_LOAD_ADDR_MISALIGNED  = 4;
static const int TRAP_CAUSE_STORE_ADDR_MISALIGNED = 6;
static const int TRAP_CAUSE_ECALL_M               = 11;

static const float SIG_RADIUS = 6.0f;

TrapDrawer::TrapDrawer(const VerilatedContainer& v):
  m_v(v),
  m_reg(v),
  m_csr(v) {}

void TrapDrawer::draw() {
  if (ImGui::BeginChild("trap",
    ImVec2(ImGui::GetContentRegionAvail().x, 0),
    ImGuiChildFlags_AutoResizeY
  )) {
    if (ImGui::CollapsingHeader("Trap View", ImGuiTreeNodeFlags_DefaultOpen)) {
      int trap_cause = this->m_csr[MCAUSE_IDX];
      int trap_val = this->m_csr[MTVAL_IDX];
      bool trap_mode = this->m_v.top()->cotm32->core->trap_mode;
      bool trap_req = this->m_v.top()->cotm32->core->trap_req;
      bool trap_mret = this->m_v.top()->cotm32->core->trap_mret;

      if (trap_req) {
        ImGui::TextUnformatted("Entering trap mode next tick");
        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + ImGui::GetTextLineHeight()));
      } else if (trap_mode) {
        if (trap_mret) {
          ImGui::Text("In trap: %d (exiting)", trap_cause);
        } else {
          ImGui::Text("In trap: %d", trap_cause);
        }

        const char* msg;
        const char* desc;

        switch (trap_cause) {
          case TRAP_CAUSE_INST_ADDR_MISALIGNED :
            msg = "Instruction address misaligned";
            desc = fmt::format("-> {:08x}", trap_val).c_str();
            break;
            case TRAP_CAUSE_ILLEGAL_INST:
            msg = "Illegal instruction";
            desc = fmt::format("-> {:08x}", trap_val).c_str();
            break;
          case TRAP_CAUSE_BREAKPOINT:
            msg = "Breakpoint";
            desc = nullptr;
            break;
          case TRAP_CAUSE_LOAD_ADDR_MISALIGNED:
            msg = "Load address misaligned";
            desc = fmt::format("-> {:08x}", trap_val).c_str();
            break;
            case TRAP_CAUSE_STORE_ADDR_MISALIGNED:
            msg = "Store address misaligned";
            desc = fmt::format("-> {:08x}", trap_val).c_str();
            break;
          case TRAP_CAUSE_ECALL_M:
            msg = "M-mode environment call";
            desc = fmt::format("a7 = 0x{:08x}", this->m_reg[REG_A7_IDX]).c_str();
            break;
          default:
            msg = "Unknown trap cause";
            desc = nullptr;
            break;
        }
        ImGui::Text("Cause  : %s", msg);
        ImGui::TextUnformatted(desc ? desc : "No description");
      } else {
        ImGui::TextUnformatted("Not in trap");
        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + ImGui::GetTextLineHeight()));
      }
      ImGui::Separator();

      this->draw_signal("Trap mode", trap_mode,
        IM_COL32(255, 40, 40, 255), IM_COL32(40, 40, 40, 255)
      );
      this->draw_signal("Trap request", trap_req,
        IM_COL32(255, 255, 40, 255), IM_COL32(40, 40, 40, 255)
      );
      this->draw_signal("Trap return", trap_mret,
        IM_COL32(40, 255, 40, 255), IM_COL32(40, 40, 40, 255)
      );
    }
  }
  ImGui::EndChild();
}

void TrapDrawer::draw_signal(const char* label, bool on, ImU32 on_color, ImU32 off_color) {
  auto* draw_list = ImGui::GetWindowDrawList();
  auto pos = ImGui::GetCursorScreenPos();
  auto col = on ? on_color : off_color;

  draw_list->AddCircleFilled(
    ImVec2(pos.x + SIG_RADIUS, pos.y + SIG_RADIUS),
    SIG_RADIUS, col
  );
  ImGui::Dummy(ImVec2(SIG_RADIUS * 2, SIG_RADIUS * 2));
  ImGui::SameLine();
  ImGui::TextUnformatted(label);
}
