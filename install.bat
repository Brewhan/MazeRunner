for /f "delims=" %%a in ( 'PowerShell -NoProfile -ExecutionPolicy Bypass -Command "[Environment]::GetFolderPath('MyDocuments')" ') do set docs=%%a
ROBOCOPY /s .\Libs\libraries %docs%\Arduino
ROBOCOPY /s .\Libs\hardware %docs%\Arduino
ROBOCOPY /s .MazeRunnerBroken %docs%\Arduino
