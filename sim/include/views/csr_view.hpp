#pragma once

#include "cotm32_defs.hpp"
#include "verilated_container.hpp"

class CsrView {
public:
    CsrView(const VerilatedContainer& v);

    IData& operator[](CsrId index);
    const IData& operator[](CsrId index) const;

    inline bool write_enable() const {
        return this->m_v.top()->cotm32->core->mem_csr_we &&
               this->m_v.top()->cotm32->core->exmem_valid;
    }

private:
    const VerilatedContainer& m_v;
};
