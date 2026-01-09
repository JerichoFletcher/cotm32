#pragma once

#include <string>

#include "controllers/dump_controller.hpp"
#include "simulator.hpp"

class DumpDrawer : public SimulatorRenderListener {
public:
    DumpDrawer(DumpController& c_dump);
    void render(const Simulator& sim) override;

private:
    static constexpr inline size_t DUMP_NAME_BUF_SIZE = 25;

    DumpController& m_c_dump;

    int m_length;
    std::string m_dump_name;
};
