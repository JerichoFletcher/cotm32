#pragma once

#include "verilated_container.hpp"
#include "views/csr_view.hpp"
#include "views/reg_view.hpp"

class TrapView {
public:
    TrapView(const VerilatedContainer& v);

    inline IData cause() const { return this->m_csr[CsrId::MCAUSE]; }
    inline IData tvalue() const { return this->m_csr[CsrId::MTVAL]; }
    inline IData reg_a7() const { return this->m_reg[REG_A7_IDX]; }

    inline bool in_trap() const { return this->m_v.top()->cotm32->core->trap_mode; }
    inline bool requested() const { return this->m_v.top()->cotm32->core->trap_req; }
    inline bool mret() const { return this->m_v.top()->cotm32->core->trap_mret; }

private:
    static constexpr inline int REG_A7_IDX = 17;

    const VerilatedContainer& m_v;
    const RegView m_reg;
    const CsrView m_csr;
};
