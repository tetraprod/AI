# Simple Bootloader

This 512-byte boot sector prints `Hello OS` using VGA text mode, enables the A20
line, loads the first 32 sectors after the bootloader into memory at `0x00100000`,
switches to 32-bit protected mode, and jumps to the loaded kernel. It targets
IBM PC compatible machines and relies on BIOS services for disk I/O.

## Build

Install NASM and run:

```bash
nasm -f bin bootloader.asm -o bootloader.bin
```

The resulting `bootloader.bin` can be written to the first sector of a disk
image. To try it in an emulator such as QEMU:

```bash
qemu-system-i386 -drive format=raw,file=bootloader.bin
```
