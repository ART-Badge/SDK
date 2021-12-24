from building import *

cwd  = GetCurrentDir()
path = [cwd]

src  = Glob('thread_pool.c')

if GetDepend(['THREAD_POOL_USING_SAMPLES']):
    src += Glob('thread_pool_sample.c')

group = DefineGroup('thread_pool', src, depend = ['PKG_USING_THREAD_POOL'], CPPPATH = path)

Return('group')
