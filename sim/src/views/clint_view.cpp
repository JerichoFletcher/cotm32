#include "views/clint_view.hpp"

ClintView::ClintView(const VerilatedContainer& v) : m_v(v), m_mem(v) {}

bool ClintView::msip() const {
    uint32_t v;
    return this->m_mem.read_word(0x02'000000, v) && v == 1;
}

uint64_t ClintView::mtimecmp() const {
    uint32_t mtimel, mtimeh;
    if (this->m_mem.read_word(0x02'004000, mtimel) && this->m_mem.read_word(0x02'004004, mtimeh)) {
        return (((uint64_t)mtimeh & 0xffffffff) << 32) | (((uint64_t)mtimel & 0xffffffff));
    }
    return 0;
}

uint64_t ClintView::mtime() const {
    uint32_t mtimel, mtimeh;
    if (this->m_mem.read_word(0x02'00bff8, mtimel) && this->m_mem.read_word(0x02'00bffc, mtimeh)) {
        return (((uint64_t)mtimeh & 0xffffffff) << 32) | (((uint64_t)mtimel & 0xffffffff));
    }
    return 0;
}
