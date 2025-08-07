#ifndef INPUT_DRIVER_H
#define INPUT_DRIVER_H

unsigned char inb(unsigned short port);
void keyboard_handler();
void init_keyboard();

// Deklarasi eksternal untuk scancode_set1
extern const unsigned char scancode_set1[];

// Definisikan ukuran scancode_set1 (sesuaikan dengan definisi di input_driver.c)
#define SCANCODES_SIZE 59

#endif