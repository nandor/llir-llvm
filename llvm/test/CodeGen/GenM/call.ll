; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s


declare i32 @fn_i32_i32_i32(i32, i32)
declare void @fn_void()

; CHECK-LABEL: _call_i32_i32_i32:
; CHECK-NEXT:  .args 1
; CHECK-NEXT:  .call c
; CHECK-NEXT:  # %bb.0:
; CHECK-NEXT:  arg.i32 $1, 0
; CHECK-NEXT:  addr.i64 $2, _fn_i32_i32_i32@FUNCTION
; CHECK-NEXT:  call.i32  $3, c, $2, $1, $1
; CHECK-NEXT:  ret.i32 $3
define i32 @call_i32_i32_i32(i32 %a) {
  %r = call i32 @fn_i32_i32_i32(i32 %a, i32 %a)
  ret i32 %r
}

; CHECK-LABEL: _call_void:
; CHECK-NEXT:  .args 1
; CHECK-NEXT:  .call c
; CHECK-NEXT:  # %bb.0:
; CHECK-NEXT:  addr.i64 $1, _fn_void@FUNCTION
; CHECK-NEXT:  call c, $1
; CHECK-NEXT:  ret
define void @call_void(i32 %a) {
  call void @fn_void()
  ret void
}
