Import('RTT_ROOT')
Import('rtconfig')
from building import *

objs = []
cwd     = GetCurrentDir()
list = os.listdir(cwd)
src     = Glob('*.c') + Glob('*.cpp')
CPPPATH = [cwd, str(Dir('#'))]

group = DefineGroup('vconsole', src, depend = ['PKG_USING_VCONSOLE'], CPPPATH = CPPPATH)

Return('group')
