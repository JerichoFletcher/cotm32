#include "simulator.hpp"

Simulator::Simulator(VerilatedContainer& v) : m_v(v) {}

void Simulator::add_update_listener(SimulatorUpdateListener* listener) {
    this->m_l_update.push_back(listener);
}

void Simulator::add_render_listener(SimulatorRenderListener* listener) {
    this->m_l_render.push_back(listener);
}

void Simulator::update() {
    for (auto* l : this->m_l_update) l->update(*this);
}

void Simulator::draw() {
    for (auto* l : this->m_l_render) l->render(*this);
}
