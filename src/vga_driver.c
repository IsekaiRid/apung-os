#include "vga_driver.h"

static u8 vga_entry_color(VGAColor fg, VGAColor bg) {
    return fg | (bg << 4);
}

static u16 vga_entry(char c, u8 color) {
    return ((u16)color << 8) | (u8)c;
}

void vga_terminal_init(VGATerminal* terminal) {
    terminal->buffer = (u16*)VGA_BUFFER_ADDRESS;
    terminal->width = VGA_WIDTH;
    terminal->height = VGA_HEIGHT;
    terminal->cursor_x = 0;
    terminal->cursor_y = 0;
    terminal->current_fg = VGA_COLOR_LIGHT_GREY;
    terminal->current_bg = VGA_COLOR_BLACK;
    vga_terminal_clear(terminal);
}

void vga_terminal_clear(VGATerminal* terminal) {
    u8 color = vga_entry_color(terminal->current_fg, terminal->current_bg);
    u16 entry = vga_entry(' ', color);
    for (int y = 0; y < terminal->height; y++) {
        for (int x = 0; x < terminal->width; x++) {
            terminal->buffer[y * terminal->width + x] = entry;
        }
    }
    terminal->cursor_x = 0;
    terminal->cursor_y = 0;
}

void vga_terminal_set_color(VGATerminal* terminal, VGAColor fg, VGAColor bg) {
    terminal->current_fg = fg;
    terminal->current_bg = bg;
}

void vga_terminal_put_char(VGATerminal* terminal, char c) {
    if (c == '\n') {
        terminal->cursor_x = 0;
        terminal->cursor_y++;
    } else {
        u8 color = vga_entry_color(terminal->current_fg, terminal->current_bg);
        terminal->buffer[terminal->cursor_y * terminal->width + terminal->cursor_x] = vga_entry(c, color);
        terminal->cursor_x++;
    }

    if (terminal->cursor_x >= terminal->width) {
        terminal->cursor_x = 0;
        terminal->cursor_y++;
    }

    if (terminal->cursor_y >= terminal->height) {
        // Scroll layar
        for (int y = 1; y < terminal->height; y++) {
            for (int x = 0; x < terminal->width; x++) {
                terminal->buffer[(y - 1) * terminal->width + x] = terminal->buffer[y * terminal->width + x];
            }
        }
        // Kosongkan baris terakhir
        u8 color = vga_entry_color(terminal->current_fg, terminal->current_bg);
        u16 blank = vga_entry(' ', color);
        for (int x = 0; x < terminal->width; x++) {
            terminal->buffer[(terminal->height - 1) * terminal->width + x] = blank;
        }
        terminal->cursor_y = terminal->height - 1;
    }
}

void vga_terminal_write(VGATerminal* terminal, const char* str) {
    while (*str) {
        vga_terminal_put_char(terminal, *str++);
    }
}

void vga_terminal_write_at(VGATerminal* terminal, const char* str, int x, int y) {
    if (x >= 0 && x < terminal->width && y >= 0 && y < terminal->height) {
        terminal->cursor_x = x;
        terminal->cursor_y = y;
        vga_terminal_write(terminal, str);
    }
}

void vga_terminal_put_char_at(VGATerminal* terminal, char c, int x, int y) {
    if (x >= 0 && x < terminal->width && y >= 0 && y < terminal->height) {
        u8 color = vga_entry_color(terminal->current_fg, terminal->current_bg);
        terminal->buffer[y * terminal->width + x] = vga_entry(c, color);
    }
}

void vga_terminal_backspace(VGATerminal* terminal) {
    if (terminal->cursor_x == 0 && terminal->cursor_y == 0)
        return;

    if (terminal->cursor_x == 0) {
        terminal->cursor_y--;
        terminal->cursor_x = terminal->width - 1;
    } else {
        terminal->cursor_x--;
    }

    vga_terminal_put_char_at(terminal, ' ', terminal->cursor_x, terminal->cursor_y);
}
