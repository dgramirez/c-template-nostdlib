# Nostd: Directory Structure  

This is a simple documentation explaining the directory layout for this
project. Due to me being the person developing this project along with
current me \(emphesis on the words *current me*\) just not needing it due
to how I set up my environment, I feel this will be necessary for anybody
that wishes to take a good look at how everything is set up, along with
*future me*.  

As of April 2nd 2025, this was updated. Therefore any new directories
after to this date \(More accurately: This commit\) will not be documented
until I come back to this file. I will do my absolute best to keep this
updated since I would like some order in this project.  

# Root \(/\)  
The root directory contains mostly what is expected in a c program.  

#### /misc  
This directory contains the scripts and non-readme documentation \(like this
one!\). For Windows, batch scripts will be used so we don't get users into
**Bad habits like turning off their Powershell Execution Policy just to
build this.** For Linux, Shell scripts will be used since its the most
widely distributed shell on Linux Distributions. Until I remove this line,
The shell script will be using the bash shell until I do a real test with
just sh. The documentation will be Markdown Files.  

#### /out \[May not exist\]  
The out directory will exist once you run a build script for the first
time. With one exception, they follow the same directory layout when
compiling:  
  - /out/${Compiler}/${Configuration}/...  

The one exception is Windows XP Support. Despite using msvc, the directory
where ${Compiler} would be will be xp32 and xp64. Another observation,
though its not a lie, would be on Linux in where I use two compilers instead
of one. This is because I wanted to support the cproc compiler, and it
doesn't support creating shared libraries. In this case, I use the compiler
that compiles the platform layer, not the shared libraries.  

#### /src  
This holds the source files for the compiler to compile. Despite being a
simple directory, a word of warning in that this is a single-module
compilation. fasm will compile the "entry-${architecture}.fasm" file and
a c compiler will compile the "entry-${architecture}-${platform}.c" file.
Here is a quick summary on the subdirectories in here:  

<ul>
    <li>
        **/src/platform:** This is where the core platform layer code
        resides. This is used to setup all necessary components \(like
        threadpools, memory allocation, graphics, etc.\)
    </li>
    <li>
        **/src/app:** This is where the application code resides. For
        console applications, This uses 3 functions (init, update, close)
        for the platform layer to call. This may expand once graphics,
        sound and inpute gets involved.
    </li>
    <li>
        **/src/cpu:** This is where the cpu code resides. This will contain
        a mixure of fasm and c code to provide the best way of delivering
        cpu features for the platform layer to give to the application.
        To make this simple, if a function, algorithm, data structure, etc.
        requires the cpuid instruction to test for features, then it belongs
        to the libcpu dynamic library. Otherwise, it can be added to the
        platform layer and a required module if necessary.
    </li>
</ul>

# /include
The include directory contains one directory and one file.  

**/include/nostd.h:** Contains all the include files for a **shared
library.** This is how the platform layer will communicate with each of the
modules, vice versa, and even between these modules \(Through the platform
layer.\)  

**/include/nostd/:** This is a directory that contains all the necessities
to make the platform layer work. It is setup in 4 directory:  

#### Inside /include/nostd/ directory:  
**/include/nostd/core/:** The functions that are relied upon by everything.
What makes this unique is the idea of being able to be designed to be
standalone. It will look the ugliest, but it should not disappoint.  

**/include/nostd/ext/** External code from other sources. With an extremely
few exceptions \(OpenGL might be one of them\), This will only contain
public domain code. In specific, Any code that the author releases or
license with a public domain license \(CC-0, UNLICENSE, WTFPL/DWTFYWT,
etc.\) could be in here. This is so I don't have to deal with licensing BS.
I want this codebase to be as public domain as possible.  

**/include/nostd/common/:** The functions that are relied upon by the
necessary modules. For instance, the application and platform need to know
the structures for graphics, sound, etc. However, the graphics module 
shouldn't need to know how the sound module and vice versa.  

**/include/nostd/platform/:** Code to be used for the specified platform.
This will be split up into directories, in this fashion:  
  - **/include/nostd/platform/${PLATFORM}/${ARCH}**  
  - **/include/nostd/platform/${PLATFORM}.h**  

The only exception to this rule is the common setup  
  - **/include/nostd/platform/common/**  
  - **/include/nostd/platform/common.h**  

common is used for functions that can be used across all platforms but is
not necessary for the modules to know about. The utilization of Graphics
Libraries would be an example \(The application don't need to know about
Vulkan or OpenGL Draw Calls, just fill in the buffer and send it over.\)  

# Future Updates  
I'm very confident with most of this directory layout. Of course, I can't
predict the future, so if I were to add more directories outside of what
is in this document then I will do my best to remember to update this
document to reflect that.  

I am very certain that I will add more modules as time goes on. I wanted to
get this codebase ready to create games, and tools \(Console and GUI.\)
When I do create a new module, that directory will be added to here.

