#pragma once

#include "controllers/term_controller.hpp"
#include "simulator.hpp"

class TerminalDrawer : public SimulatorRenderListener {
public:
    TerminalDrawer(TerminalController& c_term);
    void render(const Simulator& sim) override;

private:
    TerminalController& m_ctrl;
};
