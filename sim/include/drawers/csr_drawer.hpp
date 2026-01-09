#pragma once

#include "simulator.hpp"

struct CsrDrawer : public SimulatorRenderListener {
    void render(const Simulator& sim) override;
};
