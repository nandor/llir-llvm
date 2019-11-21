; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

declare void @ext_func(i64* %ptr)
declare void @ext_func_i32(i32* %ptr)


; CHECK-LABEL: alloca32:
; CHECK-NEXT: .stack_object 0, 4, 4
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0
define void @alloca32() noredzone {
  ; CHECK-NEXT: mov.i32 $1, 0
  ; CHECK-NEXT: frame.i64 $2, 0, 0
  ; CHECK-NEXT: st.4 [$2], $1
  ; CHECK-NEXT: ret
  %retval = alloca i32
  store i32 0, i32* %retval
  ret void
}

; CHECK-LABEL: alloca3264:
; CHECK-NEXT: .stack_object 0, 4, 4
; CHECK-NEXT: .stack_object 1, 8, 8
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0
define void @alloca3264() {
  ; CHECK: frame.i64 $3, 1, 0
  %r1 = alloca i32
  ; CHECK: frame.i64 $4, 0, 0
  %r2 = alloca double
  store i32 0, i32* %r1
  store double 0.0, double* %r2
  ret void
}

; CHECK-LABEL: allocarray:
; CHECK-NEXT: .stack_object 0, 132, 4
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0
define void @allocarray() {
  %r = alloca [33 x i32]
  %p = getelementptr [33 x i32], [33 x i32]* %r, i32 0, i32 0
  store i32 1, i32* %p
  %p2 = getelementptr [33 x i32], [33 x i32]* %r, i32 0, i32 3
  store i32 1, i32* %p2
  ret void
}

; CHECK-LABEL: non_mem_use:
; CHECK-NEXT: .stack_object 0, 27, 16
; CHECK-NEXT: .stack_object 1, 8, 8
; CHECK-NEXT: .stack_object 2, 8, 8
; CHECK-NEXT: .args 0, i64
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0
define void @non_mem_use(i8** %addr) {
  %buf = alloca [27 x i8], align 16
  %r = alloca i64
  %r2 = alloca i64
  call void @ext_func(i64* %r)
  call void @ext_func(i64* %r2)
  %gep = getelementptr inbounds [27 x i8], [27 x i8]* %buf, i32 0, i32 0
  store i8* %gep, i8** %addr
  ret void
}

; CHECK-LABEL: allocarray_inbounds:
; CHECK-NEXT: .stack_object 0, 20, 4
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0
define void @allocarray_inbounds() {
  %r = alloca [5 x i32]
  %p = getelementptr inbounds [5 x i32], [5 x i32]* %r, i32 0, i32 0
  store i32 1, i32* %p
  %p2 = getelementptr inbounds [5 x i32], [5 x i32]* %r, i32 0, i32 3
  store i32 1, i32* %p2
  call void @ext_func(i64* null)
  ret void
}
