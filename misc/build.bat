@ECHO OFF
SETLOCAL

:::::::::::::::::::::::::::
:: Variable Declarations ::
:::::::::::::::::::::::::::
SET bin_name=template

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
SET xp=

SET args_errored=

::::::::::::::::::::
:: Argument Setup ::
::::::::::::::::::::
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

:::::::::::::::::::
:: Project Setup ::
:::::::::::::::::::
IF NOT DEFINED compiler ( CALL :detect_compiler )
IF NOT DEFINED compiler ( GOTO :no_compiler_found )

IF NOT DEFINED build (
	:: Use Debug
	SET build=1
)

FOR %%i IN ( %~DP0.. ) DO SET "root=%%~fi"
SET inc=%root%\include
SET src=%root%\src
SET out=%root%\out
:::::::::::::::::::::::::
:: Configuration Setup ::
:::::::::::::::::::::::::
set cc_objs=entry-x86_64-win32.c entry-x86_64.obj
IF ["%compiler%"] == ["cl"] (
	SET out_dbg=%out%\msvc\debug
	SET out_rel=%out%\msvc\release
	SET out_drl=%out%\msvc\debug-release

	CALL :config_msvc
) ELSE (
	SET out_dbg=%out%\zig\debug
	SET out_rel=%out%\zig\release
	SET out_drl=%out%\zig\debug-release

	CALL :config_zig
)

::::::::::::::
:: Compile! ::
::::::::::::::
PUSHD "%SRC%"
	SET ERRORLEVEL=0

	fasm -d PLATFORM=WIN64 entry-x86_64.fasm
	IF ["%ERRORLEVEL%"] == ["1"] ( echo "FAILED" && goto :eof )

	SET /A "1/(build&1)" 2>nul && (
		IF NOT EXIST "%out_dbg%" ( mkdir %out_dbg% )
		%compiler% %cc_flags% %cc_dbg% %cc_objs% %link_flags% %link_dbg%
	)
	IF ["%ERRORLEVEL%"] == ["1"] ( echo "FAILED" && goto :eof )

	SET /A "1/(build&2)" 2>nul && (
		IF NOT EXIST "%out_rel%" ( mkdir %out_rel% )
		%compiler% %cc_flags% %cc_rel% %cc_objs% %link_flags% %link_rel%
	)
	IF ["%ERRORLEVEL%"] == ["1"] ( echo "FAILED" && goto :eof )

	SET /A "1/(build&4)" 2>nul && (
		IF NOT EXIST "%out_drl%" ( mkdir %out_drl% )
		%compiler% %cc_flags% %cc_drl% %cc_objs% %link_flags% %link_drl%
	)
POPD

ENDLOCAL
GOTO :EOF

:fill_args
	SET made_changes=

	:::::::::::
	:: Empty ::
	:::::::::::
	IF ["%1"] == [""] ( GOTO :EOF )

	::::::::::::::
	:: Compiler ::
	::::::::::::::
	IF ["%1"] == ["zig"] (
		SET compiler=zig cc
		GOTO :EOF
	)
	IF ["%1"] == ["msvc"] (
		SET compiler=cl
		GOTO :EOF
	)

	:::::::::::
	:: Build ::
	:::::::::::
	:fill_args_build
	IF ["%1"] == ["a"] ( SET build=7 && GOTO :EOF )

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

	:fill_args_end
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
	IF DEFINED LIB ( SET compiler=cl&&GOTO :EOF )
	CALL "%~dp0\setup_cl.bat" x64
	IF DEFINED LIB ( SET compiler=cl&&GOTO :EOF )

	WHERE zig >nul 2>nul
	IF ["%ERRORLEVEL%"] == ["0"] ( SET compiler=zig cc )
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
	ECHO    for this project, as 
	ECHO    
GOTO :EOF

:config_msvc
	SET cc_objs=%cc_objs% Kernel32.lib User32.lib Gdi32.lib Opengl32.lib
	SET cc_flags= /EHa- /fp:except- /fp:fast /GL- /Gm- /GR- /GS- /W4 /WX ^
	/Gs2147483647 /nologo /wd4201 /I"%inc%"
	SET link_flags=/link /WX /STACK:0x200000,0x200000 /INCREMENTAL:NO ^
	/NODEFAULTLIB /ENTRY:_start /SUBSYSTEM:CONSOLE

	SET cc_dbg=/Zi /Od /D "_DEBUG"
	SET cc_rel=/O2 /D "NDEBUG"
	SET cc_drl=/Zi /O2 /D "NDBEUG"

	SET link_dbg=/DEBUG:FULL /OUT:"%out_dbg%\%bin_name%.exe"
	SET link_rel=/DEBUG:NONE /RELEASE /OUT:"%out_rel%\%bin_name%.exe"
	SET link_drl=/DEBUG:FULL /OUT:"%out_drl%\%bin_name%.exe"
GOTO :EOF

:config_zig
	SET TEMP=
	FOR /F "tokens=* delims=" %%i IN ('where zig') DO SET "TEMP=%%~dpi"
	SET TEMP="%TEMP%\lib\libc\include\any-windows-any"

	SET cc_objs=%cc_objs% -lkernel32 -luser32 -lgdi32 -lopengl32
	SET cc_flags=-target x86_64-windows-gnu -std=c99 -nostdlib -D_ZIG ^
	-Wall -Werror -Wno-unused-function  ^
	-fno-asynchronous-unwind-tables -fno-rtti -fno-builtin -fno-exceptions ^
	-fno-signed-zeros -ffinite-math-only -ffast-math ^
	-I %inc% -isystem %TEMP% -mno-stack-arg-probe -z stack-size=0x200000 ^
	-e "_start" /SUBSYSTEM:WINDOWS /NODEFAULTLIB -fno-sanitize=all

	SET cc_dbg=-O0 -g -fdebug-macro -fno-standalone-debug /DEBUG:FULL ^
	-o "%out_dbg%\%bin_name%.exe"
	SET cc_rel=-O2 -fno-debug-macro /DEBUG:NONE /RELEASE ^
	-o "%out_rel%\%bin_name%.exe"
	SET cc_drl=-O2 -g -fdebug-macro -fno-standalone-debug /DEBUG:FULL ^
	/RELEASE -o "%out_drl%\%bin_name%.exe"
GOTO :EOF
