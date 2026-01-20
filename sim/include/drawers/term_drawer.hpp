#pragma once

#include "controllers/term_controller.hpp"
#include "simulator.hpp"

class TerminalDrawer : public SimulatorRenderListener {
public:
    TerminalDrawer(TerminalController& c_term);
    void render(const Simulator& sim) override;

private:
    static constexpr inline size_t BUF_SIZE = 256;

    TerminalController& m_ctrl;
    char m_buf[BUF_SIZE];

    void send_input();
};
