#pragma once

#include "drawers/clint_drawer.hpp"
#include "drawers/csr_drawer.hpp"
#include "drawers/fwd_drawer.hpp"
#include "drawers/mem_drawer.hpp"
#include "drawers/pipeline_regs_drawer.hpp"
#include "drawers/reg_drawer.hpp"
#include "drawers/status_drawer.hpp"
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
    StatusDrawer m_status_drawer;
    TrapDrawer m_trap_drawer;
    PipelineRegsDrawer m_pipeline_regs_drawer;
    FwdDrawer m_fwd_drawer;
    ClintDrawer m_clint_drawer;
};
