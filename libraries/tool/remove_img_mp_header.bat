ECHO off
rem

::remove 512bytes MP Header
bash.exe -c "dd if=OTAHeader_Bank0_MP.bin of=OTAHeader_Bank0.bin bs=1 skip=512"

:: convert ota header bin to hex 
 .\srec_cat ".\OTAHeader_Bank0.bin"   -binary -offset 0x802000 -o ".\OTAHeader_Bank0.hex" -intel
 
::handle ota header1
::bash.exe -c "dd if=OTAHeader_Bank1_MP.bin of=OTAHeader_Bank1.bin bs=1 skip=512"
:: .\srec_cat ".\OTAHeader_Bank1.bin"   -binary -offset 0x84C000 -o ".\OTAHeader_Bank1.hex" -intel