@echo off
cls
echo Build started...
gcc vbs.S -o vbs.exe -nostdlib -Wl,-Tlink.ld
objcopy -O binary -j .text vbs.exe vbs.bin
del vbs.exe
echo Done.



