#include "views/pipeline_regs_view.hpp"

PipelineRegsView::PipelineRegsView(const VerilatedContainer& v) : m_v(v) {}

const IfIdRegView PipelineRegsView::if_id() const {
    IfIdRegView view;
    view.stall = this->m_v.top()->cotm32->core->ifid_stall;
    view.flush = this->m_v.top()->cotm32->core->ifid_flush;
    view.valid = this->m_v.top()->cotm32->core->ifid_valid;
    view.pc = this->m_v.top()->cotm32->core->id_pc;
    view.pc_4 = this->m_v.top()->cotm32->core->id_pc_4;
    view.inst = this->m_v.top()->cotm32->core->id_inst;

    return view;
}
