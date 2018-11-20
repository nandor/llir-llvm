; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

%struct.__sFILE = type opaque
%struct.__va_list_tag = type { i32, i32, i8*, i8* }
@__stderrp = external local_unnamed_addr global %struct.__sFILE*, align 8


; CHECK-LABEL: va_func:
; CHECK: .args 2, 1
; CHECK call.i32.c $6, $5, $3, $4, $sp
define void @va_func(i32, i8* nocapture readonly, ...) {
  %3 = alloca [1 x %struct.__va_list_tag], align 16
  %4 = bitcast [1 x %struct.__va_list_tag]* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %4) #3
  %5 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %3, i64 0, i64 0
  call void @llvm.va_start(i8* nonnull %4)
  %6 = load %struct.__sFILE*, %struct.__sFILE** @__stderrp, align 8
  call i32 @vfprintf(%struct.__sFILE* %6, i8* %1, %struct.__va_list_tag* nonnull %5)
  call void @llvm.va_end(i8* nonnull %4)
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %4) #3
  ret void
}

declare i32 @varargsfunc(i32 %sum, ...)
declare void @varargvoid(i32 %sum, ...)


; CHECK-LABEL: va_call:
; CHECK:  call.1.c  $5, $4, $3, $2, $1
; CHECK:  call.i32.1.c  $7, $6, $4, $3, $2, $1
define i32 @va_call() {
entry:
  call void (i32, ...) @varargvoid(i32 0, i32 1, i32 2, i32 3)
  call i32 (i32, ...) @varargsfunc(i32 0, i32 1, i32 2, i32 3)
  ret i32 1
}

declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2
declare void @llvm.va_start(i8*) #3
declare i32  @vfprintf(%struct.__sFILE* nocapture, i8* nocapture readonly, %struct.__va_list_tag*) local_unnamed_addr #3
declare void @llvm.va_end(i8*) #3
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

attributes #0 = { nounwind uwtable }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }
