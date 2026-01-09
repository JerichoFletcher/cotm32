#include "views/reg_view.hpp"

#include <stdexcept>

#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"
#include "Vtop_register_file__NB20.h"
#include "cotm32_defs.hpp"

RegView::RegView(const VerilatedContainer& v) : m_v(v) {}

IData RegView::operator[](int index) const {
    if (index < 0 || index >= NUM_REGS) {
        throw std::range_error("Register index out of range");
    }
    auto* rf = this->m_v.top()->cotm32->core->rf;
    return rf->qbus[index];
}
