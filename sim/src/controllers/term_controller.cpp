#include "controllers/term_controller.hpp"

#include "Vtop_uart_term.h"
#include "colors.hpp"
#include "compare.hpp"

inline size_t el_or_default(const std::vector<size_t>& v, size_t i, size_t def) {
    return (v.size() > i) ? v[i] : def;
}

size_t parse_as_uint(std::queue<char>& q) {
    size_t val = 0;
    while (!q.empty()) {
        char argc = q.front();
        val = 10 * val + (argc - '0');
        q.pop();
    }
    return val;
}

TerminalController::TerminalController(
    size_t w, size_t h, size_t buf_cap, ImU32 fg_color, ImU32 bg_color
)
    : m_state(ReadState_GROUND),
      m_viewport_w(w),
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
    this->push_blank_row();
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

        switch (this->m_state) {
            case ReadState_GROUND: {
                switch (c) {
                    case '\e': this->m_state = ReadState_ESCAPE; break;
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
                    case '\t': {
                        size_t next_tab = ((this->m_cursor_col / TAB_WIDTH) + 1) * TAB_WIDTH;
                        if (next_tab < this->m_viewport_w) {
                            this->m_cursor_col = next_tab;
                        } else {
                            this->m_cursor_col = 0;
                            this->cursor_newline();
                        }
                        break;
                    }
                    default: {
                        Symbol s = this->create_symbol(c);
                        this->m_buf[this->active_row()][this->m_cursor_col] = s;
                        this->cursor_adv();
                        break;
                    }
                }
                break;
            }

            case ReadState_ESCAPE: {
                this->m_ansi_args.clear();
                while (!this->m_ansi_arg_chars.empty()) this->m_ansi_arg_chars.pop();

                switch (c) {
                    case '[': this->m_state = ReadState_SEQ_CSI; break;
                    default: this->m_state = ReadState_GROUND; break;
                }
                break;
            }

            case ReadState_SEQ_CSI: {
                if ('0' <= c && c <= '9') {
                    this->m_ansi_arg_chars.push(c);
                    break;
                }
                switch (c) {
                    case ';': {
                        size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                        this->m_ansi_args.push_back(arg);
                        break;
                    }
                    case 'A': {
                        if (!this->m_ansi_arg_chars.empty()) {
                            size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                            this->m_ansi_args.push_back(arg);
                        }

                        size_t n = el_or_default(this->m_ansi_args, 0, 1);
                        this->m_cursor_row = U_SUBCLAMP0(this->m_cursor_row, n);

                        this->m_state = ReadState_GROUND;
                        break;
                    }
                    case 'B': {
                        if (!this->m_ansi_arg_chars.empty()) {
                            size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                            this->m_ansi_args.push_back(arg);
                        }

                        size_t n = el_or_default(this->m_ansi_args, 0, 1);
                        this->m_cursor_row = MIN(this->m_cursor_row + n, this->m_viewport_h - 1);
                        this->push_blank_until(this->active_row() + 1);

                        this->m_state = ReadState_GROUND;
                        break;
                    }
                    case 'C': {
                        if (!this->m_ansi_arg_chars.empty()) {
                            size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                            this->m_ansi_args.push_back(arg);
                        }

                        size_t n = el_or_default(this->m_ansi_args, 0, 1);
                        this->m_cursor_col = MIN(this->m_cursor_col + n, this->m_viewport_w - 1);

                        this->m_state = ReadState_GROUND;
                        break;
                    }
                    case 'D': {
                        if (!this->m_ansi_arg_chars.empty()) {
                            size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                            this->m_ansi_args.push_back(arg);
                        }

                        size_t n = el_or_default(this->m_ansi_args, 0, 1);
                        this->m_cursor_col = U_SUBCLAMP0(this->m_cursor_col, n);

                        this->m_state = ReadState_GROUND;
                        break;
                    }
                    case 'H': {
                        if (!this->m_ansi_arg_chars.empty()) {
                            size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                            this->m_ansi_args.push_back(arg);
                        }

                        size_t n = el_or_default(this->m_ansi_args, 0, 1) - 1;
                        size_t m = el_or_default(this->m_ansi_args, 1, 1) - 1;
                        size_t new_row = MIN(n, this->m_viewport_h - 1);
                        size_t new_col = MIN(m, this->m_viewport_w - 1);
                        this->push_blank_until(this->viewport_to_buf_row(new_row) + 1);

                        this->m_cursor_row = new_row;
                        this->m_cursor_col = new_col;

                        this->m_state = ReadState_GROUND;
                        break;
                    }
                    case 'J': {
                        if (!this->m_ansi_arg_chars.empty()) {
                            size_t arg = parse_as_uint(this->m_ansi_arg_chars);
                            this->m_ansi_args.push_back(arg);
                        }

                        size_t n = el_or_default(this->m_ansi_args, 0, 0);
                        switch (n) {
                            case 0: {
                                auto& curr_row = this->m_buf[this->active_row()];
                                for (auto i = curr_row.begin() + this->m_cursor_col;
                                     i != curr_row.end();
                                     i++) {
                                    *i = this->create_symbol('\0');
                                }

                                for (size_t r = this->active_row() + 1;
                                     r <= this->viewport_to_buf_row(this->m_viewport_h - 1);
                                     r++) {
                                    curr_row = this->m_buf[r];
                                    for (auto i = curr_row.begin(); i != curr_row.end(); i++) {
                                        *i = this->create_symbol('\0');
                                    }
                                }
                                break;
                            }
                            case 1: {
                                auto& curr_row = this->m_buf[this->active_row()];
                                for (auto i = curr_row.begin();
                                     i != curr_row.begin() + this->m_cursor_col;
                                     i++) {
                                    *i = this->create_symbol('\0');
                                }

                                for (size_t r = this->viewport_to_buf_row(0);
                                     r <= this->active_row() - 1;
                                     r++) {
                                    curr_row = this->m_buf[r];
                                    for (auto i = curr_row.begin(); i != curr_row.end(); i++) {
                                        *i = this->create_symbol('\0');
                                    }
                                }
                                break;
                            }
                            case 2: {
                                for (size_t r = this->viewport_to_buf_row(0);
                                     r <= this->viewport_to_buf_row(this->m_viewport_h - 1);
                                     r++) {
                                    auto& curr_row = this->m_buf[r];
                                    for (auto i = curr_row.begin(); i != curr_row.end(); i++) {
                                        *i = this->create_symbol('\0');
                                    }
                                }
                                break;
                            }
                            case 3: {
                                this->clear_buffer();
                                this->push_blank_row();
                                break;
                            }
                        }
                        this->m_state = ReadState_GROUND;
                        break;
                    }
                    default: this->m_state = ReadState_GROUND; break;
                }
                break;
            }
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
    this->m_cursor_col = 0;
    this->m_cursor_row = 0;

    this->m_fg_color = this->m_fg_color_init;
    this->m_bg_color = this->m_bg_color_init;

    this->clear_buffer();
    this->push_blank_row();
}

