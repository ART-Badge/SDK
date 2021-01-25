1>NUL del .\user_data.bin
1>NUL del .\user_data_OTA.bin

..\prepend_header\prepend_header.exe -t app_data1 -p "raw_user_data.bin" -m 1 -c sha256 -b 9 -u 0xFFFE
1>NUL ren raw_user_data_MP.bin user_data_OTA.bin
..\md5\md5.exe "user_data_OTA.bin"

1>NUL ren raw_user_data.tmp user_data.bin





