#pragma once

#include "drawers/csr_drawer.hpp"
#include "drawers/mem_drawer.hpp"
#include "drawers/reg_drawer.hpp"
#include "drawers/trap_drawer.hpp"
#include "simulator.hpp"

class Sidebar : public SimulatorRenderListener {
public:
    Sidebar();
    void render(const Simulator& sim) override;

private:
    RegDrawer m_reg_drawer;
    CsrDrawer m_csr_drawer;
    MemDrawer m_mem_drawer;
    TrapDrawer m_trap_drawer;
};
