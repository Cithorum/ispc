/*
  Copyright (c) 2022-2023, Intel Corporation

  SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include "ISPCPass.h"

namespace ispc {

/** LLVM IR implementations of target-specific functions may include calls
    to the functions "bool __is_compile_time_constant_*(...)"; these allow
    them to have specialied code paths for where the corresponding value is
    known at compile time.  For masks, for example, this allows them to not
    incur the cost of a MOVMSK call at runtime to compute its value in
    cases where the mask value isn't known until runtime.

    This pass resolves these calls into either 'true' or 'false' values so
    that later optimization passes can operate with these as constants.

    See stdlib.m4 for a number of uses of this idiom.
 */

class IsCompileTimeConstantPass : public llvm::FunctionPass {
  public:
    static char ID;
    explicit IsCompileTimeConstantPass(bool last = false) : FunctionPass(ID) { isLastTry = last; }

    llvm::StringRef getPassName() const override { return "Resolve \"is compile time constant\""; }
    bool runOnFunction(llvm::Function &F) override;

  private:
    bool isLastTry;
    bool lowerCompileTimeConstant(llvm::BasicBlock &BB);
};

llvm::Pass *CreateIsCompileTimeConstantPass(bool isLastTry);

} // namespace ispc
