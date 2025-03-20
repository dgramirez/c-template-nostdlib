@ECHO OFF
SETLOCAL

SET bin_name=template
SET app_name=libapp

SET root=
SET inc=
SET src=
SET out=
SET out_dbg=
SET out_rel=
SET out_drl=
SET build=
SET compiler=
SET cc_objs=
SET cc_flags=
SET cc_dbg=
SET cc_rel=
SET cc_drl=
SET link_flags=
SET link_dbg=
SET link_rel=
SET link_drl=
SET clean=
SET xp=
SET xp_arch=
SET xp_subsysver=
SET fasm_arch=
SET cc_name=
SET cc_objs_app=
SET cc_flags_app=
SET link_flags_app=
SET cc_flags_common=
SET link_flags_common=
SET link_dbg_app=
SET link_rel_app=
SET link_drl_app=

SET args_errored=
CALL :fill_args %1
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %2
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %3
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %4
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %5
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %6
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %7
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %8
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

CALL :fill_args %9
IF ["%args_errored%"] == ["1"] ( GOTO :EOF )

:: Project Setup
FOR %%i IN ( %~DP0.. ) DO SET "root=%%~fi"
SET inc=%root%\include
SET src=%root%\src
SET out=%root%\out

IF ["%clean%"] == ["1"] (
	RMDIR /S /Q "%out%"
	PUSHD "%src%"
		del /S /Q /F *.obj *.o *.pdb >nul 2>nul
	POPD

	echo Cleaned Project!
	GOTO :EOF
)

:: TODO - Figure this shit out.
::        I need to set cc_name as soon as either where zig
::        or setup_cl.bat is ran.
SET fasm_arch=WIN64
IF ["%compiler%"] == ["cl"]     ( CALL :detect_msvc )
IF ["%compiler%"] == ["zig cc"] ( CALL :detect_zig )
IF NOT DEFINED compiler ( CALL :detect_compiler )
IF NOT DEFINED compiler ( GOTO :no_compiler_found )

IF NOT DEFINED build ( SET build=1 )

SET out_dbg=%out%\%cc_name%\debug
SET out_rel=%out%\%cc_name%\release
SET out_drl=%out%\%cc_name%\debug-release

:: Configuration Setup
SET cc_objs=entry-x86_64-win32.c entry-x86_64.obj
SET cc_objs_app=libapp.c
IF ["%compiler%"] == ["cl"]     ( CALL :config_msvc )
IF ["%compiler%"] == ["zig cc"] ( CALL :config_zig )

:: Compile
PUSHD "%SRC%"
	SET ERRORLEVEL=0

	fasm -d PLATFORM=%fasm_arch% entry-x86_64.fasm
	IF ["%ERRORLEVEL%"] == ["1"] ( ECHO "FAILED" && GOTO :EOF )

	SET /A "1/(build&1)" 2>nul && (
		IF NOT EXIST "%out_dbg%" ( mkdir %out_dbg% )

		%compiler% %cc_flags% %cc_dbg% %cc_objs% %link_flags% %link_dbg%

		%compiler% %cc_flags_app% %cc_dbg_app% %cc_objs_app% ^
			%link_flags_app% %link_dbg_app%
	)
	IF ["%ERRORLEVEL%"] == ["1"] ( ECHO "FAILED" && GOTO :EOF )

	SET /A "1/(build&2)" 2>nul && (
		IF NOT EXIST "%out_rel%" ( mkdir %out_rel% )
		
		%compiler% %cc_flags% %cc_rel% %cc_objs% %link_flags% %link_rel%

		%compiler% %cc_flags_app% %cc_rel_app% %cc_objs_app% ^
			%link_flags_app% %link_rel_app%
	)
	IF ["%ERRORLEVEL%"] == ["1"] ( ECHO "FAILED" && GOTO :EOF )

	SET /A "1/(build&4)" 2>nul && (
		IF NOT EXIST "%out_drl%" ( mkdir %out_drl% )

		%compiler% %cc_flags% %cc_drl% %cc_objs% %link_flags% %link_drl%

		%compiler% %cc_flags_app% %cc_drl_app% %cc_objs_app% ^
			%link_flags_app% %link_drl_app%
	)

	del /S /Q /F *.obj *.o *.pdb >nul 2>nul
POPD

ENDLOCAL
GOTO :EOF

