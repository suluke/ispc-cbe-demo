#include "cbe.h"
#include <ispc/ispc.h>
#include <ispc/llvmutil.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>

extern bool WriteCXXFile(llvm::Module *module, const char *fn, int vectorWidth,
                         const char *includeName);

using namespace std;
using namespace llvm;

static bool CBackendInitialized = false;

static void init(LLVMContext &ctx) {
  if (CBackendInitialized)
    return;
  CBackendInitialized = true;
  g = new Globals;
  llvm::InitializeNativeTarget();
  auto *target = new ::Target("x86-64", "generic", "generic-16", false, false);
  g->target = target;
  InitLLVMUtil(&ctx, *target);
}

bool write_cxx_file(llvm::Module *module, const char *outfile,
                    const int vectorWidth, const char *intrinsicsFile) {
  init(module->getContext());
  return WriteCXXFile(module, outfile, vectorWidth, intrinsicsFile);
}
