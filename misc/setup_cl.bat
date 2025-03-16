@echo off

:::::::::::::::::::::::::::::::::
:: MIT License - Allen Webster ::
:::::::::::::::::::::::::::::::::

:: Copyright (c) 2022 Allen Webster
::
:: Permission is hereby granted, free of charge, to any person obtaining a
:: copy of this software and associated documentation files
:: (the "Software"), to deal in the Software without restriction,
:: including without limitation the rights to use, copy, modify, merge,
:: publish, distribute, sublicense, and/or sell copies of the Software,
:: and to permit persons to whom the Software is furnished to do so,
:: subject to the following conditions:
::
:: The above copyright notice and this permission notice shall be included
:: in all copies or substantial portions of the Software.
::
:: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
:: OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
:: MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
:: IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
:: CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
:: TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
:: SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

:::::::::::::::::::
:: Modifications ::
:::::::::::::::::::

:: 1 - I Added the build tools (2022) to the list.
:: 2 - I prioritize the path check to the following:
::		* Community -> Build Tools -> Professional
::		* 2022 -> 2019 -> 2017 -> Older

SET "LIB="

::::::::::
:: 2022 ::
::::::::::
SET VC_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

::::::::::
:: 2019 ::
::::::::::
SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

::::::::::
:: 2017 ::
::::::::::
SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1))

:::::::::::
:: Older ::
:::::::::::
SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 13.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" %1))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" %1))


