#include "drawers/time_drawer.hpp"

TimeDrawer::TimeDrawer(TimeController& ctrl): m_ctrl(ctrl) {}

void TimeDrawer::render(const Simulator& sim) {
  ImGui::SetNextWindowPos(
    ImVec2(355, 5),
    ImGuiCond_Once,
    ImVec2(0, 0)
  );
  if (ImGui::Begin("Time", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
  )) {
    ImGui::Text("Time  : %ld", sim.v().time());
    ImGui::Text("T-Scl : %s/%s", sim.v().context()->timeunitString(), sim.v().context()->timeprecisionString());
    ImGui::Text("PC    : 0x%08x", sim.v().pc());
  
    ImGui::Separator();
    ImGui::SliderFloat(
      "Clock (Hz)",
      &this->m_ctrl.clk_hz(),
      1.0f,
      50'000'000.0f,
      "%.0f",
      ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_AlwaysClamp
    );
    ImGui::Text("P    : %.9f s", this->m_ctrl.clk_period());
  
    ImGui::Separator();
    ImGui::ProgressBar(this->m_ctrl.tick_cap_frac(), ImVec2(-FLT_MIN, 0), "Tick cap");
    ImGui::ProgressBar(this->m_ctrl.tick_debt_frac(), ImVec2(-FLT_MIN, 0), "Tick debt");

    ImGui::Checkbox("Auto", &this->m_ctrl.is_auto());
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
      this->m_ctrl.is_auto() = false;
      this->m_ctrl.request_step();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
      this->m_ctrl.is_auto() = false;
      this->m_ctrl.request_reset();
    }
  }

  ImGui::End();
}
