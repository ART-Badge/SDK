@echo off
::set iar_compiler_floder=%1
set linker_output_file_name=%1
::echo %iar_compiler_floder%
::echo %linker_output_file_name%

ielfdumparm.exe --source --code "bin\%linker_output_file_name%.out" -o "bin\%linker_output_file_name%.disasm"
..\..\..\tool\hex2bin\Hex2Bin.exe "bin\%linker_output_file_name%.hex" "bin\%linker_output_file_name%"
..\..\..\tool\prepend_header\prepend_header.exe -t app_code -p "bin\%linker_output_file_name%.bin" -m 1 -c sha256 -a "..\..\..\tool\key.json"
..\..\..\tool\MD5\md5.exe "bin\%linker_output_file_name%.bin"
..\..\..\tool\MD5\md5.exe "bin\%linker_output_file_name%_MP.bin"
del "bin\%linker_output_file_name%_MP.bin"