@echo off
:: 进入批处理文件所在的文件夹
cd /d "%~dp0"

del /Q "..\..\Client\Assets\GameUnit\Units\*.cs"
UnitGenerater.exe --UnitXmlPath=%~dp0GameObjectTemplate.xml --CppCodeTemplatePath=%~dp0CppCodeTemplate --CppUnitCodePath=%~dp0../../Server/XGame/Interface/
UnitGenerater.exe --UnitXmlPath=%~dp0GameObjectTemplate.xml --CSharpCodeTemplatePath=%~dp0CSharpCodeTemplate --CSharpUnitCodePath=%~dp0../../Client/Assets/GameUnit/Units/
pause