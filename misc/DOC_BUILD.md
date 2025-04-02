# NoSTD: Building this Project

This will describe how to build this project in a couple of ways.  

## Requirements
<ul>
    <li>Assembler: <a href="https://flatassembler.net/">FASM</a></li>
    <li>C Compiler: MSVC, Clang, Zig, GCC, CProc, TCC</li>
</ul>

## Build using build script
This is by far the easiest way of building this project. The scripts can
be used without arguments, and can be fine-tuned with arguments:  

<p><b>Common: Configurations</b></p>
<ul>
    <li>d: This will attempt to produce debug binaries</li>
    <li>r: This will attempt to produce release binaries</li>
    <li>x: This will attempt to produce debug-release binaries</li>
    <li>a: This will attempt to produce all configurations!</li>
</ul>

<p><b>Common: Compilers</b></p>
<ul>
    <li>
        zig: This will attempt to use the zig compiler to compile the
        platform layer and the modules.
    </li>
    <ul>
        <li>
            <b>WARNING (Win32):</b> Please use 0.13.0, 0.14.0 has made
            so many changes the build just breaks. I will be waiting
            for 0.14.1 or 0.15.0 to see if things get better on the c
            compiling side.
        </li>
        <li>
            <b>WARNING (Linux):</b> This cannot compile the real
            platform layer due to \-lc. This is because they want me to
            use their _start over mine and that will just not be done.
        </li>
    </ul>
</ul>

<p><b>Linux: Compilers</b></p>
<ul>
    <li>
        cproc: This will attempt to use the cproc compiler to compile only
        the platform layer.
    </li>
    <li>
        tcc: This will attempt to use the tiny c compiler to compile the
        platform layer and the modules.
        <ul>
            <li>
                <b>WARNING:</b> as of 0.9.27, it does not compile the
                platform layer!
            </li>
        </ul>
    </li>
</ul>

<p><b>Windows: Compilers</b></p>
<ul>
    <li>
        msvc: This will use the msvc compiler to compile the platform layer
        and the modules.
    </li>
    <li>
        xp32: This will use the msvc compiler to compile the platform layer
        and the modules. These binaries will be compatible for Windows XP
        32-bit SP3.
    </li>
    <li>
        xp64: This will use the msvc compiler to compile the platform layer
        and the modules. These binaries will be compatible for Windows XP
        64-bit SP2.
    </li>
</ul>

### Compiling in Linux:
<ol>
    <li>cd into /misc directory</li>
    <li>If required, chmod +x build.sh</li>
    <li>run `./build.sh`</li>
    <ul>
        <li>Read above to see what arguments you can use.</li>
    </ul>
    <li>cd back one directory (`cd ..`)</li>
    <li>go into /out directory</li>
    <li>Run `./tempalte`</li>
</ol>

### Compiling in Windows (using explorer.exe):
<ol>
    <li>Go into /misc folder</li>
    <li>Double Click on build.bat</li>
    <ul>
        <li>
            This will only create a debug version. If you want a release
            version, please read the cmd instructions below.
        </li>
    </ul>
    <li>Go back one folder</li>
    <li>Go into /out/msvc/debug</li>
    <li>Double Click on template.exe</li>
    <ul>
        <li>
            This is fine for GUI applications, but console applications
            will dissappear. On console applications, run cmd.exe, go to
            the out folder you want, then run template.
        </li>
    </ul>
</ol>

### Compiling in Windows (using cmd.exe):
<ol>
    <li>cd into /misc folder</li>
    <li>Type build</li>
    <ul>
        <li>Read above to see what arguments you can use.</li>
    </ul>
    <li>cd to `../out`</li>
    <li>cd into the compiler / platform folder used</li>
    <li>cd into the configuration desired</li>
    <li>Run `tempalte`</li>
</ol>

## Build Script - Linux Secret  
So, as mentioned above, there are a few problems with a couple of compilers.
zig must be 0.13.0 and cannot compile the real platform layer. cproc cannot
compile modules, only the platform layer. tcc can only compile the
modules.  

The thing is: <b>The compiler argument doesn't need to be filled in!</b>
There are two environment variables that you should be aware of:  
<ul>
    <li>
        CC - This environment variable is used to compile modules.
    </li>
    <li>
        CC_PLT - This environment variable is used to compile the platform
        layer.
    </li>
</ul>

With those two, you can do a bit of crazy things. for instance:  
`export CC="zig cc"`  
`export CC_PLT=cproc`  

Now, you are compiling the platform layer with cproc \(which is valid!\) and
you're compiling the modules \(with no \-lc, which is also valid!\)  

Now, of course this needs to be fixed! Ideally, I want zig to work in all
cases. It would just require more time fiddling with the build.sh file to
get that to work \(Something akin to build.bat.\)  

