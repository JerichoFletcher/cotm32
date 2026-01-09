#pragma once

#include <memory>
#include <string>

#include "Vtop.h"
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

class VerilatedContainer {
public:
    VerilatedContainer(int argc, char** argv);
    ~VerilatedContainer();

    inline std::unique_ptr<Vtop>& top() { return this->m_top; }

    inline uint64_t time() const { return this->m_contextp->time(); }
    inline IData pc() const { return this->m_top->cotm32->core->pc; }
    inline IData inst() const { return this->m_top->cotm32->core->inst; }
    inline bool is_dumping() const { return this->m_tfp->isOpen(); }

    inline const std::unique_ptr<VerilatedContext>& context() const { return this->m_contextp; }
    inline const std::unique_ptr<Vtop>& top() const { return this->m_top; }

    void start();
    void reset();
    void update();
    void finish();

    void start_trace_dump(const std::string& filename, int dump_length);
    void stop_trace_dump();

    bool read_byte(uint32_t addr, uint8_t* out) const;
    bool write_byte(uint32_t addr, uint8_t val);

private:
    std::unique_ptr<VerilatedContext> m_contextp;
    std::unique_ptr<VerilatedVcdC> m_tfp;
    std::unique_ptr<Vtop> m_top;

    bool m_started;
    bool m_finished;

    int m_dump_remaining_steps;

    void dump_step();
};
