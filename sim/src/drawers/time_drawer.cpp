#include "drawers/time_drawer.hpp"

TimeDrawer::TimeDrawer(VerilatedContainer& v): m_v(v) {}

void TimeDrawer::draw() {
  ImGui::Begin("Time", nullptr,
    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
  );

  ImGui::Text("Time : %d", this->m_v.time());
  ImGui::Text("PC   : 0x%08x", this->m_v.pc());

  ImGui::Separator();
  if (ImGui::Button("Advance")) {
    this->m_v.tick();
  }
  if (ImGui::Button("Reset")) {
    this->m_v.reset();
  }

  ImGui::End();
}
