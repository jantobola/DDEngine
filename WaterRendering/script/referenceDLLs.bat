set project_dir=%1
set active_conf=%2

if "%project_dir:~-1%"=="\" (
		set project_dir=%project_dir:~0,-1%
)

call:blank
call:highlight "Reference DLLs script for %active_conf% configuration started"

set dllFolder=%active_conf%

if "%active_conf%"=="Debug_Shaders" (
		set dllFolder="Debug"
)

if "%active_conf%"=="Release_Shaders" (
		set dllFolder="Release"
)

xcopy "%project_dir%\..\DDEngine\dll\%dllFolder%\*.dll" "%project_dir%\lib\DDEngine\dll\%active_conf%\" /s /y

call:blank
call:highlight "Reference DLLs script end"
call:blank
goto:eof

:show
echo.
echo %~1
echo.
goto:eof

:highlight
echo ====================================================
echo %~1
echo ====================================================
goto:eof

:blank
echo.
goto:eof