#include "input_driver.h"

// Fungsi untuk membaca dari port I/O
unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

// Fungsi untuk menulis ke port I/O
void outb(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

// Tabel konversi scan code ke ASCII (set 1, hanya untuk tekan tombol)
// Menambahkan spasi untuk scancode 0x39 (spacebar)
const unsigned char scancode_set1[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', // Spasi ditambahkan di posisi 0x39
    // Catatan: Tabel ini hanya mencakup scancode 0x00 sampai 0x39, perlu diperluas jika ingin semua tombol
};

// Handler untuk interrupt keyboard
void keyboard_handler() {
    unsigned char scancode = inb(0x60);
    if (scancode < sizeof(scancode_set1)) {
        char character = scancode_set1[scancode];
        if (character) {
            // Placeholder untuk memproses scancode
            // Misalnya, tulis ke buffer atau tampilkan di layar
        }
    }
}

// Inisialisasi keyboard (konfigurasi dasar untuk IRQ 1)
void init_keyboard() {
    // Aktifkan interrupt untuk IRQ 1 di PIC master
    outb(0x21, inb(0x21) & ~0x02); // Aktifkan IRQ 1
}