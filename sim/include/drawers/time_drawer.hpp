#pragma once

#include "controllers/time_controller.hpp"
#include "simulator.hpp"

class TimeDrawer : public SimulatorRenderListener {
public:
    TimeDrawer(TimeController& c_time);
    void render(const Simulator& sim) override;

private:
    TimeController& m_ctrl;
};
