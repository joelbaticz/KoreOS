@echo off

cd c:\!PROGRAMMING\KOREOS\build\

echo Starting Build
make

echo Copying KORE.SYS as KLDR.SYS
copy /y kore.sys a:\kldr.sys

cd c:\!PROGRAMMING\KOREOS\run\
bochs -f bochconfig.bxrc -q

