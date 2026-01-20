#include "verilated_container.hpp"

#include <stdexcept>

#include "Vtop_clint.h"
#include "cotm32_defs.hpp"

VerilatedContainer::VerilatedContainer(int argc, char** argv)
    : m_started(false),
      m_finished(false) {
    this->m_contextp = std::make_unique<VerilatedContext>();
    this->m_contextp->commandArgs(argc, argv);

    this->m_tfp = std::make_unique<VerilatedVcdC>();
    this->m_top = std::make_unique<Vtop>(this->m_contextp.get());
    this->m_top->trace(this->m_tfp.get(), 99);
}

VerilatedContainer::~VerilatedContainer() {
    if (this->m_tfp && this->m_tfp->isOpen()) {
        this->m_tfp->close();
    }
}

void VerilatedContainer::start() {
    if (this->m_started) {
        throw std::runtime_error("Cannot start, container already started");
    }
    if (this->m_finished) {
        throw std::runtime_error("Cannot start, container already finished");
    }

    this->m_started = true;
    this->reset();
}

void VerilatedContainer::reset() {
    this->m_top->i_rst = 1;
    this->update();
    this->m_top->i_rst = 0;
}

void VerilatedContainer::update() {
    if (!this->m_started) {
        throw std::runtime_error("Cannot tick, container not started");
    }
    if (this->m_finished) {
        throw std::runtime_error("Cannot tick, container already finished");
    }

    this->m_top->i_clk = 0;
    this->m_top->eval();
    this->m_contextp->timeInc(1UL);
    this->dump_step();

    this->m_top->i_clk = 1;
    this->m_top->eval();
    this->m_contextp->timeInc(1UL);
    this->dump_step();
}

void VerilatedContainer::finish() {
    if (!this->m_started) {
        throw std::runtime_error("Cannot finish, container not started");
    }
    if (this->m_finished) {
        throw std::runtime_error("Cannot finish, container already finished");
    }

    this->m_top->final();
    this->m_finished = true;
}

void VerilatedContainer::start_trace_dump(const std::string& filename, int dump_length) {
    if (this->is_dumping()) return;

    Verilated::traceEverOn(true);
    this->m_tfp->set_time_unit(this->m_contextp->timeunitString());
    this->m_tfp->set_time_resolution(this->m_contextp->timeprecisionString());

    this->m_tfp->open(filename.c_str());
    this->m_d_proc_steps = 0;
    this->m_d_remn_steps = dump_length;
}

void VerilatedContainer::stop_trace_dump() {
    if (!this->is_dumping()) return;

    this->m_tfp->close();
}

bool VerilatedContainer::read_byte(uint32_t addr, uint8_t* out) const {
    if (BOOTROM_MEM_START <= addr && addr <= BOOTROM_MEM_END) {
        *out = this->m_top->cotm32->core->bootrom__DOT__mem_bytes[addr - BOOTROM_MEM_START];
    } else if (CLINT_MEM_START <= addr && addr <= CLINT_MEM_END) {
        auto& mem = this->m_top->cotm32->clint->mem;
        auto addr_base = addr - CLINT_MEM_START;
        auto addr_align4 = addr_base & 0xfffffffc;
        auto shamt = (addr_base - addr_align4) * 8;

        auto t = mem.at(addr_align4);
        t = (t & (0xff << shamt)) >> shamt;
        *out = t;
    } else if (DATA_MEM_START <= addr && addr <= DATA_MEM_END) {
        *out = this->m_top->cotm32->core->dmem__DOT__mem_bytes[addr - DATA_MEM_START];
    } else {
        return false;
    }
    return true;
}

bool VerilatedContainer::write_byte(uint32_t addr, uint8_t val) {
    if (BOOTROM_MEM_START <= addr && addr <= BOOTROM_MEM_END) {
        this->m_top->cotm32->core->bootrom__DOT__mem_bytes[addr - BOOTROM_MEM_START] = val;
    } else if (CLINT_MEM_START <= addr && addr <= CLINT_MEM_END) {
        auto& mem = this->m_top->cotm32->clint->mem;
        auto addr_base = addr - CLINT_MEM_START;
        auto addr_align4 = addr_base & 0xfffffffc;
        auto shamt = (addr_base - addr_align4) * 8;

        auto t = mem.at(addr_align4);
        t = ((t & ~(0xff << shamt)) | (((uint32_t)val & 0xff) << shamt)) >> shamt;
        mem.at(addr_align4) = t;
    } else if (DATA_MEM_START <= addr && addr <= DATA_MEM_END) {
        this->m_top->cotm32->core->dmem__DOT__mem_bytes[addr - DATA_MEM_START] = val;
    } else {
        return false;
    }
    return true;
}

void VerilatedContainer::dump_step() {
    if (!this->is_dumping()) return;

    this->m_tfp->dump(this->time());
    this->m_d_proc_steps++;
    this->m_d_remn_steps--;
    if (this->m_d_remn_steps <= 0) {
        this->stop_trace_dump();
    }
}
