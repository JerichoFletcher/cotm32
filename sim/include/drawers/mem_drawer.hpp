#pragma once

#include "verilated_container.hpp"
#include "imgui_layer.hpp"

class MemDrawer : public ImGuiDrawer {
  public:
    MemDrawer(const VerilatedContainer& v);
    void draw() override;
  
  private:
    static constexpr int DISPLAY_WINDOW_W = 8;
    static constexpr int DISPLAY_WINDOW_H = 32;

    const VerilatedContainer& m_v;

    int m_mem_sec_curr;
    int m_mem_offset;
    int m_mem_disp_w;
};