:fill_args
	SET made_changes=

	IF ["%1"] == [""]     ( GOTO :EOF )
	IF ["%1"] == ["c"]    ( SET clean=1&&GOTO :EOF )
	IF ["%1"] == ["zig"]  ( SET "compiler=zig cc"&&GOTO :EOF )
	IF ["%1"] == ["msvc"] ( SET compiler=cl&&GOTO :EOF )
	IF ["%1"] == ["xp"]   ( SET xp=1&&GOTO :EOF )
	IF ["%1"] == ["xp64"] ( SET xp=2&&GOTO :EOF )
	IF ["%1"] == ["a"]    ( SET build=7&&GOTO :EOF )

	ECHO.%1 | FIND /I "d">nul && (
		IF NOT DEFINED build (
			SET build=1
			SET made_changes=1
		) ELSE (
			2>nul (
				SET /A "1/(build&1)" || (
					SET /A "build=%build%+1"
					SET made_changes=1
				)
			)
		)
	)

	ECHO.%1 | FIND /I "r">nul && (
		IF NOT DEFINED build (
			SET build=2
			SET made_changes=1
		) ELSE (
			2>nul (
				SET /A "1/(build&2)" || (
					SET /A "build=%build%+2"
					SET made_changes=1
				)
			)
		)
	)

	ECHO.%1 | FIND /I "x">nul && (
		IF NOT DEFINED build (
			SET build=4
			SET made_changes=1
		) ELSE (
			2>nul (
				SET /A "1/(build&4)" || (
					SET /A "build=%build%+4"
					SET made_changes=1
				)
			)
		)
	)
	
	IF DEFINED made_changes ( GOTO :EOF )

	ECHO Error with %1
	ECHO    - Unknown Argument
	ECHO    - Attempted to re-write COMPILER (Can only set once)
	ECHO    - Attempted to re-write BUILD (Can only set once per configuration)
	ECHO.
	ECHO Valid Arguments:
	ECHO    zig     - Compiles using Zig's  "zig.exe cc"
	ECHO    msvc    - Compiles using MSVC's "CL.exe"
	ECHO.
	ECHO    d       - Compiles with Debug Configurations.
	ECHO    r       - Compiles with Release Configurations.
	ECHO    x       - Compiles with Debug-Release Configurations.
	ECHO    a       - Compiles all configurations (same as drx)
	ECHO.
	ECHO Examples:
	ECHO    build msvc dr  && ECHO Compiles two outputs, Debug & Release
	ECHO    build msvc d x && ECHO Compiles two outputs, Debug & Debug-Release
	ECHO    build zig  x   && ECHO Compiles one output, Debug-Release
	ECHO    build msvc a   && ECHO Compiles all three outputs. Same as drx.

	SET args_errored=1
GOTO :EOF

:detect_compiler
	IF DEFINED compiler goto :EOF

	CALL :detect_msvc
	IF DEFINED compiler GOTO :EOF

	CALL :detect_zig
GOTO :EOF

:detect_msvc
	IF DEFINED LIB ( GOTO :EOF )

	IF ["%xp%"] == ["1"] ( call :config_xp32 )
	IF ["%xp%"] == ["2"] ( call :config_xp64 )
	IF DEFINED xp (
		CALL "%~dp0\setup_cl.bat" %xp_arch% -vcvars_ver=14.16
		IF NOT DEFINED LIB ( GOTO :EOF )

		SET compiler=cl
		GOTO :EOF
	)

	IF DEFINED LIB (
		SET compiler=cl
		SET cc_name=msvc
		GOTO :EOF
	)
	CALL "%~dp0\setup_cl.bat" x64
	IF DEFINED LIB (
		SET compiler=cl
		SET cc_name=msvc
		GOTO :EOF
	)
GOTO :EOF

:detect_zig
	WHERE zig >nul 2>nul
	IF ["%ERRORLEVEL%"] == ["0"] ( SET "compiler=zig cc" )

	SET cc_name=zig
GOTO :EOF

:no_compiler_found
	ECHO Unable to detect a compiler! Please install one of the following
	ECHO compilers:
	ECHO    Microsoft Visual Studio
	ECHO    VS2017 and up, including build tools, should be fine. Ensure that
	ECHO    you have the modules to compile a C/C++ Project. For Windows XP
	ECHO    support, you must have the "Windows XP" Component and the
	ECHO    "v141 Build Tools" Component to create the Windows XP Executable.
	ECHO    If there is any problems, you may need to add your version of
	ECHO    Visual Studio to setup_cl.bat [Latest is VS2022 as of this time]
	ECHO.
	ECHO    Zig Compiler
	ECHO    Version 0.13.0 ONLY! Unfortunately, 0.14.0 is a complete disaster
	ECHO    for this project. Will wait for 0.14.1 or 0.15.0
	ECHO.
GOTO :EOF

