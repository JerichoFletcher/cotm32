#pragma once

#include "verilated_container.hpp"

class RegView {
public:
    RegView(const VerilatedContainer& v);

    IData operator[](int index) const;

private:
    const VerilatedContainer& m_v;
};