void TerminalController::scroll_up(size_t delta) {
    if (this->m_buf.size() > this->m_viewport_h) {
        this->m_viewport_start_line = U_SUBCLAMP0(this->m_viewport_start_line, delta);
    }
}

void TerminalController::scroll_down(size_t delta) {
    if (this->m_buf.size() > this->m_viewport_h) {
        auto new_row = this->m_viewport_start_line + delta;
        auto max_row = this->m_buf.size() - this->m_viewport_h;

        this->m_viewport_start_line = MIN(new_row, max_row);
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

void TerminalController::push_blank_row() {
    if (this->m_buf.full()) {
        this->m_buf.pop_front(nullptr);
    }
    std::vector<Symbol> row(this->m_viewport_w, this->create_symbol('\0'));
    this->m_buf.push_back(std::move(row));
}

void TerminalController::push_blank_until(size_t target_size) {
    while (this->m_buf.size() < target_size && !this->m_buf.full()) {
        this->push_blank_row();
    }
}

void TerminalController::clear_buffer() {
    this->m_buf.clear();
    this->m_viewport_start_line = 0;
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

    if (this->active_row() >= this->m_buf.size()) {
        this->push_blank_until(this->active_row() + 1);
        this->restore_viewport();
    }

    if (this->m_cursor_row >= this->m_buf_cap) {
        this->m_cursor_row = this->m_buf_cap - 1;
    } else if (this->m_cursor_row >= this->m_viewport_h) {
        this->m_cursor_row = this->m_viewport_h - 1;
    }
}