:config_msvc
	SET cc_flags_common=

	SET cc_objs=%cc_objs% Kernel32.lib User32.lib Gdi32.lib Opengl32.lib
	SET cc_flags=/EHa- /fp:except- /fp:fast /GL- /Gm- /GR- /GS- /W4 /WX ^
	/Gs2147483647 /nologo /wd4201 /I"%inc%" %cc_flags%
	SET link_flags=/link /WX /STACK:0x200000,0x200000 /INCREMENTAL:NO ^
	/NODEFAULTLIB /ENTRY:_start /SUBSYSTEM:CONSOLE%xp_subsysver%

	SET cc_dbg=/Zi /Od /D "_DEBUG"
	SET cc_rel=/O2 /D "NODEBUG"
	SET cc_drl=/Zi /O2 /D "NODEBUG" /D "_DEBUG_RELEASE"

	SET link_dbg=/DEBUG:FULL /OUT:"%out_dbg%\%bin_name%.exe"
	SET link_rel=/DEBUG:NONE /RELEASE /OUT:"%out_rel%\%bin_name%.exe"
	SET link_drl=/DEBUG:FULL /OUT:"%out_drl%\%bin_name%.exe"

	SET cc_flags_app=/D_USRDLL /D_WINDLL %cc_flags%
	SET link_flags_app=/link /Wx /STACK:0x200000,0x200000 /INCREMENTAL:NO ^
	/NODEFAULTLIB /NOENTRY /DLL

	SET link_dbg_app=/DEBUG:FULL /OUT:"%out_dbg%\%app_name%.dll"
	SET link_rel_app=/DEBUG:NONE /RELEASE /OUT:"%out_rel%\%app_name%.dll"
	SET link_drl_app=/DEBUG:FULL /OUT:"%out_drl%\%app_name%.dll"
GOTO :EOF

:config_zig
	SET TEMP=
	FOR /F "tokens=* delims=" %%i IN ('where zig') DO SET "TEMP=%%~dpi"
	SET TEMP="%TEMP%\lib\libc\include\any-windows-any"

	SET cc_flags_app=-target x86_64-windows-gnu -std=c99 -nostdlib -D_ZIG ^
	-Wall -Werror -Wno-unused-function  ^
	-fno-asynchronous-unwind-tables -fno-rtti -fno-builtin -fno-exceptions ^
	-fno-signed-zeros -ffinite-math-only -ffast-math ^
	-I %inc% -mno-stack-arg-probe -z stack-size=0x200000 -fno-sanitize=all ^
	-isystem %TEMP% /NODEFAULTLIB

	SET link_flags_app=/DLL /NOENTRY -shared

	SET link_dbg_app=/DEBUG:FULL /OUT:"%out_dbg%\%app_name%.dll"
	SET link_rel_app=/DEBUG:NONE /RELEASE /OUT:"%out_rel%\%app_name%.dll"
	SET link_drl_app=/DEBUG:FULL /OUT:"%out_drl%\%app_name%.dll"

	SET cc_objs=%cc_objs% -lkernel32 -luser32 -lgdi32 -lopengl32
	SET cc_flags=%cc_flags_app% -e "_start" /SUBSYSTEM:CONSOLE ^
	%cc_flags%

	SET cc_dbg=-O0 -g -fdebug-macro -fno-standalone-debug /DEBUG:FULL ^
	-D_DEBUG -o "%out_dbg%\%bin_name%.exe"
	SET cc_rel=-O2 -fno-debug-macro /DEBUG:NONE -DNDEBUG ^
	-o "%out_rel%\%bin_name%.exe"
	SET cc_drl=-O2 -g -fdebug-macro -fno-standalone-debug /DEBUG:FULL ^
	/RELEASE -DNDEBUG -D_DEBUG_RELEASE -o "%out_drl%\%bin_name%.exe"
GOTO :EOF

:config_xp32
	SET xp_arch=x86
	SET fasm_arch=WIN32
	SET xp_subsysver=,5.01
	SET cc_name=xp32
	SET PATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin;%PATH%
	SET INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include;%INCLUDE%
	SET LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib;%LIB%
	SET LIBPATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib;%LIBPATH%

	SET cc_flags= /D"WINVER=0x0501" /D"_WIN32_WINNT=0x0501" ^
	/D"NTDDI_VERSION=0x05010300" /arch:IA32
GOTO :EOF

:config_xp64
	SET xp_arch=x64
	SET fasm_arch=WIN64
	SET xp_subsysver=,5.02
	SET cc_name=xp64
	SET PATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\x64;%PATH%
	SET INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include;%INCLUDE%
	SET LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64;%LIB%
	SET LIBPATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64;%LIBPATH%

	SET cc_flags=/D"WINVER=0x0502" /D"_WIN32_WINNT=0x0502" ^
	/D"NTDDI_VERSION=0x05020200"
GOTO :EOF

