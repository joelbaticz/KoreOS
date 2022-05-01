@echo off

cd ..\build\

echo Copying KORE.SYS as KLDR.SYS
copy /y kore.sys a:\kldr.sys

cd ..\run\
bochs -f bochconfig.bxrc -q
