#pragma once

#include "cotm32_defs.hpp"
#include "verilated_container.hpp"

class CsrView {
public:
    CsrView(const VerilatedContainer& v);

    IData operator[](CsrId index) const;

private:
    const VerilatedContainer& m_v;
};
