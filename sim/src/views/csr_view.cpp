#include "views/csr_view.hpp"

#include <stdexcept>

#include "Vtop_cotm32.h"
#include "Vtop_csr_file.h"
#include "Vtop_processor_core.h"
#include "cotm32_defs.hpp"

CsrView::CsrView(const VerilatedContainer& v) : m_v(v) {}

IData& CsrView::operator[](CsrId id) {
    auto* csr = this->m_v.top()->cotm32->core->csr;
    return csr->mem.at(id);
}

const IData& CsrView::operator[](CsrId id) const {
    auto* csr = this->m_v.top()->cotm32->core->csr;
    return csr->mem.at(id);
}
