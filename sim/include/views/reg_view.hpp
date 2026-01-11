#pragma once

#include "verilated_container.hpp"

class RegView {
public:
    RegView(const VerilatedContainer& v);

    IData operator[](int index) const;

    inline bool write_enable() const {
        return this->m_v.top()->cotm32->core->wb_regfile_we &&
               this->m_v.top()->cotm32->core->memwb_valid;
    }

private:
    const VerilatedContainer& m_v;
};
