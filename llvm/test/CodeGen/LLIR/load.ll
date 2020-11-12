; RUN: llc -mtriple=llir_x86_64 -verify-machineinstrs < %s | FileCheck %s

attributes #4 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

declare void @caml_array_bound_error() local_unnamed_addr #5

; CHECK: caml_string_get16:
define i64 @caml_string_get16(i64 %str, i64 %index) local_unnamed_addr #4 {
entry:
  %shr = ashr i64 %index, 1
  %cmp = icmp slt i64 %index, 0
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %entry
  %add = add nsw i64 %shr, 1
  %0 = inttoptr i64 %str to i64*
  %arrayidx.i = getelementptr i64, i64* %0, i64 -1
  %1 = load i64, i64* %arrayidx.i, align 8
  %shr.i = lshr i64 %1, 10
  %mul.i = shl nuw nsw i64 %shr.i, 3
  %sub.i = add nsw i64 %mul.i, -1
  %2 = inttoptr i64 %str to i8*
  %arrayidx1.i = getelementptr i8, i8* %2, i64 %sub.i
  %3 = load i8, i8* %arrayidx1.i, align 1
  %conv.i = sext i8 %3 to i64
  %sub2.i = sub nsw i64 %sub.i, %conv.i
  %cmp1 = icmp ult i64 %add, %sub2.i
  br i1 %cmp1, label %if.end, label %if.then

if.then:                                          ; preds = %lor.lhs.false, %entry
  tail call void @caml_array_bound_error() #11
  unreachable

if.end:                                           ; preds = %lor.lhs.false
  %arrayidx = getelementptr i8, i8* %2, i64 %shr
  %4 = load i8, i8* %arrayidx, align 1
  %arrayidx3 = getelementptr i8, i8* %2, i64 %add
  %5 = load i8, i8* %arrayidx3, align 1
  %conv = zext i8 %5 to i64
  %shl = shl nuw nsw i64 %conv, 8
  %conv4 = zext i8 %4 to i64
  %or = or i64 %shl, %conv4
  %shl6 = shl nuw nsw i64 %or, 1
  %add7 = or i64 %shl6, 1
  ret i64 %add7
}
; CHECK: .end
