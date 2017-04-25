#include <llvm/IR/Module.h>

bool write_cxx_file(llvm::Module *module, const char *outfile, const int vectorWidth,
                    const char *intrinsicsFile);
