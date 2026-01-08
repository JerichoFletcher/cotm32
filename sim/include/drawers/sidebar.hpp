#pragma once

#include "imgui_layer.hpp"
#include "verilated_container.hpp"

#include "drawers/reg_drawer.hpp"
#include "drawers/csr_drawer.hpp"

class Sidebar : public ImGuiDrawer {
  public:
    Sidebar(const VerilatedContainer& v);
    void draw() override;
  private:
    const VerilatedContainer& m_v;
    RegDrawer m_reg_drawer;
    CsrDrawer m_csr_drawer;
};
