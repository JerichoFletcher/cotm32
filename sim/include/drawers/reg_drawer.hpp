#pragma once

#include "imgui_layer.hpp"
#include "verilated_container.hpp"
#include "views/reg_view.hpp"

class RegDrawer : public ImGuiDrawer {
  public:
    RegDrawer(const VerilatedContainer& v);
    void draw() override;
  private:
    const VerilatedContainer& m_v;
    const RegView m_reg;
};
