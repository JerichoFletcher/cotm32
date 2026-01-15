#pragma once

#include "simulator.hpp"

struct ClintDrawer : public SimulatorRenderListener {
    void render(const Simulator& sim) override;
};
