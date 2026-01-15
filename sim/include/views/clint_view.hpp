#pragma once

#include "verilated_container.hpp"
#include "views/mem_view.hpp"

class ClintView {
public:
    ClintView(const VerilatedContainer& v);

    bool msip() const;
    uint64_t mtimecmp() const;
    uint64_t mtime() const;

    inline bool write_enable() const { return this->m_v.top()->cotm32->clint_we; }
    inline bool mtip() const { return this->m_v.top()->cotm32->clint_mtip; }

private:
    const VerilatedContainer& m_v;
    MemView m_mem;
};
