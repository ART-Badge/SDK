Readme
======
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
