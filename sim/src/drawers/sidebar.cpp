#include "drawers/sidebar.hpp"

Sidebar::Sidebar():
  m_reg_drawer(),
  m_csr_drawer(),
  m_mem_drawer(),
  m_trap_drawer() {}

void Sidebar::render(const Simulator& sim) {
  auto ws = ImGui::GetMainViewport()->WorkSize;

  ImGui::SetNextWindowPos(
    ImVec2(0, 0),
    ImGuiCond_Once,
    ImVec2(0, 0)
  );
  ImGui::SetNextWindowSize(ImVec2(350, ws.y));
  if (ImGui::Begin("Sidebar", nullptr,
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoTitleBar
  )) {
    if (ImGui::BeginTabBar("sidebar_tabs")) {
      if (ImGui::BeginTabItem("Memory")) {
        this->m_reg_drawer.render(sim);
        this->m_csr_drawer.render(sim);
        this->m_mem_drawer.render(sim);
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Privilege")) {
        this->m_trap_drawer.render(sim);
        this->m_csr_drawer.render(sim);
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}
