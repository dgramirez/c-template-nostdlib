# C Template: No Standard Library (NoSTD)  

This is a simple "No Standard Library" Template for my future C projects.  
License: [Unlicense](https://unlicense.org/)  

### Requirements & Build:  
Please [Read](misc/DOC_BUILD.md) for this information

### Implemented
Linux Syscalls:
  - sys_write
  - sys_mmap / sys_munmap
  - sys_clone \[Incomplete! Will Explain Below\]
  - sys_exit \[Done w/ FASM\]  

Memory Functions: \[Note: Not SIMD!\]
  - mmemcpy
  - mmemmove
  - mmemset
  - mmemcmp
  - mmemzero
  - mmemeq

Memory Containers:
  - MArena \[Linear Allocator\]
  - MPool

Strings:
  - Counted String (s8)
  - Buffer ASCII (buf8)
    - os_write \[IMPORTANT! Must be set prior to buf8_flush\]
    - buf8_flush
    - buf8_append
    - buf8_append_byte
    - buf8_append_lf / buf8_append_clrf \[Win7 and Below must have CRLF regardless if redirecting to text file!\]
    - buf8_append_usz / buf8_append_isz \[Unsigned / Signed Size Values\]
    - buf8_append_hex / buf8_append_hex_cap
    - buf8_append_f64

Assembly Function 
  - cpuid_native
  - atomics

# Future Improvements
  - Threading & Threadpool! Clone is implemented, however there's a process to use it (Using StackHead, and requires an mmap for the stack.)
