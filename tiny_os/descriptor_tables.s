global load_gdt      ; make the label load_gdt visible outside this file

section .text
load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush_cs
    ret

.flush_cs:
    ret