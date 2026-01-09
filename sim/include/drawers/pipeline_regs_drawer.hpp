#pragma once

#include "simulator.hpp"

struct PipelineRegsDrawer : public SimulatorRenderListener {
    void render(const Simulator& sim) override;
};
