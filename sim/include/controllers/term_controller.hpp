#pragma once

#include <queue>
#include <string>

#include "controllers/time_controller.hpp"
#include "simulator.hpp"

class TerminalController : public PerTickUpdateListener, public ResetListener {
public:
    TerminalController(size_t w, size_t h);
    void per_tick_update(Simulator& sim) override;
    void reset(Simulator& sim) override;

    inline size_t buf_width() const { return this->m_bufw; }
    inline size_t buf_height() const { return this->m_bufh; }
    inline const std::string& buf() const { return this->m_buf; }
    inline size_t cursor_col() const { return this->m_curx; }
    inline size_t cursor_row() const { return this->m_cury; }

    inline void enqueue_char(char c) { this->m_rx_queue.push(c); }

private:
    size_t m_bufw;
    size_t m_bufh;

    size_t m_curx;
    size_t m_cury;

    std::string m_buf;
    std::queue<uint8_t> m_rx_queue;

    bool m_prev_tx_valid;

    inline size_t get_cursor_idx() const { return this->m_cury * this->m_bufw + this->m_curx; }

    void cursor_adv();
    void cursor_newline();
};
