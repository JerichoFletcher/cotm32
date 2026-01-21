#pragma once

#include <queue>

#include "controllers/time_controller.hpp"
#include "simulator.hpp"
#include "struct/ring_buffer.hpp"

struct Symbol {
    char glyph;
    ImU32 fg_color;
    ImU32 bg_color;
};

class TerminalController : public PerTickUpdateListener, public ResetListener {
public:
    TerminalController(size_t w, size_t h, size_t buf_cap, ImU32 fg_color, ImU32 bg_color);
    TerminalController(size_t w, size_t h, size_t buf_cap);

    void per_tick_update(Simulator& sim) override;
    void reset(Simulator& sim) override;

    void scroll(int delta) { delta < 0 ? this->scroll_down(-delta) : this->scroll_up(delta); }
    void scroll_up(size_t delta);
    void scroll_down(size_t delta);
    bool symbol_at_viewport(size_t row, size_t col, Symbol* s);

    inline size_t viewport_width() const { return this->m_viewport_w; }
    inline size_t viewport_height() const { return this->m_viewport_h; }
    inline size_t viewport_start_row() const { return this->m_viewport_start_line; }
    inline size_t active_row() const { return this->m_viewport_start_line + this->m_cursor_row; }
    inline size_t viewport_to_buf_row(size_t viewport_row) {
        return this->m_viewport_start_line + viewport_row;
    }

    inline size_t cursor_col() const { return this->m_cursor_col; }
    inline size_t cursor_row() const { return this->m_cursor_row; }

    inline size_t buffer_size() const { return this->m_buf.size(); }
    inline size_t buffer_cap() const { return this->m_buf_cap; }
    inline size_t input_queue_size() const { return this->m_rx_queue.size(); }

    inline void enqueue_char(char c) { this->m_rx_queue.push(c); }

private:
    static inline constexpr size_t TAB_WIDTH = 4;

    typedef enum {
        ReadState_GROUND,
        ReadState_ESCAPE,
        ReadState_SEQ_CSI,
    } ReadState;

    ReadState m_state;
    std::vector<size_t> m_ansi_args;
    std::queue<char> m_ansi_arg_chars;

    size_t m_viewport_w;
    size_t m_viewport_h;
    ImU32 m_fg_color_init;
    ImU32 m_bg_color_init;

    size_t m_cursor_row;
    size_t m_cursor_col;
    ImU32 m_fg_color;
    ImU32 m_bg_color;

    RingBuffer<std::vector<Symbol>> m_buf;
    size_t m_buf_cap;
    size_t m_viewport_start_line;

    std::queue<uint8_t> m_rx_queue;

    bool m_prev_tx_valid;

    inline Symbol create_symbol(char c) const {
        Symbol s;
        s.glyph = c;
        s.fg_color = this->m_fg_color;
        s.bg_color = this->m_bg_color;
        return s;
    }

    inline void restore_viewport() {
        this->m_viewport_start_line =
            this->m_buf.size() > this->m_viewport_h ? this->m_buf.size() - this->m_viewport_h : 0;
    }

    void push_blank_row();
    void clear_buffer();

    void cursor_adv();
    void cursor_newline();
};
