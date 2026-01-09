#pragma once

#include <chrono>

#include "simulator.hpp"

class TimeController : public SimulatorUpdateListener {
  public:
    TimeController();
    void update(Simulator& sim) override;

    inline bool is_auto() const { return this->m_is_auto; }
    inline bool is_prev_auto() const { return this->m_is_prev_auto; }
    inline float clk_hz() const { return this->m_clk_hz; }
    inline double clk_period() const { return 1.0 / this->m_clk_hz; }

    inline float tick_cap_frac() const {
      return (float)this->m_executed_step_count / MAX_STEPS_PER_FRAME;
    }
    inline float tick_debt_frac() const {
      return this->m_accumulator / (this->clk_period() * MAX_STEPS_PER_FRAME);
    }

    inline bool& is_auto() { return this->m_is_auto; }
    inline bool& is_prev_auto() { return this->m_is_prev_auto; }
    inline float& clk_hz() { return this->m_clk_hz; }

    inline void request_step() { this->m_step_req = true; }
    inline void request_reset() { this->m_rst_req = true; }
  
  private:
    static constexpr int MAX_STEPS_PER_FRAME = 200'000;
    using clock = std::chrono::steady_clock;

    bool m_is_auto;
    bool m_is_prev_auto;
    float m_clk_hz;

    bool m_step_req;
    bool m_rst_req;

    clock::time_point m_prev_time;
    double m_accumulator;
    int m_executed_step_count;
};
