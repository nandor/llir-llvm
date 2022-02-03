//===-- LLIRInstrAArch64.td - LLIR AArch64 specific instructions ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the LLIR instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

let Predicates = [IsAArch64] in {

multiclass AtomicStore<SDNode node, string name, bits<32> Op>
{
  def _I8 : I
    < (outs I32:$dst)
    , (ins I8:$val, I64:$addr)
    , [(set I32:$dst, (node I8:$val, I64:$addr))]
    , !strconcat(name, ".i32\t$dst, $addr, $val")
    , 0xE1
    >;

  def _I16 : I
    < (outs I32:$dst)
    , (ins I16:$val, I64:$addr)
    , [(set I32:$dst, (node I16:$val, I64:$addr))]
    , !strconcat(name, ".i32\t$dst, $addr, $val")
    , 0xE1
    >;

  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$val, I64:$addr)
    , [(set I32:$dst, (node I32:$val, I64:$addr))]
    , !strconcat(name, ".i32\t$dst, $addr, $val")
    , 0xE1
    >;

  def _I64 : I
    < (outs I32:$dst)
    , (ins I64:$val, I64:$addr)
    , [(set I32:$dst, (node I64:$val, I64:$addr))]
    , !strconcat(name, ".i32\t$dst, $addr, $val")
    , 0xE1
    >;
}

multiclass AtomicLoad<SDNode node, string name, bits<32> Op>
{
  def _I8 : I
    < (outs I8:$dst)
    , (ins I64:$addr)
    , [(set I8:$dst, (node I64:$addr))]
    , !strconcat(name, ".i8\t$dst, $addr")
    , 0xE1
    >;

  def _I16 : I
    < (outs I16:$dst)
    , (ins I64:$addr)
    , [(set I16:$dst, (node I64:$addr))]
    , !strconcat(name, ".i16\t$dst, $addr")
    , 0xE1
    >;

  def _I32 : I
    < (outs I32:$dst)
    , (ins I64:$addr)
    , [(set I32:$dst, (node I64:$addr))]
    , !strconcat(name, ".i32\t$dst, $addr")
    , 0xE1
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$addr)
    , [(set I64:$dst, (node I64:$addr))]
    , !strconcat(name, ".i64\t$dst, $addr")
    , 0xE1
    >;
}

defm AARCH64_ATOMIC_STLXR_8 : AtomicStore<int_llir_stlxr, "aarch64_store_cond", 0xE1>;
defm AARCH64_ATOMIC_STXR_8  : AtomicStore<int_llir_stxr, "aarch64_store_cond", 0xE1>;
defm AARCH64_ATOMIC_LDLXR_8 : AtomicLoad<int_llir_ldaxr, "aarch64_load_link", 0xE1>;
defm AARCH64_ATOMIC_LDXR_8  : AtomicLoad<int_llir_ldxr, "aarch64_load_link", 0xE1>;

// Very incorrect.
def : Pat<(releasing_store<atomic_store_8>  I64:$ptr,  I8:$val), (ST_1_I8_I64  I64:$ptr,  I8:$val)>;
def : Pat<(monotonic_store<atomic_store_8>  I64:$ptr,  I8:$val), (ST_1_I8_I64  I64:$ptr,  I8:$val)>;
def : Pat<(atomic_store_8                   I64:$ptr,  I8:$val), (ST_1_I8_I64  I64:$ptr,  I8:$val)>;
def : Pat<(releasing_store<atomic_store_16> I64:$ptr, I16:$val), (ST_2_I16_I64 I64:$ptr, I16:$val)>;
def : Pat<(monotonic_store<atomic_store_16> I64:$ptr, I16:$val), (ST_2_I16_I64 I64:$ptr, I16:$val)>;
def : Pat<(atomic_store_16                  I64:$ptr, I16:$val), (ST_2_I16_I64 I64:$ptr, I16:$val)>;
def : Pat<(releasing_store<atomic_store_32> I64:$ptr, I32:$val), (ST_4_I32_I64 I64:$ptr, I32:$val)>;
def : Pat<(monotonic_store<atomic_store_32> I64:$ptr, I32:$val), (ST_4_I32_I64 I64:$ptr, I32:$val)>;
def : Pat<(atomic_store_32                  I64:$ptr, I32:$val), (ST_4_I32_I64 I64:$ptr, I32:$val)>;

def AARCH_DMB : I <(outs), (ins), [(LLIRMFence)], "aarch64_dmb", 0xE1>;
}
