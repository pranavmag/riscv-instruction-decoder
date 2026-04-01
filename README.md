# riscv-instruction-emulator

A RISC-V emulator built from scratch in C++ as a learning project alongside

studying Computer Architecture, Operating Systems, and Systems Programming.



\## Current Features

\- Full RV32I instruction decoder

\- Supports R, I, S, B, J, and U-type instruction formats

\- Decodes all base integer instructions including arithmetic, logic, shifts,

&#x20; branches, loads, stores, and jumps

\- Register file with correct x0 hardwiring

\- Instruction execution for arithmetic and logic instructions



\## Build

Open in Visual Studio and build with MSVC, or compile with:

```

g++ main.cpp decoder.cpp interpreter.cpp -o riscv-emulator

```


## Roadmap
- [x] Complete instruction decoder (input 32-bit binary string)
- [x] Complete instruction execution (branches, loads, stores, jumps)
- [x] Program counter and fetch-decode-execute loop
- [x] Memory simulation (byte-addressed, read/write byte/halfword/word)
- [x] Load and run RISC-V binary programs (.bin file loading)
- [x] Disassembler mode (decode and print instructions)
- [x] Manual input interpreter mode
- [ ] Run a non-trivial program (Fibonacci)
- [ ] RISC-V toolchain integration (compile and run real programs)
- [ ] ELF file loading (future)
- [ ] Multi-core simulation (future)
- [ ] Virtual memory / MMU (future)
- [ ] Interrupt and trap handling (future)
- [ ] CSR (Control and Status Registers) (future)


\## Learning Journey

This project grows alongside my reading of:

\- Patterson \& Hennessy - Computer Organization and Design RISC-V Edition

\- More books to follow



Documented on YouTube Streams: https://www.youtube.com/@NootNavDev

