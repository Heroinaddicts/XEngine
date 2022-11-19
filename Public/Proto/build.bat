@echo off
cd /d %~dp0

FOR /R ./ %%F IN (*.proto) DO protoc --proto_path=%~dp0 --cpp_out=../../Server/XGame/Proto --csharp_out=../../Client/Assets/Scripts/Proto "%%F"

pause