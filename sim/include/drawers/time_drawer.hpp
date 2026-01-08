#pragma once

#include "imgui_layer.hpp"
#include "verilated_container.hpp"

class TimeDrawer : public ImGuiDrawer {
  public:
    TimeDrawer(VerilatedContainer& v);
    void draw() override;
  private:
    VerilatedContainer& m_v;
};
