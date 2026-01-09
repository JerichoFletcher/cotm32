#include "views/csr_view.hpp"

#include <stdexcept>

#include "Vtop_cotm32.h"
#include "Vtop_csr_file__C20.h"
#include "Vtop_processor_core.h"
#include "cotm32_defs.hpp"

CsrView::CsrView(const VerilatedContainer& v) : m_v(v) {}

IData CsrView::operator[](int index) const {
    if (index < 0 || index >= NUM_CSR) {
        throw std::range_error("CSR index out of range");
    }
    auto* csr = this->m_v.top()->cotm32->core->csr;
    return csr->mem[index];
}
