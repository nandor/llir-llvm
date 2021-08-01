//===-- LLIRISelLowering.cpp - LLIR DAG Lowering Implementation -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the interfaces that LLIR uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "LLIRISelLowering.h"

#include "LLIRMachineFunctionInfo.h"
#include "LLIRRegisterInfo.h"
#include "LLIRTargetMachine.h"
#include "LLIRTargetObjectFile.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsX86.h"
#include "llvm/IR/IntrinsicsLLIR.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
using namespace llvm;

static void Fail(const SDLoc &DL, SelectionDAG &DAG, const char *msg) {
  DAG.getContext()->diagnose(DiagnosticInfoUnsupported(
      DAG.getMachineFunction().getFunction(), msg, DL.getDebugLoc()));
}

LLIRTargetLowering::LLIRTargetLowering(const TargetMachine &TM,
                                       const LLIRSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {

  auto MVTPtr = STI.is32Bit() ? MVT::i32 : MVT::i64;

  setBooleanContents(ZeroOrOneBooleanContent);
  setSchedulingPreference(Sched::RegPressure);
  setMaxAtomicSizeInBitsSupported(64);

  addRegisterClass(MVT::i8, &LLIR::I8RegClass);
  addRegisterClass(MVT::i16, &LLIR::I16RegClass);
  addRegisterClass(MVT::i32, &LLIR::I32RegClass);
  addRegisterClass(MVT::i64, &LLIR::I64RegClass);
  addRegisterClass(MVT::f32, &LLIR::F32RegClass);
  addRegisterClass(MVT::f64, &LLIR::F64RegClass);
  if (Subtarget->isX86()) {
    addRegisterClass(MVT::f80, &LLIR::F80RegClass);
  }
  if (Subtarget->isAArch64()) {
    addRegisterClass(MVT::i128, &LLIR::I128RegClass);
    addRegisterClass(MVT::f128, &LLIR::F128RegClass);
  }
  if (Subtarget->isPPC64le()) {
    addRegisterClass(MVT::f128, &LLIR::F128RegClass);
  }
  computeRegisterProperties(Subtarget->getRegisterInfo());

  // Custom lowerings for most operations.
  setOperationAction(ISD::FRAMEADDR, MVTPtr, Custom);
  setOperationAction(ISD::FrameIndex, MVTPtr, Custom);
  setOperationAction(ISD::GlobalAddress, MVTPtr, Custom);
  setOperationAction(ISD::GlobalTLSAddress, MVTPtr, Custom);
  setOperationAction(ISD::ExternalSymbol, MVTPtr, Custom);
  setOperationAction(ISD::JumpTable, MVTPtr, Custom);
  setOperationAction(ISD::BlockAddress, MVTPtr, Custom);
  setOperationAction(ISD::BRIND, MVT::Other, Custom);
  setOperationAction(ISD::BR_JT, MVT::Other, Custom);
  setOperationAction(ISD::CopyToReg, MVT::Other, Custom);
  setOperationAction(ISD::BR_JT, MVT::Other, Custom);

  // Handle alloca.
  setOperationAction(ISD::STACKSAVE, MVT::Other, Expand);
  setOperationAction(ISD::STACKRESTORE, MVT::Other, Expand);
  setOperationAction(ISD::DYNAMIC_STACKALLOC, MVTPtr, Custom);
  setStackPointerRegisterToSaveRestore(LLIR::SP);

  // Handle variable arguments.
  setOperationAction(ISD::VASTART, MVT::Other, Custom);
  setOperationAction(ISD::VAARG, MVT::Other, Custom);
  setOperationAction(ISD::VACOPY, MVT::Other, Custom);
  setOperationAction(ISD::VAEND, MVT::Other, Expand);

  // Deal with floating point operations.
  for (auto T : {MVT::f32, MVT::f64, MVT::f80, MVT::f128}) {
    // Expand conditionals.
    setOperationAction(ISD::BR_CC, T, Expand);
    setOperationAction(ISD::SELECT_CC, T, Expand);

    // Expand operations which are not yet supported.
    setOperationAction(ISD::FMA, T, Expand);
    setOperationAction(ISD::STRICT_FMA, T, Expand);

    // Custom lowerings.
    setOperationAction(ISD::FP_ROUND, T, Custom);
    setOperationAction(ISD::STRICT_FP_ROUND, T, Custom);

    // Decide what to do with funky float operations based on target.
    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86()) {
        Op = Legal;
      } else if (Subtarget->isAArch64()) {
        Op = T != MVT::f128 ? Legal : Custom;
      } else if (Subtarget->isPPC64le()) {
        Op = Legal;
      } else if (Subtarget->isRISCV()) {
        Op = Legal;
      } else {
        llvm_unreachable("invalid subtarget");
      }

      // Decide whether to allow f128 operations.
      setOperationAction(ISD::FP_EXTEND, T, Op);
      setOperationAction(ISD::FADD, T, Op);
      setOperationAction(ISD::STRICT_FADD, T, Op);
      setOperationAction(ISD::FSUB, T, Op);
      setOperationAction(ISD::STRICT_FSUB, T, Op);
      setOperationAction(ISD::FMUL, T, Op);
      setOperationAction(ISD::STRICT_FMUL, T, Op);
      setOperationAction(ISD::FDIV, T, Op);
      setOperationAction(ISD::STRICT_FDIV, T, Op);
      setOperationAction(ISD::FREM, T, Op);
      setOperationAction(ISD::STRICT_FREM, T, Op);
      setOperationAction(ISD::SETCC, T, Op);
      setOperationAction(ISD::STRICT_FSETCC, T, Op);
      setOperationAction(ISD::STRICT_FSETCCS, T, Op);
    }

    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86()) {
        Op = T == MVT::f80 ? Expand : Legal;
      } else if (Subtarget->isAArch64()) {
        Op = Legal;
      } else if (Subtarget->isPPC64le()) {
        Op = Legal;
      } else if (Subtarget->isRISCV()) {
        Op = Legal;
      } else {
        llvm_unreachable("invalid subtarget");
      }
      setOperationAction(ISD::FCEIL, T, Op);
      setOperationAction(ISD::FLOG, T, Op);
      setOperationAction(ISD::FLOG2, T, Op);
      setOperationAction(ISD::FLOG10, T, Op);
      setOperationAction(ISD::STRICT_FCEIL, T, Op);
      setOperationAction(ISD::STRICT_FLOG, T, Op);
      setOperationAction(ISD::STRICT_FLOG2, T, Op);
      setOperationAction(ISD::STRICT_FLOG10, T, Op);
      setOperationAction(ISD::STRICT_FMAXNUM, T, Op);
      setOperationAction(ISD::STRICT_FMINNUM, T, Op);
      setOperationAction(ISD::FMAXNUM, T, Expand);
      setOperationAction(ISD::FMINNUM, T, Expand);
    }

    // Disable some operations on some platforms.
    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86()) {
        Op = Legal;
      } else if (Subtarget->isAArch64()) {
        Op = T != MVT::f128 ? Legal : Custom;
      } else if (Subtarget->isPPC64le()) {
        Op = Legal;
      } else if (Subtarget->isRISCV()) {
        Op = Expand;
      } else {
        llvm_unreachable("invalid subtarget");
      }

      setOperationAction(ISD::FTRUNC, T, Op);
      setOperationAction(ISD::STRICT_FTRUNC, T, Op);
    }
  }

  // Disable some integer operations.
  for (auto T : {MVT::i8, MVT::i16, MVT::i32, MVT::i64, MVT::i128}) {
    // Expand unavailable integer operations.
    setOperationAction(ISD::BSWAP, T, Legal);
    setOperationAction(ISD::SMUL_LOHI, T, Expand);
    setOperationAction(ISD::UMUL_LOHI, T, Expand);
    setOperationAction(ISD::SDIVREM, T, Expand);
    setOperationAction(ISD::UDIVREM, T, Expand);
    setOperationAction(ISD::SHL_PARTS, T, Expand);
    setOperationAction(ISD::SRA_PARTS, T, Expand);
    setOperationAction(ISD::SRL_PARTS, T, Expand);
    setOperationAction(ISD::ADDC, T, Expand);
    setOperationAction(ISD::ADDE, T, Expand);
    setOperationAction(ISD::SUBC, T, Expand);
    setOperationAction(ISD::SUBE, T, Expand);

    // Expand conditionals.
    setOperationAction(ISD::BR_CC, T, Expand);
    setOperationAction(ISD::SELECT_CC, T, Expand);

    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86()) {
        Op = Legal;
      } else if (Subtarget->isAArch64()) {
        Op = Custom;
      } else if (Subtarget->isPPC64le()) {
        Op = Legal;
      } else if (Subtarget->isRISCV()) {
        Op = Custom;
      } else {
        llvm_unreachable("invalid subtarget");
      }

      // Handle casts.
      setOperationAction(ISD::SINT_TO_FP, T, Op);
      setOperationAction(ISD::UINT_TO_FP, T, Op);
      setOperationAction(ISD::STRICT_SINT_TO_FP, T, Op);
      setOperationAction(ISD::STRICT_UINT_TO_FP, T, Op);
      setOperationAction(ISD::FP_TO_UINT, T, Op);
      setOperationAction(ISD::FP_TO_SINT, T, Op);
      setOperationAction(ISD::STRICT_FP_TO_SINT, T, Op);
      setOperationAction(ISD::STRICT_FP_TO_UINT, T, Op);
    }

    // Custom lowering for some actions.
    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86()) {
        Op = Expand;
      } else if (Subtarget->isAArch64()) {
        Op = Custom;
      } else if (Subtarget->isPPC64le()) {
        Op = Custom;
      } else if (Subtarget->isRISCV()) {
        Op = Custom;
      } else {
        llvm_unreachable("invalid subtarget");
      }

      setOperationAction(ISD::SADDO, T, Op);
      setOperationAction(ISD::UADDO, T, Op);
      setOperationAction(ISD::SSUBO, T, Op);
      setOperationAction(ISD::USUBO, T, Op);
    }

    // Custom lowering for some actions.
    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86_32()) {
        Op = T == MVT::i64 ? Custom : Expand;
      } else if (Subtarget->isX86_64()) {
        Op = Expand;
      } else if (Subtarget->isAArch64()) {
        Op = Custom;
      } else if (Subtarget->isPPC64le()) {
        Op = Custom;
      } else if (Subtarget->isRISCV()) {
        Op = Custom;
      } else {
        llvm_unreachable("invalid subtarget");
      }

      setOperationAction(ISD::SMULO, T, Op);
      setOperationAction(ISD::UMULO, T, Op);
    }

    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86_32()) {
        Op = T == MVT::i64 || T == MVT::i128 ? Expand : Legal;
      } else if (Subtarget->isAArch64()) {
        Op = T == MVT::i128 ? Expand : Legal;
      } else {
        Op = T == MVT::i128 ? Expand : Legal;
      }
      setOperationAction(ISD::SDIV, T, Op);
      setOperationAction(ISD::UDIV, T, Op);
      setOperationAction(ISD::SREM, T, Op);
      setOperationAction(ISD::UREM, T, Op);
      setOperationAction(ISD::MUL, T, Op);
    }

    {
      LegalizeAction Op = Legal;
      if (Subtarget->isX86_32()) {
        Op = T == MVT::i64 || T == MVT::i128 ? Expand : Legal;
      } else if (Subtarget->isAArch64()) {
        Op = T == MVT::i64 ? Legal : Expand;
      } else {
        Op = T == MVT::i128 ? Expand : Legal;
      }
      setOperationAction(ISD::MULHS, T, Op);
      setOperationAction(ISD::MULHU, T, Op);
    }
  }

  // Disable in-register sign extension.
  for (auto T : {MVT::i1, MVT::i8, MVT::i16, MVT::i32, MVT::i64}) {
    setOperationAction(ISD::SIGN_EXTEND_INREG, T, Expand);
  }

  // Expand extending loads and stores.
  setLoadExtAction(ISD::EXTLOAD, MVT::f64,  MVT::f32, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f80,  MVT::f32, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f128, MVT::f32, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f80,  MVT::f64, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f128, MVT::f64, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f128, MVT::f80, Expand);
  setTruncStoreAction(MVT::f64,  MVT::f32, Expand);
  setTruncStoreAction(MVT::f80,  MVT::f32, Expand);
  setTruncStoreAction(MVT::f80,  MVT::f64, Expand);
  setTruncStoreAction(MVT::f128, MVT::f32, Expand);
  setTruncStoreAction(MVT::f128, MVT::f64, Expand);
  setTruncStoreAction(MVT::f128, MVT::f80, Expand);

  // Disable extending from bits.
  for (auto T : MVT::integer_valuetypes()) {
    for (auto Ext : {ISD::EXTLOAD, ISD::ZEXTLOAD, ISD::SEXTLOAD}) {
      setLoadExtAction(Ext, T, MVT::i1, Promote);
    }
  }

  // Only allow constants smaller than 64 bits.
  setOperationAction(ISD::ConstantFP, MVT::f32, Legal);
  setOperationAction(ISD::ConstantFP, MVT::f64, Legal);
  setOperationAction(ISD::ConstantFP, MVT::f80, Expand);
  setOperationAction(ISD::ConstantFP, MVT::f128, Expand);
  setOperationAction(ISD::Constant, MVT::i8, Legal);
  setOperationAction(ISD::Constant, MVT::i16, Legal);
  setOperationAction(ISD::Constant, MVT::i32, Legal);
  setOperationAction(ISD::Constant, MVT::i64, Legal);
  setOperationAction(ISD::Constant, MVT::i128, Expand);

  // Lower constant pools.
  setOperationAction(ISD::ConstantPool, MVTPtr, Custom);

  // Preserve traps since they terminate basic blocks.
  setOperationAction(ISD::TRAP, MVT::Other, Legal);

  // Custom lowering for intrinsics.
  setOperationAction(ISD::INTRINSIC_WO_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::INTRINSIC_W_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::INTRINSIC_VOID, MVT::Other, Custom);

  // Custom lowering for some buildings.
  setOperationAction(ISD::READCYCLECOUNTER, MVT::i64, Custom);

  // Custom lowering for fences.
  setOperationAction(ISD::ATOMIC_FENCE  , MVT::Other, Custom);

  // Expansion of memset/memcpy/memmove
  if (Subtarget->isX86()) {
    MaxStoresPerMemset = 16;
    MaxStoresPerMemsetOptSize = 8;
    MaxStoresPerMemcpy = 8;
    MaxStoresPerMemcpyOptSize = 4;
    MaxStoresPerMemmove = 8;
    MaxStoresPerMemmoveOptSize = 4;
    MaxLoadsPerMemcmp = 2;
    MaxLoadsPerMemcmpOptSize = 2;
    setPrefFunctionAlignment(Align(16));
  } else if (Subtarget->isAArch64()) {
    MaxStoresPerMemsetOptSize = 8;
    MaxStoresPerMemset = MaxStoresPerMemsetOptSize;
    MaxGluedStoresPerMemcpy = 4;
    MaxStoresPerMemcpyOptSize = 4;
    MaxStoresPerMemcpy = MaxStoresPerMemcpyOptSize;
    MaxStoresPerMemmoveOptSize = MaxStoresPerMemmove = 4;
    MaxLoadsPerMemcmpOptSize = 4;
    MaxLoadsPerMemcmp =  MaxLoadsPerMemcmpOptSize;
    setPrefFunctionAlignment(Align(8));
  } else if (Subtarget->isPPC64le()) {
    setPrefFunctionAlignment(Align(16));
  } else if (Subtarget->isRISCV()) {
    setPrefFunctionAlignment(Align(4));
  } else {
    llvm_unreachable("unknown architecture");
  }
}

