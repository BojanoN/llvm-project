//===--- BareMetal.h - Bare  Tool and ToolChain -------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MUGO_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MUGO_H

#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"

#include <string>

namespace clang {
namespace driver {
namespace tools {
namespace mugo {

class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  Linker(const ToolChain &TC) : Tool("mugo::Linker", "ld.lld", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};
} // namespace mugo
} // namespace tools

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY Mugo : public ToolChain {
public:
  Mugo(const Driver &D, const llvm::Triple &Triple,
       const llvm::opt::ArgList &Args);
  ~Mugo() override = default;

  static bool handlesTarget(const llvm::Triple &Triple);

protected:
  Tool *buildLinker() const override;

public:
  // Mugo uses `nasm`
  bool useIntegratedAs() const override { return false; }
  bool isCrossCompiling() const override { return true; }
  bool isPICDefault() const override { return false; }
  bool isPIEDefault() const override { return false; }
  bool isPICDefaultForced() const override { return false; }
  bool SupportsProfiling() const override { return false; }

  llvm::DebuggerKind getDefaultDebuggerTuning() const override {
    return llvm::DebuggerKind::GDB;
  }

  LangOptions::StackProtectorMode
  GetDefaultStackProtectorLevel(bool KernelOrKext) const override {
    return LangOptions::SSPStrong;
  }

  const char *getDefaultLinker() const override { return "ld.lld"; }

  void
  addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                        llvm::opt::ArgStringList &CC1Args,
                        Action::OffloadKind DeviceOffloadKind) const override;
};

} // namespace toolchains
} // namespace driver
} // namespace clang

#endif
