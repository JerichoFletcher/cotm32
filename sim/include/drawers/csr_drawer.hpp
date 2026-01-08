#pragma once

#include "imgui_layer.hpp"
#include "verilated_container.hpp"
#include "views/csr_view.hpp"

class CsrDrawer : public ImGuiDrawer {
  public:
    CsrDrawer(const VerilatedContainer& v);
    void draw() override;
  private:
    const VerilatedContainer& m_v;
    const CsrView m_csr;
};
