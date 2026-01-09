#pragma once

#include <string>

#include "controllers/time_controller.hpp"
#include "simulator.hpp"

class DumpController : public SimulatorUpdateListener {
public:
    static constexpr int MAX_DUMP_LENGTH = 1024;

    DumpController(TimeController& c_time);
    void update(Simulator& sim) override;

    inline bool is_dumping() const { return this->m_is_dumping; }

    void request_dump(const std::string& dump_name, int dump_length);

private:
    TimeController& m_c_time;

    bool m_req_dump;
    bool m_is_dumping;
    const std::string* m_dump_name;
    int m_dump_length;
};
