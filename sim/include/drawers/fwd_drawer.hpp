#pragma once

#include "simulator.hpp"

struct FwdDrawer : public SimulatorRenderListener {
    void render(const Simulator& sim) override;
};
