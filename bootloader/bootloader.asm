[org 0x7c00]
BITS 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov [boot_drive], dl

    ; Print message in VGA text mode (0xb8000)
    mov si, hello_msg
    mov ax, 0xb800
    mov es, ax
    xor di, di
.print_loop:
    lodsb
    or al, al
    jz .print_done
    mov ah, 0x07
    stosw
    jmp .print_loop
.print_done:

    ; Simple hardware checks and terminal

    ; Initialize mouse via BIOS Int 33h
    xor ax, ax
    int 0x33
    cmp ax, 0xffff
    jne .no_mouse
    mov si, mouse_ok
    call print_bios
    jmp .after_mouse
.no_mouse:
    mov si, mouse_fail
    call print_bios
.after_mouse:

    ; Inform about Bluetooth
    mov si, bt_msg
    call print_bios

    ; Basic terminal: wait for Enter, echo keys
    mov si, prompt_msg
    call print_bios
    mov di, input_buf
.read_loop:
    mov ah, 0
    int 0x16
    cmp al, 0x0d
    je .read_done
    stosb
    mov ah, 0x0e
    mov bh, 0
    mov bl, 0x07
    int 0x10
    jmp .read_loop
.read_done:

    ; Enable A20 via port 0x92
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Setup disk address packet
    mov byte [dap], 0x10        ; size of packet
    mov byte [dap+1], 0         ; reserved
    mov word [dap+2], 32        ; number of sectors to read
    mov dword [dap+4], KERNEL_LOAD_ADDR ; load address
    mov dword [dap+8], 1        ; starting LBA (skip boot sector)
    mov dword [dap+12], 0       ; upper part of LBA

    mov si, dap
    mov ah, 0x42
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; Setup GDT for protected mode
    lgdt [gdt_desc]

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEL:protected

[bits 32]
protected:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9fc00

    jmp KERNEL_LOAD_ADDR

[bits 16]
disk_error:
    cli
.halt: hlt
    jmp .halt

hello_msg db 'Hello OS',0
mouse_ok db 0x0d,0x0a,'Mouse detected',0
mouse_fail db 0x0d,0x0a,'No mouse found',0
bt_msg db 0x0d,0x0a,'Bluetooth requires OS driver',0
prompt_msg db 0x0d,0x0a,'Type then press Enter to boot: ',0
input_buf times 64 db 0
boot_drive db 0

KERNEL_LOAD_ADDR equ 0x00100000
CODE_SEL equ 0x08
DATA_SEL equ 0x10

align 4

dap:
    times 16 db 0

align 4

gdt_start:
    dq 0x0000000000000000     ; null
    dq 0x00cf9a000000ffff     ; code
    dq 0x00cf92000000ffff     ; data
gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

print_bios:
    pusha
.next:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    mov bh, 0
    mov bl, 0x07
    int 0x10
    jmp .next
.done:
    popa
    ret

; Pad and signature
    times 510 - ($ - $$) db 0
    dw 0xAA55
