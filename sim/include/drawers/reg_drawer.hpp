#pragma once

#include "simulator.hpp"

struct RegDrawer : public SimulatorRenderListener {
    void render(const Simulator& sim) override;
};