SDValue LLIRTargetLowering::LowerOperation(SDValue Op,
                                           SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
    case ISD::ATOMIC_FENCE:
      return LowerATOMIC_FENCE(Op, DAG);
    case ISD::FRAMEADDR:
      return LowerFRAMEADDR(Op, DAG);
    case ISD::FrameIndex:
      return LowerFrameIndex(Op, DAG);
    case ISD::GlobalAddress:
    case ISD::GlobalTLSAddress:
      return LowerGlobalAddress(Op, DAG);
    case ISD::ExternalSymbol:
      return LowerExternalSymbol(Op, DAG);
    case ISD::JumpTable:
      return LowerJumpTable(Op, DAG);
    case ISD::BR_JT:
      return LowerBR_JT(Op, DAG);
    case ISD::ConstantPool:
      return LowerConstantPool(Op, DAG);
    case ISD::DYNAMIC_STACKALLOC:
      return LowerDynamicStackalloc(Op, DAG);
    case ISD::VASTART:
      return LowerVASTART(Op, DAG);
    case ISD::VAARG:
      return LowerVAARG(Op, DAG);
    case ISD::VACOPY:
      return LowerVACOPY(Op, DAG);
    case ISD::CopyToReg:
      return LowerCopyToReg(Op, DAG);
    case ISD::SADDO:
      return LowerALUO(Op, DAG);
    case ISD::UADDO:
      return LowerALUO(Op, DAG);
    case ISD::SSUBO:
      return LowerALUO(Op, DAG);
    case ISD::USUBO:
      return LowerALUO(Op, DAG);
    case ISD::SMULO:
      return LowerALUO(Op, DAG);
    case ISD::UMULO:
      return LowerALUO(Op, DAG);
    case ISD::INTRINSIC_WO_CHAIN:
      return LowerINTRINSIC_WO_CHAIN(Op, DAG);
    case ISD::INTRINSIC_W_CHAIN:
      return LowerINTRINSIC_W_CHAIN(Op, DAG);
    case ISD::INTRINSIC_VOID:
      return LowerINTRINSIC_VOID(Op, DAG);
    case ISD::READCYCLECOUNTER:
      return LowerREADCYCLECOUNTER(Op, DAG);
    case ISD::BRIND:
      return LowerBRIND(Op, DAG);
    case ISD::FP_EXTEND:
      return LowerFP_EXTEND(Op, DAG);
    case ISD::FP_ROUND:
    case ISD::STRICT_FP_ROUND:
      return LowerFP_ROUND(Op, DAG);
    case ISD::FADD:
    case ISD::STRICT_FADD:
      return LowerF128Call(Op, DAG, RTLIB::ADD_F128);
    case ISD::FSUB:
    case ISD::STRICT_FSUB:
      return LowerF128Call(Op, DAG, RTLIB::SUB_F128);
    case ISD::FMUL:
    case ISD::STRICT_FMUL:
      return LowerF128Call(Op, DAG, RTLIB::MUL_F128);
    case ISD::FDIV:
    case ISD::STRICT_FDIV:
      return LowerF128Call(Op, DAG, RTLIB::DIV_F128);
    case ISD::SETCC:
    case ISD::STRICT_FSETCC:
    case ISD::STRICT_FSETCCS:
      return LowerSETCC(Op, DAG);
    case ISD::SINT_TO_FP:
    case ISD::UINT_TO_FP:
    case ISD::STRICT_SINT_TO_FP:
    case ISD::STRICT_UINT_TO_FP:
      return LowerINT_TO_FP(Op, DAG);
    case ISD::FP_TO_SINT:
    case ISD::FP_TO_UINT:
    case ISD::STRICT_FP_TO_SINT:
    case ISD::STRICT_FP_TO_UINT:
      return LowerFP_TO_INT(Op, DAG);
    default: {
      Op.dump();
      llvm_unreachable("unimplemented operation lowering");
      return SDValue();
    }
  }
}

