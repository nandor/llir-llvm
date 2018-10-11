; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

; Register-immediate instructions

define i32 @addi(i32 %a) nounwind {
; CHECK-LABEL: addi:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $0, 1
; CHECK-NEXT:    arg.i32 $1, 0
; CHECK-NEXT:    add.i32 $2, $1, $0
; CHECK-NEXT:    ret.i32 $2
  %1 = add i32 %a, 1
  ret i32 %1
}

define i32 @xori(i32 %a) nounwind {
; CHECK-LABEL: xori:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $0, 2
; CHECK-NEXT:    arg.i32 $1, 0
; CHECK-NEXT:    xor.i32 $2, $1, $0
; CHECK-NEXT:    ret.i32 $2
  %1 = xor i32 %a, 2
  ret i32 %1
}

define i32 @ori(i32 %a) nounwind {
; CHECK-LABEL: ori:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $0, 3
; CHECK-NEXT:    arg.i32 $1, 0
; CHECK-NEXT:    or.i32 $2, $1, $0
; CHECK-NEXT:    ret.i32 $2
  %1 = or i32 %a, 3
  ret i32 %1
}

define i32 @andi(i32 %a) nounwind {
; CHECK-LABEL: andi:
; CHECK:       # %bb.0:
; CHECK-NEXT:    imm.i32 $0, 4
; CHECK-NEXT:    arg.i32 $1, 0
; CHECK-NEXT:    and.i32 $2, $1, $0
; CHECK-NEXT:    ret.i32 $2
  %1 = and i32 %a, 4
  ret i32 %1
}
