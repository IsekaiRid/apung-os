#include "vga_driver.h"
#include "input_driver.h"

// Delay helper
void delay(long count)
{
    for (volatile long i = 0; i < count; ++i)
        ;
}

void kernel_main()
{
    VGATerminal terminal;
    vga_terminal_init(&terminal);

    // ;;;;;;;;;;;;;;;;;; ;
    // ;   bagian logo    ;
    // ;;;;;;;;;;;;;;;;;; ;

    const char *logo[] = {
        "    _                               ___  ",    
        "       / \\   _ __  _   _ _ __   __ _   / _ \\ ___  ",
        "      / _ \\ | '_ \\| | | | '_ \\ / _` | | | | / __|",
        "     / ___ \\| |_) | |_| | | | | (_| | | |_| \\__ \\ ",
        "    /_/   \\_\\ .__/ \\__,_|_| |_|\\__, |  \\___/|___/",
        "            |_|                |___/             ",
        "               Welcome to MyOS!                ",
    };
    const int logo_lines = sizeof(logo) / sizeof(logo[0]);

    // Tampilkan animasi logo sekali
    vga_terminal_clear(&terminal);
    vga_terminal_set_color(&terminal, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    int y_start = (VGA_HEIGHT - logo_lines) / 2;
    for (int i = 0; i < logo_lines; ++i)
    {
        int len = 0;
        while (logo[i][len] != '\0')
            len++;
        int x = (VGA_WIDTH - len) / 2;
        vga_terminal_write_at(&terminal, logo[i], x, y_start + i);
        delay(50000000); // Delay antar baris
    }
    delay(100000000); // Delay setelah logo selesai

    // Inisialisasi keyboard
    init_keyboard();

    // ;;;;;;;;;;;;;;;;;;;;;; ;
    // ; bagian sistem input  ;
    // ;;;;;;;;;;;;;;;;;;;;;; ;

    const char *prompt = "Apung OS> ";
    int prompt_len = 0;
    while (prompt[prompt_len] != '\0')
        prompt_len++;
    int prompt_y = y_start + logo_lines + 1;
    vga_terminal_write_at(&terminal, prompt, 0, prompt_y);

    char input_buffer[80] = {0};
    int input_pos = 0;

    unsigned char current_scancode = 0;
    unsigned char last_scancode = 0;
    int key_pressed = 0;

    while (1)
    {
        current_scancode = inb(0x60); // Baca scancode terbaru

        if (current_scancode != last_scancode)
        {
            if (!(current_scancode & 0x80) && !key_pressed)
            {
                if (current_scancode < SCANCODES_SIZE)
                {
                    char character = scancode_set1[current_scancode];

                    if (character && character != '\n')
                    {
                        if (input_pos < 79)
                        {
                            input_buffer[input_pos++] = character;
                            input_buffer[input_pos] = '\0';

                            // Tampilkan prompt + input
                            for (int i = 0; i < prompt_len + 79; ++i)
                            {
                                vga_terminal_write_at(&terminal, " ", i, prompt_y);
                            }
                            vga_terminal_write_at(&terminal, prompt, 0, prompt_y);
                            vga_terminal_write_at(&terminal, input_buffer, prompt_len, prompt_y);
                        }
                    }
                    else if (current_scancode == 0x1C) // Enter key
                    {
                        vga_terminal_write_at(&terminal, "\n", 0, prompt_y + 1);
                        vga_terminal_write_at(&terminal, prompt, 0, prompt_y + 1);
                        input_pos = 0;
                        input_buffer[0] = '\0';
                        prompt_y++; // Pindah baris setelah Enter
                    }
                    else if (current_scancode == 0x0E) // Backspace key
                    {
                        if (input_pos > 0)
                        {
                            input_pos--;
                            input_buffer[input_pos] = '\0';

                            // Hapus baris lalu gambar ulang prompt dan input
                            for (int i = 0; i < prompt_len + 79; ++i)
                            {
                                vga_terminal_write_at(&terminal, " ", i, prompt_y);
                            }
                            vga_terminal_write_at(&terminal, prompt, 0, prompt_y);
                            vga_terminal_write_at(&terminal, input_buffer, prompt_len, prompt_y);
                        }
                    }
                }
                key_pressed = 1;
            }
            else if (current_scancode & 0x80)
            {
                key_pressed = 0;
            }
            last_scancode = current_scancode;
        }
    }
}
