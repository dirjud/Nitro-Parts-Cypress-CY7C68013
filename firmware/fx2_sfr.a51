; 

    .globl _asm_get_fx2_sfr, _asm_set_fx2_sfr, _asm_set_fx2_sfr_PARM_2


    .area RSEG (DATA)
IOE = 0xB1

; FUNCTION asm_set_fx2_sfr( BYTE addr, BYTE val)
; addr is in dpl
; val is PARM_2
    .area OSEG
_asm_set_fx2_sfr_PARM_2:
    .ds 1
    .area CSEG
_asm_set_fx2_sfr:
    mov a, dpl
    mov dptr, #(set_fx2_sfr_addr+2)
    movx @dptr, a
set_fx2_sfr_addr:
    mov IOE, _asm_set_fx2_sfr_PARM_2 ; IOE replaced by actual sfr value
    ret



; FUNCTION asm_get_fx2_sfr( BYTE addr )
; addr is in dpl
; ret value also dpl
    .area CSEG
_asm_get_fx2_sfr:
    mov a, dpl ; a now has the addr
    mov dptr, #(get_fx2_sfr_addr+1)
    movx @dptr, a ; code has been changed to execute sfr
get_fx2_sfr_addr:
    mov dpl, IOE
    ret
    
    
