set project_dir=%~1
set active_conf=%~2

if "%project_dir:~-1%"=="\" (
		set project_dir=%project_dir:~0,-1%
)

call:blank
call:highlight "DLL composer script started"

xcopy "%project_dir%\..\DDEngineRes\bin\*.dll" "%project_dir%\dll\%active_conf%\" /s /y

call:blank
call:highlight "DLL composer script end"
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