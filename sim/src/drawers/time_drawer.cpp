#include "drawers/time_drawer.hpp"

TimeDrawer::TimeDrawer(VerilatedContainer& v):
  m_v(v),
  m_auto(false),
  m_prev_auto(false),
  m_clk_hz(50.0f),
  m_tick_period(0.1),
  m_step_req(false),
  m_rst_req(false) {}

void TimeDrawer::draw() {
  ImGui::SetNextWindowPos(
    ImVec2(355, 5),
    ImGuiCond_Once,
    ImVec2(0, 0)
  );
  if (ImGui::Begin("Time", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
  )) {
    ImGui::Text("Time  : %ld", this->m_v.time());
    ImGui::Text("T-Scl : %s/%s", this->m_v.context()->timeunitString(), this->m_v.context()->timeprecisionString());
    ImGui::Text("PC    : 0x%08x", this->m_v.pc());
  
    ImGui::Separator();
    ImGui::SliderFloat(
      "Clock (Hz)",
      &this->m_clk_hz,
      1.0f,
      50'000'000.0f,
      "%.0f",
      ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_AlwaysClamp
    );
    this->m_tick_period = 1.0 / (double)this->m_clk_hz;
    ImGui::Text("Tick : %.9f s", this->m_tick_period);
  
    ImGui::Separator();
    ImGui::ProgressBar(this->tick_cap_frac(), ImVec2(-FLT_MIN, 0), "Tick cap");
    ImGui::ProgressBar(this->tick_debt_frac(), ImVec2(-FLT_MIN, 0), "Tick debt");

    ImGui::Checkbox("Auto", &this->m_auto);
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
      this->m_auto = false;
      this->m_step_req = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
      this->m_auto = false;
      this->m_rst_req = true;
    }
  }

  ImGui::End();
  this->update();
}

void TimeDrawer::update() {
  if (this->m_rst_req) {
    this->m_v.reset();
    this->m_rst_req = false;
    this->m_accumulator = 0.0;
    this->m_executed_step_count = 0;
    return;
  }

  if (!this->m_auto && (!this->m_step_req || this->m_prev_auto)) {
    this->m_prev_auto = this->m_auto;
    this->m_accumulator = 0.0;
    this->m_executed_step_count = 0;
    return;
  }

  if (this->m_auto && !this->m_prev_auto) {
    this->m_prev_time = clock::now();
    this->m_prev_auto = this->m_auto;
  }

  auto now = clock::now();
  std::chrono::duration<double> delta = now - this->m_prev_time;
  this->m_prev_time = now;

  if (!this->m_auto && this->m_step_req) {
    this->m_v.tick();
    this->m_step_req = false;
    this->m_accumulator = 0.0;
    return;
  }

  this->m_accumulator += delta.count();
  this->m_executed_step_count = 0;
  while (
    this->m_accumulator >= this->m_tick_period
    && this->m_executed_step_count < TimeDrawer::MAX_STEPS_PER_FRAME
  ) {
    this->m_v.tick();
    this->m_accumulator -= this->m_tick_period;
    this->m_executed_step_count++;
  }
}
