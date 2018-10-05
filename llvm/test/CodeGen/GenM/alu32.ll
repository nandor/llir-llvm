; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s -check-prefix=GENM

; These tests are each targeted at a particular RISC-V ALU instruction. Other
; files in this folder exercise LLVM IR instructions that don't directly match a
; RISC-V instruction

; Register-immediate instructions

define i32 @addi(i32 %a) nounwind {
; GENM-LABEL: addi:
; GENM:       # %bb.0:
; GENM-NEXT:    imm.i32 $1, 1
; GENM-NEXT:    add.i32 $3, $2, $1
; GENM-NEXT:    ret
  %1 = add i32 %a, 1
  ret i32 %1
}

define i32 @xori(i32 %a) nounwind {
; GENM-LABEL: xori:
; GENM:       # %bb.0:
; GENM-NEXT:    imm.i32 $1, 2
; GENM-NEXT:    xor.i32 $3, $2, $1
; GENM-NEXT:    ret
  %1 = xor i32 %a, 2
  ret i32 %1
}

define i32 @ori(i32 %a) nounwind {
; GENM-LABEL: ori:
; GENM:       # %bb.0:
; GENM-NEXT:    imm.i32 $1, 3
; GENM-NEXT:    or.i32 $3, $2, $1
; GENM-NEXT:    ret
  %1 = or i32 %a, 3
  ret i32 %1
}

define i32 @andi(i32 %a) nounwind {
; GENM-LABEL: andi:
; GENM:       # %bb.0:
; GENM-NEXT:    imm.i32 $1, 4
; GENM-NEXT:    and.i32 $3, $2, $1
; GENM-NEXT:    ret
  %1 = and i32 %a, 4
  ret i32 %1
}
