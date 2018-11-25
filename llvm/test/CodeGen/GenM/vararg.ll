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

; CHECK-LABEL: vararg:
define double @vararg(i32, ...) local_unnamed_addr #0 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  %3 = bitcast [1 x %struct.__va_list_tag]* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %3) #2
  call void @llvm.va_start(i8* nonnull %3)
  %4 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %2, i64 0, i64 0, i32 1
  %5 = load i32, i32* %4, align 4
  %6 = icmp ult i32 %5, 161
  br i1 %6, label %7, label %13

  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %2, i64 0, i64 0, i32 3
  %9 = load i8*, i8** %8, align 16
  %10 = sext i32 %5 to i64
  %11 = getelementptr i8, i8* %9, i64 %10
  %12 = add i32 %5, 16
  store i32 %12, i32* %4, align 4
  br label %17

  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %2, i64 0, i64 0, i32 2
  %15 = load i8*, i8** %14, align 8
  %16 = getelementptr i8, i8* %15, i64 8
  store i8* %16, i8** %14, align 8
  br label %17

  %18 = phi i8* [ %11, %7 ], [ %15, %13 ]
  %19 = bitcast i8* %18 to double*
  %20 = load double, double* %19, align 8
  call void @llvm.va_end(i8* nonnull %3)
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %3) #2
  ret double %20
}


declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2
declare void @llvm.va_start(i8*) #3
declare i32  @vfprintf(%struct.__sFILE* nocapture, i8* nocapture readonly, %struct.__va_list_tag*) local_unnamed_addr #3
declare void @llvm.va_end(i8*) #3
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

attributes #0 = { nounwind uwtable }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }
