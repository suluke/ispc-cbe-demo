# Demo: Using C-Backend Independent of ISPC

LLVM's C-Backend (cbe) has not been actively maintained upstream by the llvm community for quite some time.
Still, use cases pop up every now and then, e.g. if you want to get LLVM IR running on platforms where only proprietary C compilers are available.
The [ISPC project](https://github.com/ispc/ispc) maintains their own fork of the c-backend, although tightly coupled with the rest of the ispc compiler.
This demo project is intended to show how the cbackend can be compiled and used outside the ispc source tree.
Furthermore, the small tool (`cbetest`) that can be compiled with cmake can make it easier to test the cbe's correctness with arbitrary LLVM IR input.
