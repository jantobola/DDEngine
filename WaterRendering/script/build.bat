set project_dir=%1
set active_conf=%2

if "%project_dir:~-1%"=="\" (
		set project_dir=%project_dir:~0,-1%
)

call:blank
call:highlight "Prebuild script for %active_conf% configuration started"

rmdir "%project_dir%\bin\%active_conf%" /s /q
call:show "Build folder cleaned"

set dllFolder=%active_conf%

if "%active_conf%"=="Debug_precompiled_shaders" (
		set dllFolder="Debug"
)

if "%active_conf%"=="Release_precompiled_shaders" (
		set dllFolder="Release"
)

xcopy "%project_dir%\lib\DDEngine\dll\%dllFolder%\*.dll" "%project_dir%\bin\%active_conf%\bin\" /s
xcopy "%project_dir%\res\*.*" "%project_dir%\bin\%active_conf%\bin\res\" /s
xcopy "%project_dir%\data\*" "%project_dir%\bin\%active_conf%\bin\"

if NOT "%active_conf%"=="Debug" (
	if NOT "%active_conf%"=="Debug_precompiled_shaders" (
		if NOT "%active_conf%"=="Release_precompiled_shaders" (
			xcopy "%project_dir%\src\shaders\*.*" "%project_dir%\bin\%active_conf%\bin\shaders\" /s
		)
	)
)

call:blank
call:highlight "Prebuild script end"
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