#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Option/Option.h>
#include <llvm/Support/SourceMgr.h> // SMDiagnostic
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_ostream.h>
#include <string>
#include "cbe.h"

using namespace llvm;
using namespace std;
cl::OptionCategory
    OptionCat("Options", "Options to influence the behavior of the tool");
static cl::opt<string> Filename(cl::Positional, cl::desc("<input>"),
                                     cl::init("-"), cl::cat(OptionCat));
static cl::opt<string> Output("o", cl::desc("Output file"),
                                   cl::init("out.cc"),
                                   cl::cat(OptionCat));
static cl::opt<string>
    IntriniscsFile("intrinsics", cl::desc("Path to header where intrinsics are defined"),
                   cl::init("generic-64.h"),
                   cl::cat(OptionCat));

int main(int argc, char **argv) {
  LLVMContext Context;
  cl::ParseCommandLineOptions(argc, argv);
  SMDiagnostic Diag;
  auto M_owner = parseIRFile(Filename, Diag, Context);
  if (!M_owner) {
    errs() << "Couldn't load input file " << Filename << "\n";
    Diag.print(Filename.c_str(), errs(), true, true);
    return 1;
  }
  auto &M = *M_owner.get();
  write_cxx_file(&M, Output.c_str(), 16,
                 IntriniscsFile.c_str());
  return 0;
}
