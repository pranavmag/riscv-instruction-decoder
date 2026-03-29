# riscv-instruction-emulator

\# RISC-V Emulator



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



\## Roadmap

\- \[x] Complete instruction decoder (input 32-bit binary string)

\- \[ ] Complete instruction execution (branches, loads, stores, jumps)

\- \[ ] Program counter and fetch-decode-execute loop

\- \[ ] Memory simulation

\- \[ ] Load and run RISC-V binary programs

\- \[ ] Multi-core simulation (future)

\- \[ ] Virtual memory (future)



\## Learning Journey

This project grows alongside my reading of:

\- Patterson \& Hennessy - Computer Organization and Design RISC-V Edition

\- More books to follow



Documented on YouTube Streams: https://www.youtube.com/@NootNavDev

