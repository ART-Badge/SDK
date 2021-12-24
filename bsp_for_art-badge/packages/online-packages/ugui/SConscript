import os
import rtconfig
from building import *

cwd = GetCurrentDir()

# add file
src = Split('''
port/ugui_port.c
source/ugui.c
widgets/ugui_progressbar.c
''')


path =  [cwd]
path += [cwd + '/port']
path += [cwd + '/source']
path += [cwd + '/widgets']

LOCAL_CCFLAGS = ''

group = DefineGroup('ugui', src, depend = ['PKG_USING_UGUI'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
