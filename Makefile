# Tools
AS = nasm
CC = gcc
LD = ld

# Flags
ASFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -nostdlib -I src/include -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Paths
BOOT_SRC = boot/boot.asm
KERNEL_SRC = src/kernel.c
VGA_DRIVER_SRC = src/vga_driver.c
INPUT_DRIVER_SRC = src/input_driver.c

BOOT_OBJ = obj/boot.o
KERNEL_OBJ = obj/kernel.o
VGA_DRIVER_OBJ = obj/vga_driver.o
INPUT_DRIVER_OBJ = obj/input_driver.o
KERNEL_BIN = bin/kernel.bin

# Create directories if they don't exist
$(shell mkdir -p obj bin)

all: $(KERNEL_BIN)

$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(VGA_DRIVER_OBJ): $(VGA_DRIVER_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUT_DRIVER_OBJ): $(INPUT_DRIVER_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(VGA_DRIVER_OBJ) $(INPUT_DRIVER_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

run: $(KERNEL_BIN)
	qemu-system-i386 -kernel $(KERNEL_BIN)

clean:
	rm -rf obj/* bin/*

.PHONY: all clean run