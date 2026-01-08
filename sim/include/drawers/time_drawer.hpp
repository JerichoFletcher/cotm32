#pragma once

#include <chrono>

#include "imgui_layer.hpp"
#include "verilated_container.hpp"

class TimeDrawer : public ImGuiDrawer {
  public:
    TimeDrawer(VerilatedContainer& v);
    void draw() override;
  private:
    static constexpr int MAX_STEPS_PER_FRAME = 200'000;
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
    int m_executed_step_count;

    void update();

    inline float tick_cap_frac() const {
      return (float)this->m_executed_step_count / MAX_STEPS_PER_FRAME;
    }
    inline float tick_debt_frac() const {
      return this->m_accumulator / (this->m_tick_period * MAX_STEPS_PER_FRAME);
    }
};
