#include "controllers/dump_controller.hpp"

DumpController::DumpController(TimeController& c_time) : m_c_time(c_time), m_req_dump(false) {}

void DumpController::update(Simulator& sim) {
    if (this->m_req_dump && !this->m_is_dumping) {
        if (sim.v().is_dumping()) return;

        sim.v().start_trace_dump(*this->m_dump_name, this->m_dump_length);
        this->m_c_time.is_auto() = true;

        this->m_req_dump = false;
        this->m_is_dumping = true;
    }

    if (this->m_is_dumping && !sim.v().is_dumping()) {
        this->m_is_dumping = false;
        this->m_c_time.is_auto() = false;
    }
}

void DumpController::request_dump(const std::string& dump_name, int dump_length) {
    if (this->m_req_dump || this->m_is_dumping) return;

    this->m_req_dump = true;
    this->m_dump_name = &dump_name;
    this->m_dump_length = dump_length;
}
