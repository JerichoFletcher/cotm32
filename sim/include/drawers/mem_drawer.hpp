#pragma once

#include "simulator.hpp"

class MemDrawer : public SimulatorRenderListener {
public:
    MemDrawer();
    void render(const Simulator& sim) override;

private:
    static constexpr int DISPLAY_WINDOW_W = 8;
    static constexpr int DISPLAY_WINDOW_H = 32;

    int m_mem_sec_curr;
    int m_mem_offset;
    int m_mem_disp_w;
};