## Linux Secret 2: Sad Day.
I hope for the day that I can remove this section. However, if you've
noticed with zig, or just reading the build.sh file... a libc implementation
is still being used for the "real application." The reason is just one set
of functions: dlfcn.  
<ul>
    <li>dlopen</li>
    <li>dlsym</li>
    <li>dlclose</li>
</ul>

Now, I can opt-in using static libraries, and this will go away. However,
this will remove future plans that I have for this project. This is why
there are **two compilations: One using -nostdlib alone, and one using -lc**.
If I were to use a standard library function, the first compilation would
fail, and the build script will stop!  

This is where I would like to introduce MUSL libc. Of course, the idea of
getting rid of libc is to not have that reliance on it, however since I
didn't want to break weeks on attempting custom dlfcn \(I did waste
two weeks already, with no success.\) There are other libc implementations
\(ulibc, dietlibc, etc.\) so if having it just for those three functions
is ok, then those are great alternatives.  

If this is a deal stopper, then just wait. I **will** get custom dlfcn
working. Unfortunately, it won't be anytime soon \(I am in the early
stages of development,\) but once the pressure is on for having a libc-free
linux binary comes back then this will be re-attempted.  

## Building - No Script  
An even bigger secret is that you don't need build scripts at all. If you
know what you want to build, its as simple as 4 steps:  

### Linux  
<ol>
    <li>fasm -d PLATFORM=LINUX src/entry-x86_64.fasm</li>
    <li>cc main-linux.c entry-x86_64.o</li>
    <li>cc ... -i &ltInclude Dir&gt</li>
    <li>cc ... -nostdlib # -lc for platform layer, Read Above</li>
</ol>

### Windows (Non-XP)  
<ol>
    <li>fasm -d PLATFORM=WIN64 src/entry-x86_64.fasm</li>
    <li>%CC% entry-x86_64-win32.c entry-x86_64.obj</li>
    <li>/I &ltInclude Dir&gt</li>
    <li>/link /NODEFAULTLIB /ENTRY:_start /SUBSYSTEM:&ltsubsystem&gt</li>
</ol>

### Windows XP  
Windows XP is a bit complicated, as you have to set it up yourself \(unlike
in Visual Studio IDE, where xml files already do the job for you.\) The
important things are as follows:  
<ol>
    <li>
        The Environment Variables are set to use Windows 7 SDK with XP
        Support.
    </li>
    <ul>
        <li>Read :config_xp32 and :config_xp64 inside build.bat.</li>
    </ul>
    <li>
        When running vcvars batch file, make sure it uses the Win7 SDK w/ XP
        support version.
    </li>
    <ul>
        <li>vcvars_ver=14.16</li>
        <li>Read :detect_msvc inside build.bat</li>
    </ul>
    <li>
        /SUBSYSTEM must exist with a value, and **must** append the correct
        subsystem version.
    </li>
    <ul>
        <li>XP SP3 x86: 5.01</li>
        <li>XP SP2 x64: 5.02</li>
        <li>
            Read :config_xp32 and :config_xp64 for the version, then
            :config_msvc for application inside build.bat
        </li>
    </ul>
    <li>Finally, the defines for msvc to detect XP</li>
    <ul>
        <li>WINVER, _WIN32_WINNT and NTDDI_VERSION.</li>
        <li>Read :config_xp32 and :config_xp64 inside build.bat to see
            its values.
        </li>
    </ul>
    <li>
        For Windows XP SP3 x86, you are going to want two additional
        things.
    </li>
    <ul>
        <li>For FASM's Platform, replace WIN64 with WIN32.</li>
        <li>
            Include /arch:IA32 as a **compiler** flag \(Make sure its
            before /link.\)
        </li>
        <li>
            Read :config_xp32 and :config_xp64 inside build.bat for
            these two.
        </li>
    </ul>
</ol>

Finally, For Windows XP \(64-bit, Remember Step 5 From Above for x86\)  
<ol>
    <li>fasm -d PLATFORM=WIN64 entry-x86_64.fasm</li>
    <li>%CC% entry-x86_64-win32.c entry-x86_64.obj</li>
    <li>/I &ltInclude Dir&gt</li>
    <li>/link /NODEFAULTLIB /ENTRY:_start /SUBSYSTEM:&ltsubsystem&gt</li>
</ol>

### Zig CC
Compiling with Zig CC is another complex one. Unlike Window's
/NODEFAULTLIB, using -nostdlib will have you lose &ltwindows.h&gt include
path. Why? I have NO IDEA. Read :config_zig inside build.bat to see how I
solved that issue. From there, its very similar to the Linux/Clang flags,
adding the /subsystem:<subsystem> from lld-link. I don't know if you can do
Windows XP support here, so I simply don't support it with zig cc.  

