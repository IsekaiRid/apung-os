[BITS 32]             ; Kita bekerja di Protected Mode (32-bit)

section .text
    align 4           ; Multiboot header harus 4-byte aligned

    ; === Multiboot Header ===
    dd 0x1BADB002      ; magic number
    dd 0x00000000      ; flags (0 = tidak minta apa-apa)
    dd -(0x1BADB002 + 0x00000000) ; checksum = -(magic + flags)

global start
extern kernel_main

start:
    cli               ; Disable interrupt (keamanan)
    call kernel_main  ; Panggil fungsi utama dari C
.hang:
    hlt               ; Hentikan CPU
    jmp .hang         ; Loop selamanya agar tidak return
