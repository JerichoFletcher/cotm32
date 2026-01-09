#pragma once

#include "simulator.hpp"
#include "controllers/time_controller.hpp"

class TimeDrawer : public SimulatorRenderListener {
  public:
    TimeDrawer(TimeController& m_ctrl);
    void render(const Simulator& sim) override;

  private:
    TimeController& m_ctrl;
};
