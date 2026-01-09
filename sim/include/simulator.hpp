#pragma once

#include <vector>

#include "sdl_window.hpp"
#include "imgui_renderer.hpp"
#include "verilated_container.hpp"

class Simulator;

struct SimulatorUpdateListener {
  virtual void update(Simulator& sim) = 0;
  virtual ~SimulatorUpdateListener() = default;
};

struct SimulatorRenderListener {
  virtual void render(const Simulator& sim) = 0;
  virtual ~SimulatorRenderListener() = default;
};

class Simulator : public SdlWindowUpdateListener, public ImGuiDrawer {
  public:
    Simulator(VerilatedContainer& v);

    inline VerilatedContainer& v() { return this->m_v; }
    inline const VerilatedContainer& v() const { return this->m_v; }
  
    void add_update_listener(SimulatorUpdateListener* listener);
    void add_render_listener(SimulatorRenderListener* listener);

    void update() override;
    void draw() override;

  private:
    VerilatedContainer& m_v;
    std::vector<SimulatorUpdateListener*> m_l_update;
    std::vector<SimulatorRenderListener*> m_l_render;
};