SDValue LLIRTargetLowering::LowerATOMIC_FENCE(SDValue Op,
                                              SelectionDAG &DAG) const {
  if (Subtarget->isX86()) {
    SDLoc dl(Op);
    AtomicOrdering FenceOrdering =
        static_cast<AtomicOrdering>(Op.getConstantOperandVal(1));
    SyncScope::ID FenceSSID =
        static_cast<SyncScope::ID>(Op.getConstantOperandVal(2));

    // The only fence that needs an instruction is a sequentially-consistent
    // cross-thread fence.
    if (FenceOrdering == AtomicOrdering::SequentiallyConsistent &&
        FenceSSID == SyncScope::System) {
      return DAG.getNode(LLIRISD::MFENCE, dl, MVT::Other, Op.getOperand(0));
    }

    // MEMBARRIER is a compiler barrier; it codegens to a no-op.
    return DAG.getNode(LLIRISD::BARRIER, dl, MVT::Other, Op.getOperand(0));
  }
  llvm_unreachable("not implemented");

}

SDValue LLIRTargetLowering::LowerFRAMEADDR(SDValue Op,
                                           SelectionDAG &DAG) const {
  MVT PtrVT = getPointerTy(DAG.getDataLayout());
  return DAG.getCopyFromReg(Op.getOperand(0), SDLoc(Op), LLIR::FRAME_ADDR,
                            PtrVT);
}

SDValue LLIRTargetLowering::LowerFrameIndex(SDValue Op,
                                            SelectionDAG &DAG) const {
  const auto *FI = cast<FrameIndexSDNode>(Op);
  return DAG.getTargetFrameIndex(FI->getIndex(), Op.getValueType());
}

SDValue LLIRTargetLowering::LowerGlobalAddress(SDValue Op,
                                               SelectionDAG &DAG) const {
  SDLoc DL(Op);

  const auto *GA = cast<GlobalAddressSDNode>(Op);
  if (GA->getAddressSpace() != 0) {
    Fail(DL, DAG, "LLIR only expects the 0 address space");
  }
  if (GA->getTargetFlags() != 0) {
    Fail(DL, DAG, "Unexpected target flags");
  }

  EVT VT = Op.getValueType();
  return DAG.getNode(
      LLIRISD::SYMBOL, DL, VT,
      DAG.getTargetGlobalAddress(GA->getGlobal(), DL, VT, GA->getOffset()));
}

