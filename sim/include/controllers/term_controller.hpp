#pragma once

#include <queue>
#include <vector>

#include "controllers/time_controller.hpp"
#include "simulator.hpp"

struct Symbol {
    char glyph;
    ImU32 fg_color;
    ImU32 bg_color;
};

class TerminalController : public PerTickUpdateListener, public ResetListener {
public:
    TerminalController(size_t w, size_t h, ImU32 fg_color, ImU32 bg_color);
    TerminalController(size_t w, size_t h);

    void per_tick_update(Simulator& sim) override;
    void reset(Simulator& sim) override;

    bool symbol_at(size_t row, size_t col, Symbol* s);

    inline size_t buf_width() const { return this->m_bufw; }
    inline size_t buf_height() const { return this->m_bufh; }
    inline size_t cursor_col() const { return this->m_curx; }
    inline size_t cursor_row() const { return this->m_cury; }

    inline const std::vector<Symbol>& buf() const { return this->m_buf; }
    inline size_t input_queue_size() const { return this->m_rx_queue.size(); }

    inline void enqueue_char(char c) { this->m_rx_queue.push(c); }

private:
    static inline constexpr size_t TAB_WIDTH = 4;

    size_t m_bufw;
    size_t m_bufh;

    size_t m_curx;
    size_t m_cury;
    ImU32 m_fg_color;
    ImU32 m_bg_color;

    std::vector<Symbol> m_buf;
    std::queue<uint8_t> m_rx_queue;

    bool m_prev_tx_valid;

    inline size_t get_cursor_idx() const { return this->m_cury * this->m_bufw + this->m_curx; }
    inline Symbol create_symbol(char c) const {
        Symbol s;
        s.glyph = c;
        s.fg_color = this->m_fg_color;
        s.bg_color = this->m_bg_color;
        return s;
    }

    void cursor_adv();
    void cursor_newline();
};
