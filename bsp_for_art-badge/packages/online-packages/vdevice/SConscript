Import('RTT_ROOT')
Import('rtconfig')
from building import *

cwd     = GetCurrentDir()
src     = Glob('*.c') + Glob('*.cpp')
CPPPATH = [cwd, str(Dir('#'))]

group = DefineGroup('vdevice', src, depend = ['PKG_USING_VDEVICE'], CPPPATH = CPPPATH)

Return('group')
