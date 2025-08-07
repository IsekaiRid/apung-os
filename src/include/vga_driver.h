#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER_ADDRESS 0xB8000

// Tipe dasar buatan sendiri
typedef unsigned char u8;
typedef unsigned short u16;

// Warna VGA
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
} VGAColor;

typedef struct {
    u16* buffer;
    int width;
    int height;
    int cursor_x;
    int cursor_y;
    VGAColor current_fg;
    VGAColor current_bg;
} VGATerminal;

// Fungsi
void vga_terminal_init(VGATerminal* terminal);
void vga_terminal_clear(VGATerminal* terminal);
void vga_terminal_set_color(VGATerminal* terminal, VGAColor fg, VGAColor bg);
void vga_terminal_write(VGATerminal* terminal, const char* str);
void vga_terminal_write_at(VGATerminal* terminal, const char* str, int x, int y);
void vga_terminal_put_char(VGATerminal* terminal, char c);
void vga_terminal_put_char_at(VGATerminal* terminal, char c, int x, int y);
void vga_terminal_backspace(VGATerminal* terminal);

#endif
