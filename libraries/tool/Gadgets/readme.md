Readme
======
md5.exe
==============
The *md5.exe* is a PC tool to make the output binary image file meets the
requirement of *MPTool*.

Usage
--------------
    md5 <filename>
        Add md5 checksum to file name

Notes
--------------
MPTool requires the binary file names should be ended with -[md5sum], it will
use md5sum to check whether the binary file is valid or not.

Examples
--------------
    1. Add md5 checksum to Patch binary
    D:\>md5 Patch.bin
    Output Image: Patch-381b3b002cfa300054142e01c0332e01.bin

    2. Add md5 checksum to APP binary
    D:\>md5 app.bin
    Output Image: app-381b6f00f4fc2c0054142e01c0332e01.bin


prepend_header.exe
==============
The *prepend_header.exe* is a PC tool to manipulate the image header and 
make the output binary image file meets the requirement of OTA or MP.

Version
--------------
v1.0.0

Usage
--------------
    prepend_header [/s,/c,/mp,/app_data,/dsp_data,/bkp_data1,/bkp_data2,
                   ,/app_data0,/app_data1,/app_data2,/app_data3] <filename>
        /s      Generate SHA256 and then prepend the image header
        /c      Generate CRC16 and then prepend the image header
        /mp     Generate a new image with MP header
        /app_data       Generate and append header for BBPro APP data
        /dsp_data       Generate and append header for BBPro DSP data
        /bkp_data1      Generate and append header for BBPro bkp data1
        /bkp_data2      Generate and append header for BBPro bkp data2
        /app_data0      Generate and append header for BEE2 APP data0
        /app_data1      Generate and append header for BEE2 APP data1
        /app_data2      Generate and append header for BEE2 APP data2
        /app_data3      Generate and append header for BEE2 APP data3

Notes
--------------
    All parameters are incompatible, do not use them at the same time
    except "/s /mp".

    The "/s" parameter also calculates CRC16, that is, CRC Check and
    SHA Check would both succeed when doing OTA in BBPro.

    The "/mp" parameter will firstly read the MP information from a
    initialization file named "mp.ini", and then generate a new image
    file with MP header according to it, and then read the image version
    pattern if the version.h have defined it, the finally output file 
    name is "{Image_Name}_MP-{Major}.{Minor}.{Rev}.{Build}_{CommitID}-{md5}.bin".

    The /app_data/dsp_data/bkp_data1/bkp_data2... parameter is used to
    insert corresponding image header for BBPro/Bee2 data image.
    Note that these parameters do not calculate CRC or SHA checksum,
    thus after appending the data image header, please use this tool
    again and pass "/c" or "/s" parameter according to your requirement.

Examples
--------------
    1) prepend_header /s app.bin
    Generate SHA256 and then prepend header of app.bin
    2) prepend_header /c app.bin
    Generate CRC16 and then prepend header of app.bin
    3) prepend_header /mp app.bin
    Generate a new image file with MP header, which name will be app_mp.bin
    4) prepend_header /s /mp app.bin
    Generate SHA256 and prepend header of app.bin, and then generate a new
    image file with MP header, which name will be app_MP-{version&md5}.bin
    5) prepend_header /app_data data.bin
       prepend_header /c data.bin
    Insert BBPro APP data image header and calculate CRC for data.bin
    6) prepend_header /dsp_data data.bin
       prepend_header /s data.bin
    Insert BBPro DSP data image header and calculate SHA for data.bin
    7) prepend_header /app_data1 data.bin
       prepend_header /c data.bin
    Insert Bee2 APP data1 image header and calculate CRC for data.bin