SDValue LLIRTargetLowering::LowerConstantPool(SDValue Op,
                                              SelectionDAG &DAG) const {
  SDLoc DL(Op);

  const auto *CP = cast<ConstantPoolSDNode>(Op);
  if (CP->getTargetFlags() != 0) {
    Fail(DL, DAG, "Unexpected target flags");
  }

  EVT VT = Op.getValueType();
  return DAG.getNode(LLIRISD::SYMBOL, DL, VT,
                     DAG.getTargetConstantPool(CP->getConstVal(), VT,
                                               CP->getAlign(), CP->getOffset()));
}

SDValue LLIRTargetLowering::LowerExternalSymbol(SDValue Op,
                                                SelectionDAG &DAG) const {
  SDLoc DL(Op);
  const auto *ES = cast<ExternalSymbolSDNode>(Op);
  EVT VT = Op.getValueType();
  if (ES->getTargetFlags() != 0) {
    Fail(DL, DAG, "Unexpected target flags");
  }
  return DAG.getNode(LLIRISD::SYMBOL, DL, VT,
                     DAG.getTargetExternalSymbol(ES->getSymbol(), VT));
}

SDValue LLIRTargetLowering::LowerBR_JT(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);

  const auto *JT = cast<JumpTableSDNode>(Op.getOperand(1));
  SDValue Index = Op.getOperand(2);

  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Index);

  // Add a BB operand for each case.
  MachineJumpTableInfo *MJTI = DAG.getMachineFunction().getJumpTableInfo();
  for (auto MBB : MJTI->getJumpTables()[JT->getIndex()].MBBs) {
    Ops.push_back(DAG.getBasicBlock(MBB));
  }

  // Create a custom node.
  return DAG.getNode(LLIRISD::SWITCH, DL, MVT::Other, Ops);
}

SDValue LLIRTargetLowering::LowerJumpTable(SDValue Op,
                                           SelectionDAG &DAG) const {
  const JumpTableSDNode *JT = cast<JumpTableSDNode>(Op);
  return DAG.getTargetJumpTable(JT->getIndex(), Op.getValueType(),
                                JT->getTargetFlags());
}

SDValue LLIRTargetLowering::LowerDynamicStackalloc(SDValue Op,
                                                   SelectionDAG &DAG) const {
  MVT PtrVT = getPointerTy(DAG.getDataLayout());
  SDVTList VTs = DAG.getVTList(PtrVT, MVT::Other);
  return DAG.getNode(LLIRISD::ALLOCA, SDLoc(Op), VTs, Op.getOperand(0),
                     Op.getOperand(1), Op.getOperand(2));
}

SDValue LLIRTargetLowering::LowerVASTART(SDValue Op, SelectionDAG &DAG) const {
  return DAG.getNode(LLIRISD::VASTART, SDLoc(Op), MVT::Other, Op.getOperand(0),
                     Op.getOperand(1));
}

SDValue LLIRTargetLowering::LowerVAARG(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("not implemented");
}

SDValue LLIRTargetLowering::LowerVACOPY(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);
  SDValue DstPtr = Op.getOperand(1);
  SDValue SrcPtr = Op.getOperand(2);

  const Value *DstSV = cast<SrcValueSDNode>(Op.getOperand(3))->getValue();
  const Value *SrcSV = cast<SrcValueSDNode>(Op.getOperand(4))->getValue();

  if (Subtarget->isX86()) {
    // X86-64 va_list is a struct { i32, i32, i8*, i8* }, except on Windows,
    // where a va_list is still an i8*.
    if (Subtarget->isCallingConvWin64(
            DAG.getMachineFunction().getFunction().getCallingConv()))
      // Probably a Win64 va_copy.
      return DAG.expandVACopy(Op.getNode());

    return DAG.getMemcpy(Chain, DL, DstPtr, SrcPtr,
                         DAG.getIntPtrConstant(24, DL), Align(8),
                         /*isVolatile*/ false, false, false,
                         MachinePointerInfo(DstSV), MachinePointerInfo(SrcSV));
  }
  if (Subtarget->isAArch64()) {
    // AAPCS has three pointers and two ints (= 32 bytes), Darwin has single
    // pointer.
    unsigned PtrSize = Subtarget->isTargetILP32() ? 4 : 8;
    unsigned VaListSize =
        (Subtarget->isTargetDarwin() || Subtarget->isTargetWindows()) ? PtrSize
                                                                      : 32;
    return DAG.getMemcpy(Chain, DL, DstPtr, SrcPtr,
                         DAG.getConstant(VaListSize, DL, MVT::i32),
                         Align(PtrSize),
                         /*isVolatile*/ false, false, false,
                         MachinePointerInfo(DstSV), MachinePointerInfo(SrcSV));
  }
  if (Subtarget->isPPC64le()) {
    return DAG.expandVACopy(Op.getNode());
  }
  if (Subtarget->isRISCV()) {
    return DAG.expandVACopy(Op.getNode());
  }
  llvm_unreachable("invalid architecture");
}

SDValue LLIRTargetLowering::LowerCopyToReg(SDValue Op,
                                           SelectionDAG &DAG) const {
  SDValue Src = Op.getOperand(2);
  if (isa<FrameIndexSDNode>(Src.getNode())) {
    SDValue Chain = Op.getOperand(0);
    SDLoc DL(Op);

    EVT VT = Src.getValueType();

    unsigned MovOp;
    if (VT == MVT::i8)
      MovOp = LLIR::MOV_I8;
    else if (VT == MVT::i16)
      MovOp = LLIR::MOV_I16;
    else if (VT == MVT::i32)
      MovOp = LLIR::MOV_I32;
    else if (VT == MVT::i64)
      MovOp = LLIR::MOV_I64;
    else
      llvm_unreachable("invalid copy type");

    SDValue Copy(DAG.getMachineNode(MovOp, DL, VT, Src), 0);

    unsigned Reg = cast<RegisterSDNode>(Op.getOperand(1))->getReg();
    if (Op.getNode()->getNumValues() == 1) {
      return DAG.getCopyToReg(Chain, DL, Reg, Copy);
    } else {
      return DAG.getCopyToReg(
          Chain, DL, Reg, Copy,
          Op.getNumOperands() == 4 ? Op.getOperand(3) : SDValue());
    }
  }
  return SDValue();
}

SDValue LLIRTargetLowering::LowerALUO(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDNode *N = Op.getNode();
  SDValue LHS = N->getOperand(0);
  SDValue RHS = N->getOperand(1);

  unsigned vop, op;
  switch (Op.getOpcode()) {
    case ISD::SADDO:
      vop = ISD::ADD;
      op = LLIRISD::SADDO;
      break;
    case ISD::UADDO:
      vop = ISD::ADD;
      op = LLIRISD::UADDO;
      break;
    case ISD::SSUBO:
      vop = ISD::SUB;
      op = LLIRISD::SSUBO;
      break;
    case ISD::USUBO:
      vop = ISD::SUB;
      op = LLIRISD::USUBO;
      break;
    case ISD::SMULO:
      vop = ISD::MUL;
      op = LLIRISD::SMULO;
      break;
    case ISD::UMULO:
      vop = ISD::MUL;
      op = LLIRISD::UMULO;
      break;
  }

  SDValue Result = DAG.getNode(vop, DL, N->getValueType(0), LHS, RHS);
  SDValue Flag = DAG.getNode(op, DL, N->getValueType(1), LHS, RHS);
  return DAG.getNode(ISD::MERGE_VALUES, DL, N->getVTList(), Result, Flag);
}

SDValue LLIRTargetLowering::LowerINTRINSIC_WO_CHAIN(SDValue Op,
                                                    SelectionDAG &DAG) const {
  llvm_unreachable("invalid intrinsic");
}

