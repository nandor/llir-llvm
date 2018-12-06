; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s


%struct.compare_stack = type { [8 x i32], i32* }


; CHECK-LABEL: compare_val
define i64 @compare_val(i64 %v1)  {
entry:
  %stk = alloca %struct.compare_stack, align 8
  %arraydecay = getelementptr inbounds %struct.compare_stack, %struct.compare_stack* %stk, i64 0, i32 0, i64 0
  br label %while

while:
  %sp.0.ph = phi i32* [ %spec, %while ], [ %arraydecay, %entry ]
  %x4 = load i32, i32* %sp.0.ph, align 4
  %cmp5 = icmp eq i32 %x4, 0
  %spec = select i1 %cmp5, i32* %sp.0.ph, i32* %sp.0.ph
  br label %while
}
