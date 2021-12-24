
from building import *
import rtconfig

cwd     = GetCurrentDir()
src     = Glob('*.c')
CPPPATH = [cwd]

group = DefineGroup('sys_load_monitor', src, depend = ['PKG_USING_SYS_LOAD_MONITOR'], CPPPATH = CPPPATH)

Return('group')