SDValue LLIRTargetLowering::LowerINTRINSIC_W_CHAIN(SDValue Op,
                                                   SelectionDAG &DAG) const {
  switch (cast<ConstantSDNode>(Op.getOperand(1))->getZExtValue()) {
    case Intrinsic::x86_rdtsc: {
      return DAG.getNode(LLIRISD::RDTSC, SDLoc(Op),
                         DAG.getVTList(MVT::i64, MVT::Other), Op.getOperand(0));
    }
    case Intrinsic::llir_ldaxr:
    case Intrinsic::llir_stlxr:
    case Intrinsic::llir_ldxr:
    case Intrinsic::llir_stxr: {
      return SDValue();
    }
  }
  llvm_unreachable("invalid intrinsic");
}

SDValue LLIRTargetLowering::LowerINTRINSIC_VOID(SDValue Op,
                                                SelectionDAG &DAG) const {
  llvm_unreachable("invalid intrinsic");
}

SDValue LLIRTargetLowering::LowerREADCYCLECOUNTER(SDValue Op,
                                                  SelectionDAG &DAG) const {
  return DAG.getNode(LLIRISD::RDTSC, SDLoc(Op),
                   DAG.getVTList(MVT::i64, MVT::Other), Op.getOperand(0));
}

SDValue LLIRTargetLowering::LowerBRIND(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("not implemented");
}

MachineBasicBlock *LLIRTargetLowering::EmitInstrWithCustomInserter(
    MachineInstr &MI, MachineBasicBlock *MBB) const {
  llvm_unreachable("EmitInstrWithCustomInserter");
}

const char *LLIRTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (static_cast<LLIRISD::NodeType>(Opcode)) {
    case LLIRISD::FIRST_NUMBER:
      return nullptr;
    case LLIRISD::RETURN:
      return "LLIRISD::RETURN";
    case LLIRISD::ARGUMENT:
      return "LLIRISD::ARGUMENT";
    case LLIRISD::CALL:
      return "LLIRISD::CALL";
    case LLIRISD::TCALL:
      return "LLIRISD::TCALL";
    case LLIRISD::VOID:
      return "LLIRISD::VOID";
    case LLIRISD::TVOID:
      return "LLIRISD::TVOID";
    case LLIRISD::CALL_VA:
      return "LLIRISD::CALL_VA";
    case LLIRISD::TCALL_VA:
      return "LLIRISD::TCALL_VA";
    case LLIRISD::VOID_VA:
      return "LLIRISD::VOID_VA";
    case LLIRISD::TVOID_VA:
      return "LLIRISD::TVOID";
    case LLIRISD::SYMBOL:
      return "LLIRISD::SYMBOL";
    case LLIRISD::SWITCH:
      return "LLIRISD::SWITCH";
    case LLIRISD::VASTART:
      return "LLIRISD::VASTART";
    case LLIRISD::SADDO:
      return "LLIRISD::SADDO";
    case LLIRISD::UADDO:
      return "LLIRISD::UADDO";
    case LLIRISD::SSUBO:
      return "LLIRISD::SSUBO";
    case LLIRISD::USUBO:
      return "LLIRISD::USUBO";
    case LLIRISD::SMULO:
      return "LLIRISD::SMULO";
    case LLIRISD::UMULO:
      return "LLIRISD::UMULO";
    case LLIRISD::ALLOCA:
      return "LLIRISD::ALLOCA";
    case LLIRISD::RDTSC:
      return "LLIRISD::RDTSC";
    case LLIRISD::LL:
      return "LLIRISD::LL";
    case LLIRISD::SC:
      return "LLIRISD::SC";
    case LLIRISD::BARRIER:
      return "LLIRISD::BARRIER";
    case LLIRISD::MFENCE:
      return "LLIRISD::MFENCE";
  }
  llvm_unreachable("invalid opcode");
}

std::pair<unsigned, const TargetRegisterClass *>
LLIRTargetLowering::getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                                 StringRef Constraint,
                                                 MVT VT) const {
  if (Constraint.size() == 1) {
    switch (Constraint[0]) {
      default:
        break;
      case 'r':
        switch (VT.SimpleTy) {
          default:
            llvm_unreachable("unknown type");
          case MVT::i8:
            return std::make_pair(0U, &LLIR::I8RegClass);
          case MVT::i16:
            return std::make_pair(0U, &LLIR::I16RegClass);
          case MVT::i32:
            return std::make_pair(0U, &LLIR::I32RegClass);
          case MVT::i64:
            return std::make_pair(0U, &LLIR::I64RegClass);
          case MVT::f32:
            return std::make_pair(0U, &LLIR::F32RegClass);
          case MVT::f64:
            return std::make_pair(0U, &LLIR::F64RegClass);
        }
        break;
    }
  }

  return TargetLowering::getRegForInlineAsmConstraint(TRI, Constraint, VT);
}

bool LLIRTargetLowering::isOffsetFoldingLegal(
    const GlobalAddressSDNode *GA) const {
  return true;
}

MVT LLIRTargetLowering::getScalarShiftAmountTy(const DataLayout &DL,
                                               EVT VT) const {
  return MVT::i8;
}

EVT LLIRTargetLowering::getSetCCResultType(const DataLayout &DL,
                                           LLVMContext &Context, EVT VT) const {
  if (!VT.isVector()) {
    return MVT::i8;
  }
  llvm_unreachable("getSetCCResultType");
}

static bool isCallingConvSupported(CallingConv::ID ID) {
  switch (ID) {
    case CallingConv::C:
    case CallingConv::Fast:
    case CallingConv::Win64:
    case CallingConv::LLIR_SETJMP:
      return true;
    default:
      return false;
  }
}

static uint64_t EncodeFlags(const ISD::ArgFlagsTy &Flag) {
  if (Flag.isByVal()) {
    return (static_cast<uint64_t>(Flag.getByValSize()) << 32ull) |
           (static_cast<uint64_t>(Flag.getNonZeroByValAlign().value() << 16ull)) |
           (static_cast<uint64_t>(1));
  } else if (Flag.isZExt()) {
    return 2;
  } else if (Flag.isSExt()) {
    return 3;
  } else {
    return 0;
  }
}

SDValue LLIRTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  auto *MFI = MF.getInfo<LLIRMachineFunctionInfo>();

  if (!isCallingConvSupported(CallConv)) {
    Fail(DL, DAG, "unsupported calling convention");
  }

  for (const auto &In : Ins) {
    InVals.push_back(
        DAG.getNode(LLIRISD::ARGUMENT, DL, In.VT,
                    DAG.getTargetConstant(InVals.size(), DL, MVT::i32)));
    MFI->addParam(In.VT, EncodeFlags(In.Flags));
  }

  return Chain;
}

