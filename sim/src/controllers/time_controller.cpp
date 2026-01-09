#include "controllers/time_controller.hpp"

TimeController::TimeController()
    : m_is_auto(false),
      m_is_prev_auto(false),
      m_clk_hz(50.0f),
      m_step_req(false),
      m_rst_req(false),
      m_accumulator(0.0f),
      m_exec_step(0) {}

void TimeController::update(Simulator& sim) {
    if (this->m_rst_req) {
        sim.v().reset();
        this->m_rst_req = false;
        this->m_accumulator = 0.0;
        this->m_exec_step = 0;
        return;
    }

    if (!this->m_is_auto && (!this->m_step_req || this->m_is_prev_auto)) {
        this->m_is_prev_auto = this->m_is_auto;
        this->m_accumulator = 0.0;
        this->m_exec_step = 0;
        return;
    }

    if (this->m_is_auto && !this->m_is_prev_auto) {
        this->m_prev_time = clock::now();
        this->m_is_prev_auto = this->m_is_auto;
    }

    auto now = clock::now();
    std::chrono::duration<double> delta = now - this->m_prev_time;
    this->m_prev_time = now;

    if (!this->m_is_auto && this->m_step_req) {
        sim.v().update();
        this->m_step_req = false;
        this->m_accumulator = 0.0;
        return;
    }

    this->m_accumulator += delta.count();
    this->m_exec_step = 0;
    while (this->m_accumulator >= this->clk_period() && this->m_exec_step < MAX_STEPS_PER_FRAME) {
        sim.v().update();
        this->m_accumulator -= this->clk_period();
        this->m_exec_step++;
    }
}
