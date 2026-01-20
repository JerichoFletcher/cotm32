#include "controllers/term_controller.hpp"

#include <algorithm>

#include "Vtop_uart_term.h"

TerminalController::TerminalController(size_t w, size_t h)
    : m_bufw(w),
      m_bufh(h),
      m_curx(0),
      m_cury(0),
      m_buf(w * h, ' '),
      m_prev_tx_valid(false) {}

void TerminalController::per_tick_update(Simulator& sim) {
    auto* uart = sim.v().top()->cotm32->uart_term;

    if (uart->tx_valid && !this->m_prev_tx_valid) {
        char c = uart->tx_data;
        size_t next_tab = ((this->m_curx / TAB_WIDTH) + 1) * TAB_WIDTH;

        switch (c) {
            case '\r': this->m_curx = 0; break;
            case '\n':
                this->m_curx = 0;
                this->cursor_newline();
                break;
            case '\b':
                if (this->m_curx > 0) {
                    this->m_curx--;
                }
                break;
            case '\t':
                if (next_tab < this->m_bufw) {
                    this->m_curx = next_tab;
                } else {
                    this->m_curx = 0;
                    this->cursor_newline();
                }
                break;
            default:
                this->m_buf[this->get_cursor_idx()] = c;
                this->cursor_adv();
                break;
        }
    }
    this->m_prev_tx_valid = uart->tx_valid;

    if (!this->m_rx_queue.empty() && !uart->rx_valid) {
        uart->rx_data = this->m_rx_queue.front();
        uart->rx_valid = 1;
        this->m_rx_queue.pop();
    }
}

void TerminalController::reset(Simulator& sim) {
    std::fill(this->m_buf.begin(), this->m_buf.end(), ' ');
    this->m_curx = 0;
    this->m_cury = 0;
}

void TerminalController::cursor_adv() {
    this->m_curx++;
    if (this->m_curx >= this->m_bufw) {
        this->m_curx = 0;
        this->cursor_newline();
    }
}

void TerminalController::cursor_newline() {
    this->m_cury++;

    if (this->m_cury >= this->m_bufh) {
        std::rotate(this->m_buf.begin(), this->m_buf.begin() + this->m_bufw, this->m_buf.end());
        for (auto v = this->m_buf.rbegin(); v != this->m_buf.rbegin() + this->m_bufw; v++) {
            *v = ' ';
        }
        this->m_cury = this->m_bufh - 1;
    }
}