SDValue LLIRTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc DL = CLI.DL;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;

  if (const auto *GA = ::dyn_cast_or_null<GlobalAddressSDNode>(CLI.Callee)) {
    if (auto *GV = GA->getGlobal()) {
      StringRef name = GV->getName( );
      if (name == "sigsetjmp" || name == "setjmp" || name == "_setjmp") {
        CallConv = CallingConv::LLIR_SETJMP;
      }
    }
  }

  MachineFunction &MF = DAG.getMachineFunction();
  const Module *M = MF.getMMI().getModule();
  Metadata *IsCFProtectionSupported = M->getModuleFlag("cf-protection-branch");

  if (CLI.Ins.size() > 1) {
    Fail(DL, DAG, "more than 1 return value not supported");
  }

  if (!isCallingConvSupported(CallConv)) {
    Fail(DL, DAG, "unsupported calling convention");
  }

  if (IsCFProtectionSupported) {
    Fail(DL, DAG, "CF protection not supported");
  }

  // Argument to the call node.
  SmallVector<SDValue, 16> Ops;

  // Collect all fixed arguments.
  unsigned NumFixedArgs = 0;
  for (unsigned i = 0; i < CLI.Outs.size(); ++i) {
    NumFixedArgs += CLI.Outs[i].IsFixed;
  }

  // Add all arguments to the call.
  // chain - flags - nargs - retflags - callee - args
  Ops.push_back(Chain);
  unsigned callFlag = static_cast<int>(CallConv);
  Ops.push_back(DAG.getTargetConstant(callFlag, DL, MVT::i32));
  if (CLI.IsVarArg) {
    Ops.push_back(DAG.getTargetConstant(NumFixedArgs, DL, MVT::i32));
  }

  SmallVector<EVT, 8> InTys;
  for (const auto &In : CLI.Ins) {
    uint64_t flag = EncodeFlags(In.Flags);
    Ops.push_back(DAG.getTargetConstant(flag, DL, MVT::i64));
    InTys.push_back(In.VT);
  }
  InTys.push_back(MVT::Other);
  Ops.push_back(Callee);

  for (unsigned i = 0; i < CLI.Outs.size(); ++i) {
    const SDValue &Arg = CLI.OutVals[i];
    const ISD::OutputArg &Out = CLI.Outs[i];
    uint64_t flag = EncodeFlags(Out.Flags);
    Ops.push_back(DAG.getTargetConstant(flag, DL, MVT::i64));
    Ops.push_back(Arg);
  }

  // Construct the call node.
  if (CLI.Ins.empty()) {
    unsigned Op;
    if (CLI.IsVarArg) {
      Op = CLI.IsTailCall ? LLIRISD::TVOID_VA : LLIRISD::VOID_VA;
    } else {
      Op = CLI.IsTailCall ? LLIRISD::TVOID : LLIRISD::VOID;
    }
    return DAG.getNode(Op, DL, DAG.getVTList(InTys), Ops);
  } else {
    if (CLI.IsTailCall) {
      unsigned Op = CLI.IsVarArg ? LLIRISD::TCALL_VA : LLIRISD::TCALL;
      return DAG.getNode(Op, DL, DAG.getVTList(InTys), Ops).getValue(1);
    } else {
      unsigned Op = CLI.IsVarArg ? LLIRISD::CALL_VA : LLIRISD::CALL;
      SDValue Call = DAG.getNode(Op, DL, DAG.getVTList(InTys), Ops);
      InVals.push_back(Call);
      return Call.getValue(1);
    }
  }
}

void LLIRTargetLowering::ReplaceNodeResults(SDNode *N,
                                            SmallVectorImpl<SDValue> &Results,
                                            SelectionDAG &DAG) const {
  SDLoc dl(N);
  switch (N->getOpcode()) {
  default:
#ifndef NDEBUG
    N->dump(&DAG);
#endif
    llvm_unreachable("Do not know how to custom type legalize this operation!");
  }
}

bool LLIRTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv, MachineFunction &MF, bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs, LLVMContext &Context) const {
  return Outs.size() <= 1;
}

SDValue LLIRTargetLowering::LowerReturn(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
    SelectionDAG &DAG) const {
  if (Outs.size() > 1) {
    Fail(DL, DAG, "calling convention not supported");
  }

  SmallVector<SDValue, 4> Ops(1, Chain);
  for (unsigned I = 0, N = Outs.size(); I < N; ++I) {
    uint64_t flag = EncodeFlags(Outs[I].Flags);
    Ops.push_back(DAG.getTargetConstant(flag, DL, MVT::i64));
    Ops.push_back(OutVals[I]);
  }
  Chain = DAG.getNode(LLIRISD::RETURN, DL, MVT::Other, Ops);

  return Chain;
}

LLIRTargetLowering::AtomicExpansionKind
LLIRTargetLowering::shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const {
  if (Subtarget->isX86()) {
    AtomicRMWInst::BinOp Op = AI->getOperation();
    switch (Op) {
    default:
      llvm_unreachable("Unknown atomic operation");
    case AtomicRMWInst::Xchg:
      return AtomicExpansionKind::CmpXChg;
    case AtomicRMWInst::Add:
    case AtomicRMWInst::Sub:
    case AtomicRMWInst::Or:
    case AtomicRMWInst::And:
    case AtomicRMWInst::Xor:
    case AtomicRMWInst::Nand:
    case AtomicRMWInst::Max:
    case AtomicRMWInst::Min:
    case AtomicRMWInst::UMax:
    case AtomicRMWInst::UMin:
    case AtomicRMWInst::FAdd:
    case AtomicRMWInst::FSub:
      return AtomicExpansionKind::CmpXChg;
    }
  }
  if (Subtarget->isAArch64()) {
    return AtomicExpansionKind::LLSC;
  }
  if (Subtarget->isRISCV()) {
    return AtomicExpansionKind::CmpXChg;
  }
  if (Subtarget->isPPC64le()) {
    return AtomicExpansionKind::LLSC;
  }
  llvm_unreachable("not implemented");
}

bool LLIRTargetLowering::mayBeEmittedAsTailCall(const CallInst *CI) const {
  // Make sure tail calls aren't disabled.
  auto Attr =
      CI->getParent()->getParent()->getFnAttribute("disable-tail-calls");
  if (!CI->isTailCall() || Attr.getValueAsString() == "true") {
    return false;
  }

  switch (CI->getCallingConv()) {
    // C calling conventions:
    case CallingConv::C:
    case CallingConv::Win64:
    case CallingConv::X86_64_SysV:
      return true;
    // Callee pop conventions:
    case CallingConv::X86_ThisCall:
    case CallingConv::X86_StdCall:
    case CallingConv::X86_VectorCall:
    case CallingConv::X86_FastCall:
      return true;
    // Guarantee TCO.
    case CallingConv::Fast:
    case CallingConv::GHC:
    case CallingConv::X86_RegCall:
    case CallingConv::HiPE:
    case CallingConv::HHVM:
      return true;
    // Don't tail call otherwise.
    default:
      return false;
  }
}

Value *LLIRTargetLowering::emitLoadLinked(IRBuilder<> &Builder, Value *Addr,
                                             AtomicOrdering Ord) const {
  Module *M = Builder.GetInsertBlock()->getParent()->getParent();
  bool IsAcquire = isAcquireOrStronger(Ord);

  Type *ValTy = cast<PointerType>(Addr->getType())->getElementType();
  Type *Tys[] = {ValTy, Addr->getType()};
  Intrinsic::ID Int = IsAcquire ? Intrinsic::llir_ldaxr : Intrinsic::llir_ldxr;
  Function *Ldxr = Intrinsic::getDeclaration(M, Int, Tys);

  return Builder.CreateCall(Ldxr, Addr);
}


