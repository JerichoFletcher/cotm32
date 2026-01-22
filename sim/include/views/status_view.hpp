#pragma once

#include "Vtop_csr_file.h"
#include "cotm32_defs.hpp"
#include "verilated_container.hpp"

class StatusView {
public:
    StatusView(const VerilatedContainer& v);

    inline IData mstatus() const {
        return this->m_v.top()->cotm32->core->csr->mem.at(CsrId_MSTATUS);
    }
    inline PrivMode privilege() const {
        return static_cast<PrivMode>(this->m_v.top()->cotm32->core->priv_mode);
    }
    inline bool m_interr_enable() const { return (this->mstatus() >> 3) & 1; }

private:
    const VerilatedContainer& m_v;
};
