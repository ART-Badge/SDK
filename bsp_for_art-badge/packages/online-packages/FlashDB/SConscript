from building import *

# get current directory
cwd     = GetCurrentDir()
# The set of source files associated with this SConscript file.
src     = Glob('src/*.c')
path    = [cwd + '/inc']

if GetDepend(['FLASHDB_USING_SAMPLES']):
    src += Glob('samples/*.c')

if GetDepend(['RT_USING_UTEST']):
    src += Glob('tests/*.c')

group = DefineGroup('FlashDB', src, depend = ['PKG_USING_FLASHDB'], CPPPATH = path)

Return('group')
