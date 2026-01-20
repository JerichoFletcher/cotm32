#include "controllers/time_controller.hpp"

TimeController::TimeController()
    : m_is_auto(false),
      m_is_prev_auto(false),
      m_clk_hz(50.0f),
      m_step_req(false),
      m_rst_req(false),
      m_accumulator(0.0f),
      m_exec_step(0) {}

void TimeController::add_update_listener(PerTickUpdateListener* listener) {
    this->m_l_update.push_back(listener);
}

void TimeController::add_reset_listener(ResetListener* listener) {
    this->m_l_reset.push_back(listener);
}

void TimeController::update(Simulator& sim) {
    if (this->m_rst_req) {
        this->reset(sim);
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
        this->tick_update(sim);
        this->m_step_req = false;
        this->m_accumulator = 0.0;
        return;
    }

    this->m_accumulator += delta.count();
    this->m_exec_step = 0;
    while (this->m_accumulator >= this->clk_period() && this->m_exec_step < MAX_STEPS_PER_FRAME) {
        this->tick_update(sim);
        this->m_accumulator -= this->clk_period();
        this->m_exec_step++;
    }
}

void TimeController::tick_update(Simulator& sim) {
    sim.v().update();
    for (auto* l : this->m_l_update) {
        l->per_tick_update(sim);
    }
}

void TimeController::reset(Simulator& sim) {
    sim.v().reset();
    for (auto* l : this->m_l_reset) {
        l->reset(sim);
    }
}
