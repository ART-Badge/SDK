# RT-Thread building script for component

Import('rtconfig')
from building import *

cwd = GetCurrentDir()
src = Glob('src/*.c')
CPPPATH = [cwd + '/inc']
LOCAL_CCFLAGS = ''

if rtconfig.CROSS_TOOL == 'keil':
    if rtconfig.PLATFORM == 'armcc':
        LOCAL_CCFLAGS += ' --gnu'
    elif rtconfig.PLATFORM == 'armclang':
        LOCAL_CCFLAGS += ' -std=gnu99'

group = DefineGroup('lpm', src, depend = ['PKG_USING_LPM'], CPPPATH = CPPPATH, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')