Import('RTT_ROOT')
Import('rtconfig')
from building import *

src = []
cwd     = GetCurrentDir()
list = os.listdir(cwd)
CPPPATH = [cwd, str(Dir('#'))]

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_ACCE']):
    src += Glob('v_accelerometer.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_BARO']):
    src += Glob('v_barometer.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_FORCE']):
    src += Glob('v_force.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_GYRO']):
    src += Glob('v_gyroscope.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_HR']):
    src += Glob('v_heartRate.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_HUMI']):
    src += Glob('v_humidity.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_LIGHT']):
    src += Glob('v_light.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_MAG']):
    src += Glob('v_magnetometer.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_NOISE']):
    src += Glob('v_noise.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_PROX']):
    src += Glob('v_proximity.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_STEP']):
    src += Glob('v_stepcounter.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_TEMP']):
    src += Glob('v_temperature.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_TVOC']):
    src += Glob('v_tvoc.c')

if GetDepend(['PKG_USING_VIRTUAL_SENSOR_SPO2']):
    src += Glob('v_spo2.c')

group = DefineGroup('vsensor', src, depend = ['PKG_USING_VIRTUAL_SENSOR'], CPPPATH = CPPPATH)

Return('group')

