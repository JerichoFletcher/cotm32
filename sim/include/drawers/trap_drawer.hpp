#pragma once

#include "simulator.hpp"

struct TrapDrawer : public SimulatorRenderListener {
  void render(const Simulator& sim) override;
};
