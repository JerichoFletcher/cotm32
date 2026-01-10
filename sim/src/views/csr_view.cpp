#include "views/csr_view.hpp"

#include <stdexcept>

#include "Vtop_cotm32.h"
#include "Vtop_csr_file.h"
#include "Vtop_processor_core.h"
#include "cotm32_defs.hpp"

static const std::map<CsrId, size_t> csr_indices = {
    {CsrId::MTVEC,  0},
    {CsrId::MEPC,   1},
    {CsrId::MCAUSE, 2},
    {CsrId::MTVAL,  3},
};

CsrView::CsrView(const VerilatedContainer& v) : m_v(v) {}

IData CsrView::operator[](CsrId id) const {
    auto i = csr_indices.find(id);
    if (i == csr_indices.end()) {
        throw std::range_error("Unknown CSR identifier");
    }
    auto* csr = this->m_v.top()->cotm32->core->csr;
    return csr->mem[i->second];
}