Value *LLIRTargetLowering::emitStoreConditional(IRBuilder<> &Builder, Value *Val,
                                              Value *Addr,
                                              AtomicOrdering Ord) const {
  Module *M = Builder.GetInsertBlock()->getParent()->getParent();
  bool IsRelease = isReleaseOrStronger(Ord);
  Intrinsic::ID Int = IsRelease ? Intrinsic::llir_stlxr : Intrinsic::llir_stxr;

  Type *ValTy = cast<PointerType>(Addr->getType())->getElementType();
  Type *Tys[] = {ValTy, Addr->getType()};
  Function *Stxr = Intrinsic::getDeclaration(M, Int, Tys);

  return Builder.CreateCall(Stxr, {Val, Addr});
}

SDValue LLIRTargetLowering::LowerF128Call(SDValue Op, SelectionDAG &DAG,
                                          RTLIB::Libcall Call) const {
  bool IsStrict = Op->isStrictFPOpcode();
  unsigned Offset = IsStrict ? 1 : 0;
  SDValue Chain = IsStrict ? Op.getOperand(0) : SDValue();
  SmallVector<SDValue, 2> Ops(Op->op_begin() + Offset, Op->op_end());
  MakeLibCallOptions CallOptions;
  SDValue Result;
  SDLoc dl(Op);
  std::tie(Result, Chain) =
      makeLibCall(DAG, Call, Op.getValueType(), Ops, CallOptions, dl, Chain);
  return IsStrict ? DAG.getMergeValues({Result, Chain}, dl) : Result;
}

SDValue LLIRTargetLowering::LowerI128Call(SDValue Op, SelectionDAG &DAG,
                                          RTLIB::Libcall Call) const {
  SmallVector<SDValue, 2> Ops(Op->op_begin(), Op->op_end());
  MakeLibCallOptions CallOptions;

  SDValue Result;
  SDValue Chain;
  SDLoc dl(Op);
  std::tie(Result, Chain) =
      makeLibCall(DAG, Call, Op.getValueType(), Ops, CallOptions, dl);
  return Result;
}

SDValue LLIRTargetLowering::LowerSETCC(SDValue Op, SelectionDAG &DAG) const {
  bool IsStrict = Op->isStrictFPOpcode();
  bool IsSignaling = Op.getOpcode() == ISD::STRICT_FSETCCS;
  unsigned OpNo = IsStrict ? 1 : 0;
  SDValue Chain;
  if (IsStrict)
    Chain = Op.getOperand(0);
  SDValue LHS = Op.getOperand(OpNo + 0);
  SDValue RHS = Op.getOperand(OpNo + 1);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(OpNo + 2))->get();
  EVT VT = Op.getValueType();
  SDLoc dl(Op);

  assert(LHS.getValueType() == MVT::f128);
  softenSetCCOperands(DAG, MVT::f128, LHS, RHS, CC, dl, LHS, RHS, Chain,
                      IsSignaling);

  // If softenSetCCOperands returned a scalar, use it.
  if (!RHS.getNode()) {
    assert(LHS.getValueType() == Op.getValueType() &&
           "Unexpected setcc expansion!");
    return IsStrict ? DAG.getMergeValues({LHS, Chain}, dl) : LHS;
  }

  if (LHS.getValueType().isInteger()) {
    SDValue Res = DAG.getSetCC(dl, VT, LHS, RHS, CC, SDValue(), IsSignaling);
    return IsStrict ? DAG.getMergeValues({Res, Chain}, dl) : Res;
  }

  llvm_unreachable("not implemented");
}

SDValue LLIRTargetLowering::LowerFP_EXTEND(SDValue Op,
                                           SelectionDAG &DAG) const {
  assert(Op.getValueType() == MVT::f128 && "Unexpected lowering");
  RTLIB::Libcall LC;
  LC = RTLIB::getFPEXT(Op.getOperand(0).getValueType(), Op.getValueType());
  return LowerF128Call(Op, DAG, LC);
}

SDValue LLIRTargetLowering::LowerFP_ROUND(SDValue Op, SelectionDAG &DAG) const {
  bool IsStrict = Op->isStrictFPOpcode();
  SDValue SrcVal = Op.getOperand(IsStrict ? 1 : 0);
  EVT SrcVT = SrcVal.getValueType();

  RTLIB::Libcall LC;
  LC = RTLIB::getFPROUND(SrcVT, Op.getValueType());

  if (SrcVT != MVT::f128) {
    // It's legal except when f128 is involved
    return Op;
  }

  // FP_ROUND node has a second operand indicating whether it is known to be
  // precise. That doesn't take part in the LibCall so we can't directly use
  // LowerF128Call.
  MakeLibCallOptions CallOptions;
  SDValue Chain = IsStrict ? Op.getOperand(0) : SDValue();
  SDValue Result;
  SDLoc dl(Op);
  std::tie(Result, Chain) = makeLibCall(DAG, LC, Op.getValueType(), SrcVal,
                                        CallOptions, dl, Chain);
  return IsStrict ? DAG.getMergeValues({Result, Chain}, dl) : Result;
}

SDValue LLIRTargetLowering::LowerINT_TO_FP(SDValue Op,
                                           SelectionDAG &DAG) const {
  SDLoc dl(Op);
  bool IsSigned = Op.getOpcode() == ISD::SINT_TO_FP ||
                  Op.getOpcode() == ISD::STRICT_SINT_TO_FP;
  bool IsStrict = Op->isStrictFPOpcode();
  SDValue SrcVal = Op.getOperand(IsStrict ? 1 : 0);
  auto SrcTy = SrcVal.getValueType();

  // i128 conversions are libcalls.
  if (SrcTy == MVT::i128)
    return SDValue();

  // Convert i8/i16 to i32.
  if (Subtarget->isAArch64() && (SrcTy == MVT::i8 || SrcTy == MVT::i16))
    SrcTy = MVT::i32;

  // Other conversions are legal, unless it's to the completely software-based
  // fp128.
  if (Op.getValueType() != MVT::f128)
    return Op;

  RTLIB::Libcall LC;
  if (IsSigned) {
    LC = RTLIB::getSINTTOFP(SrcTy, Op.getValueType());
    if (SrcVal.getValueType() != SrcTy)
      SrcVal = DAG.getSExtOrTrunc(SrcVal, dl, SrcTy);
  } else {
    LC = RTLIB::getUINTTOFP(SrcTy, Op.getValueType());
    if (SrcVal.getValueType() != SrcTy)
      SrcVal = DAG.getZExtOrTrunc(SrcVal, dl, SrcTy);
  }

  SDValue Chain = IsStrict ? Op.getOperand(0) : SDValue();
  SmallVector<SDValue, 2> Ops{ SrcVal };
  MakeLibCallOptions CallOptions;
  SDValue Result;
  std::tie(Result, Chain) =
      makeLibCall(DAG, LC, Op.getValueType(), Ops, CallOptions, dl, Chain);
  return IsStrict ? DAG.getMergeValues({Result, Chain}, dl) : Result;
}

SDValue LLIRTargetLowering::LowerFP_TO_INT(SDValue Op,
                                              SelectionDAG &DAG) const {
  bool IsStrict = Op->isStrictFPOpcode();
  SDValue SrcVal = Op.getOperand(IsStrict ? 1 : 0);

  if (SrcVal.getValueType() != MVT::f128) {
    // It's legal except when f128 is involved
    return Op;
  }

  RTLIB::Libcall LC;
  if (Op.getOpcode() == ISD::FP_TO_SINT ||
      Op.getOpcode() == ISD::STRICT_FP_TO_SINT)
    LC = RTLIB::getFPTOSINT(SrcVal.getValueType(), Op.getValueType());
  else
    LC = RTLIB::getFPTOUINT(SrcVal.getValueType(), Op.getValueType());

  return LowerF128Call(Op, DAG, LC);
}

