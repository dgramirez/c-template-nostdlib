# C Template: No Standard Library  

This is a simple "No Standard Library" Template for my future C projects.  
License: [Unlicense](https://unlicense.org/)  

### Requirements  

Linux
  - Assembler: FASM
  - Platform: Clang, GCC, Zig \(zig cc\)
  - App \[Shared Library\]: Clang, GCC, Zig \(zig cc\), CProc, TCC

Windows:
  - Assembler: FASM
  - Platform: MSVC (VS 2022), MSVC (2017: XP Support), Zig 0.13.0  
  - App: MSVC (VS 2022), MSVC (2017: XP Support), Zig 0.13.0

### Quick Build
Linux:
  1. Set CC Environment Variable to either Clang or GCC. If this is not done, then cc will be used.
  2. Go into /misc directory
  3. If required, chmod +x build.sh (Make sure its the shell file!)
  4. run `./build.sh`
  5. Go back one directory (`cd ..`)
  6. go into /out directory
  7. Run `./tempalte`

Windows (GUI):
  1. Go into /misc folder
  2. Double Click on build.bat (Make sure its the batch file!)  
    - Note: This will only create a debug version. If you want a release version, please read the cmd
            instructions below.
  3. Go back one folder
  4. Go into /out/msvc/debug
  5. Double Click on template.exe  
    - This is fine for GUI applications, but console applications will dissappear.
      On console applications, run cmd.exe, go to the out folder you want, then
      run template.

Windows (Cmd):
  1. cd into /misc folder
  2. Type any of these configurations  
    - build  
    - build a  
  3. cd to `../out`
  4. cd into the compiler / platform folder used
  5. cd into the configuration desired
  6. run `template`.

### Self Build
You can use **any** C compiler as long as it supports the features required. As of now, Cproc and TCC  
are not supported for the platform layer, as it uses volatile and atomics. Here are the files you need
to compile for Linux:
  1. fasm -d PLATFORM=LINUX src/entry-x86_64.fasm
  2. cc main-linux.c entry-x86_64.o
  3. cc ... -i \<Include Dir\>
  5. cc ... -nostdlib # -lc for platform layer, hoping to change this eventually, though musl is an option

For Windows (Non-XP):
  1. fasm -d PLATFORM=WIN64 src/entry-x86_64.fasm
  2. %CC% entry-x86_64-win32.c entry-x86_64.obj
  3. /I \<Include Dir\>
  4. /link /NODEFAULTLIB /ENTRY:_start /SUBSYSTEM:\<subsystem\>

Windows XP is a bit complicated, as you have to set it up yourself (unlike in Visual Studio IDE, where
xml files already do the job for you.) The important things are as follows:
  1. The Environment Variables are set to use Windows 7 SDK with XP Support  
    - Read :config_xp32 and :config_xp64 inside build.bat  
  2. When running vcvars batch file, make sure it uses the Win7 SDK w/ XP support version  
    - vcvars_ver=14.16  
    - Read :detect_msvc inside build.bat  
  3. /SUBSYSTEM must exist with a value, and **must** append the correct subsystem version  
    - XP SP3 x86: 5.01  
    - XP SP2 x64: 5.02  
    - Read :config_xp32 and :config_xp64 for the version, then :config_msvc for application inside build.bat  
  4. Finally, the defines for msvc to detect XP
    - WINVER, _WIN32_WINNT and NTDDI_VERSION.  
    - Read :config_xp32 and :config_xp64 inside build.bat to see its values.  
  5. For Windows XP SP3 x86, you are going to want two additional things  
    - For FASM's Platform, replace WIN64 with WIN32  
    - Include /arch:IA32 as a **compiler** flag (Make sure its before /link)  
    - Read :config_xp32 and :config_xp64 inside build.bat for these two.

Finally, For Windows XP \(64-bit, Remember Step 5 From Above for x86\)
  1. fasm -d PLATFORM=WIN64
  2. %CC% entry-x86_64-win32.c entry-x86_64.obj
  3. /link /NODEFAULTLIB /ENTRY:_start /SUBSYSTEM:\<subsystem\>

Compiling with Zig CC is another complex one. Unlike Window's /NODEFAULTLIB, using -nostdlib will have you
lose \<windows.h\> include path. Why? I have NO IDEA. Read :config_zig inside build.bat to see how I solved
that issue. From there, its very similar to the Linux/Clang flags, adding the /subsystem:<subsystem> from
lld-link. I don't know if you can do Windows XP support here, so I simply don't support it with zig cc.  

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

Assembly Function:
  - cpuid_native

# Future Improvements
  - Threading & Threadpool! Clone is implemented, however there's a process to use it (Using StackHead, and requires an mmap for the stack.)
