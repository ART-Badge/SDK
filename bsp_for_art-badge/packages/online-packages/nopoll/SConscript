import os
import rtconfig
from building import *

cwd     = GetCurrentDir()
src     = Split('''
nopoll/nopoll.c
nopoll/nopoll_conn_opts.c
nopoll/nopoll_ctx.c
nopoll/nopoll_decl.c
nopoll/nopoll_io.c
nopoll/nopoll_listener.c
nopoll/nopoll_log.c
nopoll/nopoll_loop.c
nopoll/nopoll_msg.c
nopoll/nopoll_conn.c
nopoll/nopoll_rtthread.c
''')

CPPPATH = [cwd + '/nopoll', cwd + '/sha1']

LOCAL_CCFLAGS = ''

if rtconfig.CROSS_TOOL == 'keil':
    LOCAL_CCFLAGS += ' --gnu'

group = DefineGroup('nopoll', src, depend = ['RT_USING_LWIP', 'PKG_USING_NOPOLL', 'PKG_USING_TINYCRYPT'], CPPPATH = CPPPATH, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

examples_src = Split('''
examples/nopoll_client.c
''')

group = group + DefineGroup('nopoll-examples', examples_src, depend = ['PKG_USING_NOPOLL', 'PKG_USING_NOPOLL_EXAMPLE'], LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
