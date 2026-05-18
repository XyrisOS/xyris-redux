## Dev environment tips
- The assumed IDE is either CLion (preferred) or VSCode (fallback)
- When working on Windows, prefer using WSL when possible

## Programming Languages
- Swift (Embedded) 6.3.2
- C++ 23
- C 23

## CMake Preferences
- Local, internal variables should use lower-snake case
- Public, exposed variables should use upper-snake case
  
## Kernel
- The current target ISA is x86-64.
- The only supported boot method is Limine using UEFI. BIOS is not supported.
- Modern hardware is the target and legacy x86 BIOS calls, hardware, memory segmenting, etc. is to be avoided

## Tools
- It is OK and expected that the tools CMake reaches into the `Modules` directory despite being in a different directory
- The host hardware and toolchain is the build target for everything within `Tools`