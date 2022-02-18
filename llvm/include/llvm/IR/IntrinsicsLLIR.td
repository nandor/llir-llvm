//===- IntrinsicsAARCH64.td - Defines AARCH64 intrinsics ---*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines all of the AARCH64-specific intrinsics.
//
//===----------------------------------------------------------------------===//

let TargetPrefix = "llir" in {

def int_llir_ldxr : Intrinsic<[llvm_anyint_ty], [llvm_anyptr_ty]>;
def int_llir_ldaxr : Intrinsic<[llvm_anyint_ty], [llvm_anyptr_ty]>;
def int_llir_stxr : Intrinsic<[llvm_i32_ty], [llvm_anyint_ty, llvm_anyptr_ty]>;
def int_llir_stlxr : Intrinsic<[llvm_i32_ty], [llvm_anyint_ty, llvm_anyptr_ty]>;

def int_llir_landing_pad_x86_64 : Intrinsic<[llvm_ptr_ty, llvm_i32_ty], []>;

}
