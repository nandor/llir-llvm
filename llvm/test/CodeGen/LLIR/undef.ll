; RUN: llc -mtriple=llir -verify-machineinstrs < %s | FileCheck %s

declare void @ext_func(i64 %val)


; CHECK-LABEL: call_undef:
; CHECK-NEXT:  .call c
; CHECK-NEXT: # %bb.0
; CHECK: undef.i64 $1
; CHECK: call.c $2, $1
define void @call_undef() {
  call void @ext_func(i64 undef)
  ret void
}

; CHECK-LABEL: ret_undef_i64:
; CHECK-NEXT:  .call c
; CHECK-NEXT: # %bb.0
; CHECK: undef.i64 $1
; CHECK: ret $1
define i64 @ret_undef_i64() {
  ret i64 undef
}

; CHECK-LABEL: ret_undef_i32:
; CHECK-NEXT:  .call c
; CHECK-NEXT: # %bb.0
; CHECK: undef.i32 $1
; CHECK: ret $1
define i32 @ret_undef_i32() {
  ret i32 undef
}
