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
    ImVec2(280, 10),
    ImGuiCond_Once,
    ImVec2(0, 0)
  );
  if (ImGui::Begin("Time", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
  )) {
    ImGui::Text("Time : %d", this->m_v.time());
    ImGui::Text("PC   : 0x%08x", this->m_v.pc());
  
    ImGui::Separator();
    ImGui::SliderFloat(
      "Clock (Hz)",
      &this->m_clk_hz,
      1.0f,
      50'000'000.0f,
      "%.0f",
      ImGuiSliderFlags_Logarithmic
    );
    this->m_tick_period = 1.0 / (double)this->m_clk_hz;
    ImGui::Text("Tick : %.9f s", this->m_tick_period);
  
    ImGui::Separator();
    ImGui::Checkbox("Auto", &this->m_auto);
    ImGui::SameLine();
    if (ImGui::Button("Advance")) {
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
    return;
  }

  if (!this->m_auto && !this->m_step_req) {
    this->m_prev_auto = this->m_auto;
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
  int steps = 0;
  while (
    this->m_accumulator >= this->m_tick_period
    && steps < TimeDrawer::MAX_STEPS_PER_FRAME
  ) {
    this->m_v.tick();
    this->m_accumulator -= this->m_tick_period;
    steps++;
  }

  // Reset accumulator if max step count is reached
  if (steps >= TimeDrawer::MAX_STEPS_PER_FRAME) {
    this->m_accumulator = 0.0;
  }
}
