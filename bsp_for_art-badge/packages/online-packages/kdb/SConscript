Import('RTT_ROOT')
Import('rtconfig')
from building import *

objs = []
cwd     = GetCurrentDir()
list = os.listdir(cwd)
src     = []
CPPPATH = []
CPPPATH += [cwd + '/inc']
src     += Glob('./src/*.c')

if GetDepend('KDB_TEST_ENABLE') == False:
    SrcRemove(src, ['./src/kdb_test.c'])

if GetDepend('KDB_ENABLE_CUSTOMIZE_OUTPUT') == False:
    src += Glob('./kdb_port.c')

group = DefineGroup('kdb', src, depend = ['PKG_USING_KDB'], CPPPATH = CPPPATH)

Return('group')
