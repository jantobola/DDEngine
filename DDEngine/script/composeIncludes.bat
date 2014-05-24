set project_dir=%~1
set active_conf=%~2

if "%project_dir:~-1%"=="\" (
		set project_dir=%project_dir:~0,-1%
)

call:blank
call:highlight "Include composer script started"

rmdir "%project_dir%\include\include" /s /q
call:show "Dependency include folder cleaned"

mkdir "%project_dir%\include\include"

xcopy "%project_dir%\lib\AntTweakBar\include" "%project_dir%\include\include\" /s
xcopy "%project_dir%\lib\CEGUI\include" "%project_dir%\include\include\" /s
xcopy "%project_dir%\lib\DirectXTK\include" "%project_dir%\include\include\" /s
xcopy "%project_dir%\lib\Effects11\include" "%project_dir%\include\include\" /s

call:blank
call:highlight "Include composer script end"
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