; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s -check-prefix=GENM

; These tests are each targeted at a particular RISC-V ALU instruction. Other
; files in this folder exercise LLVM IR instructions that don't directly match a
; RISC-V instruction

; Register-immediate instructions

define i32 @addi(i32 %a) nounwind {
; GENM-LABEL: addi:
; GENM:       # %bb.0:
; GENM-NEXT:    addi a0, a0, 1
; GENM-NEXT:    ret
  %1 = add i32 %a, 1
  ret i32 %1
}

define i32 @slti(i32 %a) nounwind {
; GENM-LABEL: slti:
; GENM:       # %bb.0:
; GENM-NEXT:    slti a0, a0, 2
; GENM-NEXT:    ret
  %1 = icmp slt i32 %a, 2
  %2 = zext i1 %1 to i32
  ret i32 %2
}

define i32 @sltiu(i32 %a) nounwind {
; GENM-LABEL: sltiu:
; GENM:       # %bb.0:
; GENM-NEXT:    sltiu a0, a0, 3
; GENM-NEXT:    ret
  %1 = icmp ult i32 %a, 3
  %2 = zext i1 %1 to i32
  ret i32 %2
}

define i32 @xori(i32 %a) nounwind {
; GENM-LABEL: xori:
; GENM:       # %bb.0:
; GENM-NEXT:    xori a0, a0, 4
; GENM-NEXT:    ret
  %1 = xor i32 %a, 4
  ret i32 %1
}

define i32 @ori(i32 %a) nounwind {
; GENM-LABEL: ori:
; GENM:       # %bb.0:
; GENM-NEXT:    ori a0, a0, 5
; GENM-NEXT:    ret
  %1 = or i32 %a, 5
  ret i32 %1
}

define i32 @andi(i32 %a) nounwind {
; GENM-LABEL: andi:
; GENM:       # %bb.0:
; GENM-NEXT:    andi a0, a0, 6
; GENM-NEXT:    ret
  %1 = and i32 %a, 6
  ret i32 %1
}

define i32 @slli(i32 %a) nounwind {
; GENM-LABEL: slli:
; GENM:       # %bb.0:
; GENM-NEXT:    slli a0, a0, 7
; GENM-NEXT:    ret
  %1 = shl i32 %a, 7
  ret i32 %1
}

define i32 @srli(i32 %a) nounwind {
; GENM-LABEL: srli:
; GENM:       # %bb.0:
; GENM-NEXT:    srli a0, a0, 8
; GENM-NEXT:    ret
  %1 = lshr i32 %a, 8
  ret i32 %1
}

define i32 @srai(i32 %a) nounwind {
; GENM-LABEL: srai:
; GENM:       # %bb.0:
; GENM-NEXT:    srai a0, a0, 9
; GENM-NEXT:    ret
  %1 = ashr i32 %a, 9
  ret i32 %1
}

; Register-register instructions

define i32 @add(i32 %a, i32 %b) nounwind {
; GENM-LABEL: add:
; GENM:       # %bb.0:
; GENM-NEXT:    add a0, a0, a1
; GENM-NEXT:    ret
  %1 = add i32 %a, %b
  ret i32 %1
}

define i32 @sub(i32 %a, i32 %b) nounwind {
; GENM-LABEL: sub:
; GENM:       # %bb.0:
; GENM-NEXT:    sub a0, a0, a1
; GENM-NEXT:    ret
  %1 = sub i32 %a, %b
  ret i32 %1
}

define i32 @sll(i32 %a, i32 %b) nounwind {
; GENM-LABEL: sll:
; GENM:       # %bb.0:
; GENM-NEXT:    sll a0, a0, a1
; GENM-NEXT:    ret
  %1 = shl i32 %a, %b
  ret i32 %1
}

define i32 @slt(i32 %a, i32 %b) nounwind {
; GENM-LABEL: slt:
; GENM:       # %bb.0:
; GENM-NEXT:    slt a0, a0, a1
; GENM-NEXT:    ret
  %1 = icmp slt i32 %a, %b
  %2 = zext i1 %1 to i32
  ret i32 %2
}

define i32 @sltu(i32 %a, i32 %b) nounwind {
; GENM-LABEL: sltu:
; GENM:       # %bb.0:
; GENM-NEXT:    sltu a0, a0, a1
; GENM-NEXT:    ret
  %1 = icmp ult i32 %a, %b
  %2 = zext i1 %1 to i32
  ret i32 %2
}

define i32 @xor(i32 %a, i32 %b) nounwind {
; GENM-LABEL: xor:
; GENM:       # %bb.0:
; GENM-NEXT:    xor a0, a0, a1
; GENM-NEXT:    ret
  %1 = xor i32 %a, %b
  ret i32 %1
}

define i32 @srl(i32 %a, i32 %b) nounwind {
; GENM-LABEL: srl:
; GENM:       # %bb.0:
; GENM-NEXT:    srl a0, a0, a1
; GENM-NEXT:    ret
  %1 = lshr i32 %a, %b
  ret i32 %1
}

define i32 @sra(i32 %a, i32 %b) nounwind {
; GENM-LABEL: sra:
; GENM:       # %bb.0:
; GENM-NEXT:    sra a0, a0, a1
; GENM-NEXT:    ret
  %1 = ashr i32 %a, %b
  ret i32 %1
}

define i32 @or(i32 %a, i32 %b) nounwind {
; GENM-LABEL: or:
; GENM:       # %bb.0:
; GENM-NEXT:    or a0, a0, a1
; GENM-NEXT:    ret
  %1 = or i32 %a, %b
  ret i32 %1
}

define i32 @and(i32 %a, i32 %b) nounwind {
; GENM-LABEL: and:
; GENM:       # %bb.0:
; GENM-NEXT:    and a0, a0, a1
; GENM-NEXT:    ret
  %1 = and i32 %a, %b
  ret i32 %1
}
