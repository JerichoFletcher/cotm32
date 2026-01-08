#pragma once

#include "verilated_container.hpp"

class CsrView {
  public:  
    CsrView(const VerilatedContainer& v);
    
    IData operator[](int index) const;

  private:
    const VerilatedContainer& m_v;
};
