@echo off
gcc mbr.S -o mbr.exe -nostdlib -Wl,-Tlink.ld
objcopy -O binary -j .text mbr.exe mbr.bin
del mbr.exe
echo Done.



