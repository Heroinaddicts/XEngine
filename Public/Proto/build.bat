@echo off
cd /d %~dp0

FOR /R ./ %%F IN (*.proto) DO protoc --proto_path=%~dp0 --cpp_out=./Cpp --csharp_out=./CSharp "%%F"

::xcopy .\csharp\Game.cs "..\client\Assets\Scripts\Proto\" /s/e/i/y
::xcopy .\csharp\Data.cs "..\client\Assets\Scripts\Proto\" /s/e/i/y
xcopy .\Cpp\* ..\..\XEngine\Demo\Proto /s/e/i/y

pause