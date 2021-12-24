from building import *

cwd     = GetCurrentDir()
src     = Glob('*.c') + Glob('*.cpp')
CPPPATH = [cwd]

group = DefineGroup('partition', src, depend = ['RT_USING_DFS', 'PKG_USING_PARTITION'], CPPPATH = CPPPATH)

Return('group')
