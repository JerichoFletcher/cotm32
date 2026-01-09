#pragma once

#include "verilated_container.hpp"

struct IfIdRegView {
    bool valid;
    bool stall;
    bool flush;

    IData pc;
    IData pc_4;
    IData inst;
};

class PipelineRegsView {
public:
    PipelineRegsView(const VerilatedContainer& v);

    const IfIdRegView if_id() const;

private:
    const VerilatedContainer& m_v;
};
