; RUN: llc -mtriple=llir -verify-machineinstrs < %s | FileCheck %s

declare void @ext_func(i64* %ptr)
declare void @ext_func_i32(i32* %ptr)
declare void @use_i8_star(i8*)
declare i8* @llvm.frameaddress(i32)

; CHECK-LABEL: dynamic_alloca:
; CHECK-NEXT: .args 0, i32
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @dynamic_alloca(i32 %alloc) {
 %r = alloca i32, i32 %alloc
 call void @ext_func_i32(i32* %r)
 ret void
}
; CHECK: .end

; CHECK-LABEL: dynamic_alloca_redzone:
; CHECK-NEXT: .args 0, i32
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @dynamic_alloca_redzone(i32 %alloc) {
 %r = alloca i32, i32 %alloc
 store i32 0, i32* %r
 ret void
}
; CHECK: .end

; CHECK-LABEL: dynamic_static_alloca:
; CHECK-NEXT: .stack_object 0, 4, 4
; CHECK-NEXT: .args 0, i32
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @dynamic_static_alloca(i32 %alloc) noredzone {
 %static = alloca i32
 store volatile i32 101, i32* %static
 %dynamic = alloca i32, i32 %alloc
 store volatile i32 102, i32* %static
 store volatile i32 103, i32* %dynamic
 %dynamic.2 = alloca i32, i32 %alloc
 store volatile i32 104, i32* %static
 store volatile i32 105, i32* %dynamic
 store volatile i32 106, i32* %dynamic.2
 ret void
}
; CHECK: .end


declare i8* @llvm.stacksave()
declare void @llvm.stackrestore(i8*)

; CHECK-LABEL: llvm_stack_builtins:
; CHECK-NEXT: .args 0, i32
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @llvm_stack_builtins(i32 %alloc) noredzone {
 %stack = call i8* @llvm.stacksave()
 %dynamic = alloca i32, i32 %alloc
 call void @llvm.stackrestore(i8* %stack)
 ret void
}
; CHECK: .end

; CHECK-LABEL: dynamic_alloca_nouse:
; CHECK-NEXT: .args 0, i32
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @dynamic_alloca_nouse(i32 %alloc) noredzone {
 %dynamic = alloca i32, i32 %alloc
 ret void
}
; CHECK: .end

; CHECK-LABEL: copytoreg_fi:
; CHECK-NEXT: .stack_object 0, 4, 4
; CHECK-NEXT: .args 0, i32, i64
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @copytoreg_fi(i1 %cond, i32* %b) {
entry:
 %addr = alloca i32
 br label %body
body:
 %a = phi i32* [%addr, %entry], [%b, %body]
 store i32 1, i32* %a
 br i1 %cond, label %body, label %exit
exit:
 ret void
}
; CHECK: .end


; CHECK-LABEL: frameaddress_0:
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @frameaddress_0() {
  %t = call i8* @llvm.frameaddress(i32 0)
  call void @use_i8_star(i8* %t)
  ret void
}
; CHECK: .end

; CHECK-LABEL: frameaddress_1:
; CHECK-NEXT: .call c
; CHECK-NEXT: # %bb.0:
define void @frameaddress_1() {
  %t = call i8* @llvm.frameaddress(i32 1)
  call void @use_i8_star(i8* %t)
  ret void
}
; CHECK: .end

