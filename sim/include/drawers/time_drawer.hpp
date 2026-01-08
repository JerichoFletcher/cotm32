#pragma once

#include <chrono>

#include "imgui_layer.hpp"
#include "verilated_container.hpp"

class TimeDrawer : public ImGuiDrawer {
  public:
    TimeDrawer(VerilatedContainer& v);
    void draw() override;
  private:
    static constexpr int MAX_STEPS_PER_FRAME = 300'000;
    using clock = std::chrono::steady_clock;
  
    VerilatedContainer& m_v;
    
    bool m_auto;
    bool m_prev_auto;
    float m_clk_hz;
    double m_tick_period;

    bool m_step_req;
    bool m_rst_req;

    clock::time_point m_prev_time;
    double m_accumulator;

    void update();
};
