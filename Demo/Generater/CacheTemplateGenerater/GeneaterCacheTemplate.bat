@echo off
:: 进入批处理文件所在的文件夹
cd /d "%~dp0"

MysqlTemplateGenerater.exe --Host=127.0.0.1 --Port=3306 --Db=game --Username=root --Password=abc123123 --Template=%~dp0SqlCodeTemplate --Save=%~dp0../../Server/XGame/Interface/SQLCacheTemplate.h
pause