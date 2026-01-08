#include "drawers/sidebar.hpp"

Sidebar::Sidebar(const VerilatedContainer& v):
  m_v(v),
  m_reg_drawer(v),
  m_csr_drawer(v),
  m_mem_drawer(v),
  m_trap_drawer(v) {}

void Sidebar::draw() {
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
        this->m_reg_drawer.draw();
        this->m_csr_drawer.draw();
        this->m_mem_drawer.draw();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Privilege")) {
        this->m_trap_drawer.draw();
        this->m_csr_drawer.draw();
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}
