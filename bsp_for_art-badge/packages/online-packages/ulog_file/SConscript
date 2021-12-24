Import('RTT_ROOT')
Import('rtconfig')
from building import *

objs = []
cwd     = GetCurrentDir()
list = os.listdir(cwd)
src     = Glob('*.c')
CPPPATH = [cwd, str(Dir('#'))]

group = DefineGroup('ulog_file', src, depend = ['PKG_USING_ULOG_FILE'], CPPPATH = CPPPATH)

Return('group')
