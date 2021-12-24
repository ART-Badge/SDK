# RT-Thread building script for component

from building import *

cwd = GetCurrentDir()
CPPPATH = [cwd]
src = []
group = DefineGroup('TinyJPEG', src, depend = ['PKG_USING_TINYJPEG'], CPPPATH = CPPPATH)

Return('group')
