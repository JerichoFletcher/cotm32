#pragma once

#include "simulator.hpp"

struct StatusDrawer : public SimulatorRenderListener {
    void render(const Simulator& sim) override;
};
