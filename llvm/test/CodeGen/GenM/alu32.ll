; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

; These tests are each targeted at a particular RISC-V ALU instruction. Other
; files in this folder exercise LLVM IR instructions that don't directly match a
; RISC-V instruction

; Register-immediate instructions

define i32 @addi(i32 %a) nounwind {
; CHECK-LABEL: addi:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $1, 1
; CHECK-NEXT:    add.i32 $3, $2, $1
; CHECK-NEXT:    ret
  %1 = add i32 %a, 1
  ret i32 %1
}

define i32 @xori(i32 %a) nounwind {
; CHECK-LABEL: xori:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $1, 2
; CHECK-NEXT:    xor.i32 $3, $2, $1
; CHECK-NEXT:    ret
  %1 = xor i32 %a, 2
  ret i32 %1
}

define i32 @ori(i32 %a) nounwind {
; CHECK-LABEL: ori:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $1, 3
; CHECK-NEXT:    or.i32 $3, $2, $1
; CHECK-NEXT:    ret
  %1 = or i32 %a, 3
  ret i32 %1
}

define i32 @andi(i32 %a) nounwind {
; CHECK-LABEL: andi:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $1, 4
; CHECK-NEXT:    and.i32 $3, $2, $1
; CHECK-NEXT:    ret
  %1 = and i32 %a, 4
  ret i32 %1
}
