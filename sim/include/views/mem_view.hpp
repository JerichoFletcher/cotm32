#pragma once

#include "verilated_container.hpp"

class MemView {
public:
    MemView(const VerilatedContainer& v);

    inline bool write_enable() const {
        return this->m_v.top()->cotm32->core->mem_dmem_we &&
               this->m_v.top()->cotm32->core->exmem_valid &&
               !this->m_v.top()->cotm32->core->trap_req;
    }

    inline bool read_byte(uint32_t addr, uint8_t& out) const {
        return this->m_v.read_byte(addr, &out);
    }

    inline bool read_half(uint32_t addr, uint16_t& out) const {
        uint8_t val8_0, val8_1;
        if (this->m_v.read_byte(addr, &val8_0) && this->m_v.read_byte(addr + 1, &val8_1)) {
            out = (((uint16_t)val8_1 & 0xff) << 8) | ((uint16_t)val8_0 & 0xff);
            return true;
        }
        return false;
    }

    inline bool read_word(uint32_t addr, uint32_t& out) const {
        uint8_t val8_0, val8_1, val8_2, val8_3;
        if (this->m_v.read_byte(addr, &val8_0) && this->m_v.read_byte(addr + 1, &val8_1) &&
            this->m_v.read_byte(addr + 2, &val8_2) && this->m_v.read_byte(addr + 3, &val8_3)) {
            out = (((uint32_t)val8_3 & 0xff) << 24) | (((uint32_t)val8_2 & 0xff) << 16) |
                  (((uint32_t)val8_1 & 0xff) << 8) | ((uint32_t)val8_0 & 0xff);
            return true;
        }
        return false;
    }

private:
    const VerilatedContainer& m_v;
};
