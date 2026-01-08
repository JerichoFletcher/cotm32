#pragma once

#include "imgui_layer.hpp"
#include "verilated_container.hpp"
#include "views/reg_view.hpp"
#include "views/csr_view.hpp"

class TrapDrawer : public ImGuiDrawer {
  public:
    TrapDrawer(const VerilatedContainer& v);
    void draw() override;

  private:
    const VerilatedContainer& m_v;
    const RegView m_reg;
    const CsrView m_csr;

    void draw_signal(const char* label, bool on, ImU32 on_color, ImU32 off_color);
};
