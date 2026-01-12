#pragma once

#include "cotm32_defs.hpp"
#include "verilated_container.hpp"

class FwdView {
public:
    FwdView(const VerilatedContainer& v);

    inline uint8_t ex_rs1_addr() const { return this->m_v.top()->cotm32->core->ex_rs1_addr; }
    inline uint8_t ex_rs2_addr() const { return this->m_v.top()->cotm32->core->ex_rs2_addr; }
    inline uint8_t mem_rd_addr() const { return this->m_v.top()->cotm32->core->mem_rd_addr; }
    inline uint8_t wb_rd_addr() const { return this->m_v.top()->cotm32->core->wb_rd_addr; }

    inline FwdSrc forward_a_src() const {
        return static_cast<FwdSrc>(this->m_v.top()->cotm32->core->forward_a);
    }
    inline FwdSrc forward_b_src() const {
        return static_cast<FwdSrc>(this->m_v.top()->cotm32->core->forward_b);
    }

    inline IData ex_rs1_fwd() const { return this->m_v.top()->cotm32->core->ex_rs1_fwd; }
    inline IData ex_rs2_fwd() const { return this->m_v.top()->cotm32->core->ex_rs2_fwd; }

private:
    const VerilatedContainer& m_v;
};
