from building import *
import rtconfig

cwd = GetCurrentDir()
path = [cwd]
src = []

path += [
    cwd + '/inc']

# Host stack
src += Split('''
    src/bsal.c
    ''')

if GetDepend(['PKG_BSAL_SRV_BAS']):
	path += [cwd + '/profiles/service/basl_bas']
	src += Split("""
		profiles/service/basl_bas/bsal_srv_bas.c
		""")
		
if GetDepend(['PKG_BSAL_SRV_BLUFI']):
	path += [cwd + '/profiles/service/bsal_blufi']
	src += Split("""
		profiles/service/bsal_blufi/bsal_srv_blufi.c
		""")
		
if GetDepend(['PKG_BSAL_SAMPLE_BAS_ONLY']):
	src += Split("""
		samples/ble_bas_only_app.c
		""")
elif GetDepend(['PKG_BSAL_SAMPLE_BAS_BLUFI_COMBINE']):
    src += Split("""
		samples/ble_bas_blufi_app.c
		""")

		
if GetDepend(['PKG_BSAL_NIMBLE_STACK']):
	path += [cwd + '/port/nimble']
	src += Split("""
		port/nimble/bsal_nimble.c
		port/nimble/bsal_osif.c
		""")
		
elif GetDepend(['PKG_BSAL_RTK_STACK']):
	path += [cwd + '/port/realtek']
	src += Split("""
		port/realtek/bsal_rtk.c
		port/realtek/bsal_osif.c
		""")
		
elif GetDepend(['PKG_BSAL_EMPTY_STACK']):
	path += [cwd + '/port/empty_stack']
	src += Split("""
		port/empty_stack/bsal_osal_empty.c
		port/empty_stack/bsal_stack_empty.c
		""")
	
LOCAL_CCFLAGS = ''
    
group = DefineGroup('bsal', src, depend = ['PKG_USING_BSAL'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
