; bootloader.asm - prints "Hello OS" and loads a kernel at 0x100000
; Assemble with: nasm -f bin bootloader.asm -o bootloader.bin

[BITS 16]
[ORG 0x7C00]

start:
    ; preserve boot drive
    mov [BOOT_DRIVE], dl

    ; set text mode 80x25
    mov ax, 0x0003
    int 0x10

    ; print greeting
    mov si, hello_msg
.print:
    lodsb
    or al, al
    jz .done_print
    mov ah, 0x0E
    mov bx, 0x0007    ; page 0, attribute 7
    int 0x10
    jmp .print
.done_print:

    ; enable A20 so we can access >1MB
    call enable_a20

    ; load kernel to 0x100000 using BIOS
    mov bx, 0x0010      ; offset
    mov es, 0xFFFF      ; segment => 0xFFFF0 + 0x10 = 0x100000
    mov ah, 0x02        ; BIOS read sectors
    mov al, 32          ; number of sectors to read (adjust as needed)
    mov ch, 0
    mov dh, 0
    mov cl, 2           ; start reading from sector 2
    mov dl, [BOOT_DRIVE]
    int 0x13
    jc disk_error

    ; jump to loaded kernel
    jmp 0xFFFF:0x0010

disk_error:
    mov si, err_msg
.print_err:
    lodsb
    or al, al
    jz .hang
    mov ah, 0x0E
    mov bx, 0x0004
    int 0x10
    jmp .print_err
.hang:
    cli
    hlt
    jmp .hang

; ---------------------
; enable A20 gate via keyboard controller
; ---------------------
enable_a20:
    in al, 0x64
.wait1:
    test al, 2
    jnz .wait1
    mov al, 0xAD
    out 0x64, al
.wait2:
    in al, 0x64
    test al, 2
    jnz .wait2
    mov al, 0xD0
    out 0x64, al
.wait3:
    in al, 0x64
    test al, 1
    jz .wait3
    in al, 0x60
    push ax
.wait4:
    in al, 0x64
    test al, 2
    jnz .wait4
    mov al, 0xD1
    out 0x64, al
.wait5:
    in al, 0x64
    test al, 2
    jnz .wait5
    pop ax
    or al, 2
    out 0x60, al
.wait6:
    in al, 0x64
    test al, 2
    jnz .wait6
    mov al, 0xAE
    out 0x64, al
.wait7:
    in al, 0x64
    test al, 2
    jnz .wait7
    ret

hello_msg db 'Hello OS',0
err_msg   db 'Disk read error',0
BOOT_DRIVE db 0

times 510-($-$$) db 0
DW 0xAA55
