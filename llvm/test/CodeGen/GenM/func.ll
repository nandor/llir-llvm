; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

; Test that basic functions assemble as expected.

target datalayout = "e-m:e-p:32:32-i64:64-n32:64-S128"
target triple = "wasm32-unknown-unknown"

; CHECK-LABEL: _f0:
; CHECK: ret
define void @f0() {
  ret void
}

; CHECK-LABEL: _f1:
; CHECK: imm.i32   $0, 0
; CHECK-NEXT: ret.i32  $0
 define i32 @f1() {
  ret i32 0
}

; CHECK-LABEL: _f2:
; CHECK: imm.i32   $0, 0
; CHECK-NEXT: ret.i32  $0
define i32 @f2(i32 %p1, float %p2) {
  ret i32 0
}

; CHECK-LABEL: _f3:
; CHECK: ret
define void @f3(i32 %p1, float %p2) {
  ret void
}

; CHECK-LABEL: _f4:
; CHECK: jf
; CHECK: jmp
; CHECK: ret.i32
; CHECK: ret.i32
define i32 @f4(i32 %x) {
entry:
   %c = trunc i32 %x to i1
   br i1 %c, label %true, label %false
true:
   ret i32 0
false:
   ret i32 1
}