static bool isOffsetSuitableForCodeModel(int64_t Offset, CodeModel::Model M,
                                       bool hasSymbolicDisplacement) {
  // Offset should fit into 32 bit immediate field.
  if (!isInt<32>(Offset))
    return false;

  // If we don't have a symbolic displacement - we don't have any extra
  // restrictions.
  if (!hasSymbolicDisplacement)
    return true;

  // FIXME: Some tweaks might be needed for medium code model.
  if (M != CodeModel::Small && M != CodeModel::Kernel)
    return false;

  // For small code model we assume that latest object is 16MB before end of 31
  // bits boundary. We may also accept pretty large negative constants knowing
  // that all objects are in the positive half of address space.
  if (M == CodeModel::Small && Offset < 16*1024*1024)
    return true;

  // For kernel code model we know that all object resist in the negative half
  // of 32bits address space. We may not accept negative offsets, since they may
  // be just off and we may accept pretty large positive ones.
  if (M == CodeModel::Kernel && Offset >= 0)
    return true;

  return false;
}

bool LLIRTargetLowering::isLegalAddressingMode(const DataLayout &DL,
                                               const AddrMode &AM, Type *Ty,
                                               unsigned AS,
                                               Instruction *I) const {
  if (Subtarget->isX86()) {
    // X86 supports extremely general addressing modes.
    CodeModel::Model M = getTargetMachine().getCodeModel();

    // X86 allows a sign-extended 32-bit immediate field as a displacement.
    if (!isOffsetSuitableForCodeModel(AM.BaseOffs, M,
                                           AM.BaseGV != nullptr))
      return false;

    if (AM.BaseGV) {
      // TODO
      /*
      unsigned GVFlags = Subtarget->classifyGlobalReference(AM.BaseGV);

      // If a reference to this global requires an extra load, we can't fold it.
      if (isGlobalStubReference(GVFlags))
        return false;

      // If BaseGV requires a register for the PIC base, we cannot also have a
      // BaseReg specified.
      if (AM.HasBaseReg && isGlobalRelativeToPICBase(GVFlags))
        return false;

      // If lower 4G is not available, then we must use rip-relative addressing.
      if ((M != CodeModel::Small || isPositionIndependent()) &&
          Subtarget->is64Bit() && (AM.BaseOffs || AM.Scale > 1))
        return false;
      */
    }

    switch (AM.Scale) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 8:
      // These scales always work.
      break;
    case 3:
    case 5:
    case 9:
      // These scales are formed with basereg+scalereg.  Only accept if there is
      // no basereg yet.
      if (AM.HasBaseReg)
        return false;
      break;
    default: // Other stuff never works.
      return false;
    }

    return true;
  }
  if (Subtarget->isAArch64()) {
    // AArch64 has five basic addressing modes:
    //  reg
    //  reg + 9-bit signed offset
    //  reg + SIZE_IN_BYTES * 12-bit unsigned offset
    //  reg1 + reg2
    //  reg + SIZE_IN_BYTES * reg

    // No global is ever allowed as a base.
    if (AM.BaseGV)
      return false;

    // No reg+reg+imm addressing.
    if (AM.HasBaseReg && AM.BaseOffs && AM.Scale)
      return false;

    // FIXME: Update this method to support scalable addressing modes.
    if (isa<ScalableVectorType>(Ty))
      return AM.HasBaseReg && !AM.BaseOffs && !AM.Scale;

    // check reg + imm case:
    // i.e., reg + 0, reg + imm9, reg + SIZE_IN_BYTES * uimm12
    uint64_t NumBytes = 0;
    if (Ty->isSized()) {
      uint64_t NumBits = DL.getTypeSizeInBits(Ty);
      NumBytes = NumBits / 8;
      if (!isPowerOf2_64(NumBits))
        NumBytes = 0;
    }

    if (!AM.Scale) {
      int64_t Offset = AM.BaseOffs;

      // 9-bit signed offset
      if (isInt<9>(Offset))
        return true;

      // 12-bit unsigned offset
      unsigned shift = Log2_64(NumBytes);
      if (NumBytes && Offset > 0 && (Offset / NumBytes) <= (1LL << 12) - 1 &&
          // Must be a multiple of NumBytes (NumBytes is a power of 2)
          (Offset >> shift) << shift == Offset)
        return true;
      return false;
    }

    // Check reg1 + SIZE_IN_BYTES * reg2 and reg1 + reg2

    return AM.Scale == 1 || (AM.Scale > 0 && (uint64_t)AM.Scale == NumBytes);
  }
  if (Subtarget->isRISCV()) {
    // No global is ever allowed as a base.
    if (AM.BaseGV)
      return false;

    // Require a 12-bit signed offset.
    if (!isInt<12>(AM.BaseOffs))
      return false;

    switch (AM.Scale) {
    case 0: // "r+i" or just "i", depending on HasBaseReg.
      break;
    case 1:
      if (!AM.HasBaseReg) // allow "r+i".
        break;
      return false; // disallow "r+r" or "r+r+i".
    default:
      return false;
    }

    return true;
  }
  if (Subtarget->isPPC64le()) {
    // Vector type r+i form is supported since power9 as DQ form. We don't check
    // the offset matching DQ form requirement(off % 16 == 0), because on
    // PowerPC, imm form is preferred and the offset can be adjusted to use imm
    // form later in pass PPCLoopInstrFormPrep. Also in LSR, for one LSRUse, it
    // uses min and max offset to check legal addressing mode, we should be a
    // little aggressive to contain other offsets for that LSRUse.
    if (Ty->isVectorTy() && AM.BaseOffs != 0 && !Subtarget->hasP9Vector())
      return false;

    // PPC allows a sign-extended 16-bit immediate field.
    if (AM.BaseOffs <= -(1LL << 16) || AM.BaseOffs >= (1LL << 16) - 1)
      return false;

    // No global is ever allowed as a base.
    if (AM.BaseGV)
      return false;

    // PPC only support r+r,
    switch (AM.Scale) {
    case 0: // "r+i" or just "i", depending on HasBaseReg.
      break;
    case 1:
      if (AM.HasBaseReg && AM.BaseOffs) // "r+r+i" is not allowed.
        return false;
      // Otherwise we have r+r or r+i.
      break;
    case 2:
      if (AM.HasBaseReg || AM.BaseOffs) // 2*r+r  or  2*r+i is not allowed.
        return false;
      // Allow 2*r as r+r.
      break;
    default:
      // No other scales are supported.
      return false;
    }

    return true;
  }
  llvm_unreachable("unknown subtarget");
}

int LLIRTargetLowering::getScalingFactorCost(const DataLayout &DL,
                                             const AddrMode &AM, Type *Ty,
                                             unsigned AS) const {
  if (Subtarget->isX86()) {
    if (isLegalAddressingMode(DL, AM, Ty, AS))
      return AM.Scale != 0;
    return -1;
  }
  if (Subtarget->isAArch64()) {
    if (isLegalAddressingMode(DL, AM, Ty, AS))
      return AM.Scale != 0 && AM.Scale != 1;
    return -1;
  }
  if (Subtarget->isRISCV()) {
    return TargetLowering::getScalingFactorCost(DL, AM, Ty, AS);
  }
  if (Subtarget->isPPC64le()) {
    return TargetLowering::getScalingFactorCost(DL, AM, Ty, AS);
  }
  llvm_unreachable("unknown subtarget");
}
