@echo off
set keil_compiler_include_floder=%1
set full_name_path=%2
set linker_output_file_name=%3
::echo %keil_compiler_include_floder%
::echo %full_name_path%
::echo %linker_output_file_name%

%keil_compiler_include_floder%\..\bin\fromelf.exe --bin -o  "bin/" "%full_name_path%"
%keil_compiler_include_floder%\..\bin\fromelf.exe -acd --interleave=source -o "bin\%linker_output_file_name%.disasm" "%full_name_path%"
::libraries\tool\prepend_header\prepend_header.exe -r "..\..\..\tool\keys\rtk_rsa.pem" -t app_code -p "bin\%linker_output_file_name%.bin" -m 1 -c sha256 -a "..\..\..\tool\key.json"
libraries\tool\prepend_header\prepend_header.exe -t app_code -p "bin\%linker_output_file_name%.bin" -m 1 -c sha256 -a "..\..\..\tool\key.json"
libraries\tool\MD5\md5.exe "bin\%linker_output_file_name%.bin"
libraries\tool\MD5\md5.exe "bin\%linker_output_file_name%_MP.bin"
del "bin\%linker_output_file_name%_MP.bin"