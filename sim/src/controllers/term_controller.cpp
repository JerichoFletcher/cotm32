#include "controllers/term_controller.hpp"

#include "Vtop_uart_term.h"
#include "colors.hpp"

TerminalController::TerminalController(
    size_t w, size_t h, size_t buf_cap, ImU32 fg_color, ImU32 bg_color
)
    : m_viewport_w(w),
      m_viewport_h(h),
      m_fg_color_init(fg_color),
      m_bg_color_init(bg_color),
      m_cursor_col(0),
      m_cursor_row(0),
      m_fg_color(fg_color),
      m_bg_color(bg_color),
      m_prev_tx_valid(false),
      m_buf(buf_cap),
      m_buf_cap(buf_cap),
      m_viewport_start_line(0) {
    std::vector<Symbol> initial(w, this->create_symbol('\0'));
    this->m_buf.push_back(std::move(initial));
}

TerminalController::TerminalController(size_t w, size_t h, size_t buf_cap)
    : TerminalController(
          w, h, buf_cap, cotm32::colors::TERM_BRIGHT_WHITE, cotm32::colors::TERM_BLACK
      ) {}

void TerminalController::per_tick_update(Simulator& sim) {
    auto* uart = sim.v().top()->cotm32->uart_term;

    if (uart->tx_valid && !this->m_prev_tx_valid) {
        this->restore_viewport();
        char c = uart->tx_data;
        size_t next_tab = ((this->m_cursor_col / TAB_WIDTH) + 1) * TAB_WIDTH;

        switch (c) {
            case '\r': this->m_cursor_col = 0; break;
            case '\n':
                this->m_cursor_col = 0;
                this->cursor_newline();
                break;
            case '\b':
                if (this->m_cursor_col > 0) {
                    this->m_cursor_col--;
                }
                break;
            case '\t':
                if (next_tab < this->m_viewport_w) {
                    this->m_cursor_col = next_tab;
                } else {
                    this->m_cursor_col = 0;
                    this->cursor_newline();
                }
                break;
            default:
                this->m_buf[this->m_viewport_start_line + this->m_cursor_row][this->m_cursor_col] =
                    this->create_symbol(c);
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
    this->m_buf.clear();

    this->m_cursor_col = 0;
    this->m_cursor_row = 0;
    this->m_viewport_start_line = 0;

    this->m_fg_color = this->m_fg_color_init;
    this->m_bg_color = this->m_bg_color_init;

    std::vector<Symbol> initial(this->m_viewport_w, this->create_symbol('\0'));
    this->m_buf.push_back(std::move(initial));
}

void TerminalController::scroll_up(size_t delta) {
    if (this->m_buf.size() > this->m_viewport_h) {
        this->m_viewport_start_line =
            this->m_viewport_start_line > delta ? this->m_viewport_start_line - delta : 0;
    }
}

void TerminalController::scroll_down(size_t delta) {
    if (this->m_buf.size() > this->m_viewport_h) {
        auto new_row = this->m_viewport_start_line + delta;
        auto max_row = this->m_buf.size() - this->m_viewport_h;

        this->m_viewport_start_line = new_row < max_row ? new_row : max_row;
    }
}

bool TerminalController::symbol_at_viewport(size_t row, size_t col, Symbol* s) {
    if (row < this->m_viewport_h && col < this->m_viewport_w) {
        auto i = this->m_viewport_start_line + row;
        if (i >= this->m_buf.size()) return false;

        *s = this->m_buf[i][col];
        return true;
    }
    return false;
}

void TerminalController::cursor_adv() {
    this->m_cursor_col++;
    if (this->m_cursor_col >= this->m_viewport_w) {
        this->m_cursor_col = 0;
        this->cursor_newline();
    }
}

void TerminalController::cursor_newline() {
    this->m_cursor_row++;

    if (this->m_viewport_start_line + this->m_cursor_row >= this->m_buf.size()) {
        std::vector<Symbol> newline(this->m_viewport_w, this->create_symbol('\0'));
        if (this->m_buf.full()) {
            this->m_buf.pop_front(nullptr);
        }
        this->m_buf.push_back(std::move(newline));
        this->restore_viewport();
    }

    if (this->m_cursor_row >= this->m_buf_cap) {
        this->m_cursor_row = this->m_buf_cap - 1;
    } else if (this->m_cursor_row >= this->m_viewport_h) {
        this->m_cursor_row = this->m_viewport_h - 1;
    }
}
