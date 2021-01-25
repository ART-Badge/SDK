/* JS API For Sensors */

var dcmlib = require("dcm");
var emqlib = require("emq");
var data_pool;

/* sensor : emq command list */
/* Accelerometer */
var ID_ACCE_ON = 0x0301;
var ID_ACCE_ID = 0x0302;
var ID_ACCE_OFF = 0x0304;
var ID_ACCE_PERIOD = 0x0305;

/* Gyroscope */
var ID_GYRO_ON = 0x0311;
var ID_GYRO_ID = 0x0312;
var ID_GYRO_OFF = 0x0314;
var ID_GYRO_PERIOD = 0x0315;

/* Magnetometer */
var ID_MAG_ON = 0x0321;
var ID_MAG_ID = 0x0322;
var ID_MAG_OFF = 0x0324;
var ID_MAG_PERIOD = 0x0325;

/* Temperature */
var ID_TEMP_ON = 0x0331;
var ID_TEMP_ID = 0x0332;
var ID_TEMP_OFF = 0x0334;
var ID_TEMP_PERIOD = 0x0335;

/* Humidity */
var ID_HUMI_ON = 0x0341;
var ID_HUMI_ID = 0x0342;
var ID_HUMI_OFF = 0x0344;
var ID_HUMI_PERIOD = 0x0345;

/* Barometer */
var ID_BARO_ON = 0x0351;
var ID_BARO_ID = 0x0352;
var ID_BARO_OFF = 0x0354;
var ID_BARO_PERIOD = 0x0355;

/* Light */
var ID_LIGHT_ON = 0x0361;
var ID_LIGHT_ID = 0x0362;
var ID_LIGHT_OFF = 0x0364;
var ID_LIGHT_PERIOD = 0x0365;

/* proximity */
var ID_PROX_ON = 0x0371;
var ID_PROX_ID = 0x0372;
var ID_PROX_OFF = 0x0374;
var ID_PROX_PERIOD = 0x0375;

/* heart rate */
var ID_HR_ON = 0x0381;
var ID_HR_ID = 0x0382;
var ID_HR_OFF = 0x0384;
var ID_HR_PERIOD = 0x0385;

/* TVOC */
var ID_TVOC_ON = 0x0391;
var ID_TVOC_ID = 0x0392;
var ID_TVOC_OFF = 0x0394;
var ID_TVOC_PERIOD = 0x0395;

/* NOISE */
var ID_NOISE_ON = 0x03A1;
var ID_NOISE_ID = 0x03A2;
var ID_NOISE_OFF = 0x03A4;
var ID_NOISE_PERIOD = 0x03A5;

/* step counter */
var ID_STEP_ON = 0x03B1;
var ID_STEP_ID = 0x03B2;
var ID_STEP_OFF = 0x03B4;
var ID_STEP_PERIOD = 0x03B5;

/* force */
var ID_FORCE_ON = 0x03C1;
var ID_FORCE_ID = 0x03C2;
var ID_FORCE_OFF = 0x03C4;
var ID_FORCE_PERIOD = 0x03C5;

/* SPO2 */
var ID_SPO2_ON = 0x03D1;
var ID_SPO2_ID = 0x03D2;
var ID_SPO2_OFF = 0x03D4;
var ID_SPO2_PERIOD = 0x03D5;

/* sensor : emq channel list */
var emq_acce_channel = "sens.0"; /* Accelerometer */
var emq_gyro_channel = "sens.1"; /* Gyroscope */
var emq_mag_channel = "sens.2"; /* Magnetometer */
var emq_temp_channel = "sens.3"; /* Temperature */
var emq_humi_channel = "sens.4"; /* Humidity */
var emq_baro_channel = "sens.5"; /* Barometer */
var emq_light_channel = "sens.6"; /* Light */
var emq_prox_channel = "sens.7"; /* Proximity */
var emq_hr_channel = "sens.8"; /* HeartRate */
var emq_tvoc_channel = "sens.9"; /* TVOC */
var emq_noise_channel = "sens.10"; /* NOISE */
var emq_step_channel = "sens.11"; /* StepCounter */
var emq_force_channel = "sens.12"; /* Force */
var emq_spo2_channel = "sens.13"; /* SPO2 */

/* sensor : dcm pool name */
var dcm_pool_name = "sens";

/* sensor : dcm key list */
var dcm_acce_id_name = "acceid"; /* Accelerometer */
var dcm_acce_data_name = "accedata";
var dcm_acce_period_name = "acceperiod"
var dcm_gyro_id_name = "gyroid"; /* Gyroscope */
var dcm_gyro_data_name = "gyrodata";
var dcm_gyro_period_name = "gyroperiod"
var dcm_mag_id_name = "magid"; /* Magnetometer */
var dcm_mag_data_name = "magdata";
var dcm_mag_period_name = "magperiod"
var dcm_temp_id_name = "tempid"; /* Temperature */
var dcm_temp_data_name = "tempdata";
var dcm_temp_period_name = "tempperiod"
var dcm_humi_id_name = "humiid"; /* Humidity */
var dcm_humi_data_name = "humidata";
var dcm_humi_period_name = "humiperiod"
var dcm_baro_id_name = "baroid"; /* Barometer */
var dcm_baro_data_name = "barodata";
var dcm_baro_period_name = "baroperiod"
var dcm_light_id_name = "lightid"; /* Light */
var dcm_light_data_name = "lightdata";
var dcm_light_period_name = "lightperiod"
var dcm_prox_id_name = "proxid"; /* Proximity */
var dcm_prox_data_name = "proxdata";
var dcm_prox_period_name = "proxperiod"
var dcm_heartrate_id_name = "hrid"; /* HeartRate */
var dcm_heartrate_data_name = "hrdata";
var dcm_heartrate_period_name = "hrperiod"
var dcm_tvoc_id_name = "tvocid"; /* TVOC */
var dcm_tvoc_data_name = "tvocdata";
var dcm_tvoc_period_name = "tvocperiod"
var dcm_noise_id_name = "noiseid"; /* NOISE */
var dcm_noise_data_name = "noisedata";
var dcm_noise_period_name = "noiseperiod"
var dcm_step_id_name = "stepid"; /* StepCounter */
var dcm_step_data_name = "stepdata";
var dcm_step_period_name = "stepperiod"
var dcm_force_id_name = "forceid"; /* Force */
var dcm_force_data_name = "forcedata";
var dcm_force_period_name = "forceperiod"
var dcm_spo2_id_name = "spo2id"; /* SPO2 */
var dcm_spo2_data_name = "spo2data";
var dcm_spo2_period_name = "spo2period"

/* sensor : dcm onChange callback functions list */
var acce_func_arr = []; /* Accelerometer */
var gyro_func_arr = []; /* Gyroscope */
var mag_func_arr = []; /* Magnetometer */
var temp_func_arr = []; /* Temperature */
var humidity_func_arr = []; /* Humidity */
var baro_func_arr = []; /* Barometer */
var light_func_arr = []; /* Light */
var prox_func_arr = []; /* Proximity */
var heartRate_func_arr = []; /* HeartRate */
var tvoc_func_arr = []; /* TVOC */
var noise_func_arr = []; /* NOISE */
var step_func_arr = []; /* StepCounter */
var force_func_arr = []; /* Force */
var spo2_func_arr = []; /* SPO2 */

/* 开启监听：DCM 数据 改变 */
function dcmSensOnChange(func_arr, onChange, dcm_name)
{
    var obj =
    {
        fun : null,
        pool : null
    };

    if (func_arr == null)
    {
        console.log("dcmSensOnChange: func_arr is null!")
        return false;
    }

    if (onChange == null)
    {
        console.log("dcmSensOnChange: onChange is null!")
        return false;
    }

    if (dcm_name == null)
    {
        console.log("dcmSensOnChange: dcm_name is null!")
        return false;
    }

    func_arr.forEach(function(x, index, a)
    {
        if (onChange == func_arr[index].fun)
        {
            console.log("dcm onChange func exists!")
            return true;
        }
    })

    obj.fun = onChange
    obj.pool = dcmlib.Open(dcm_pool_name);
    obj.pool.onChange(dcm_name, onChange);

    func_arr.push(obj);
    console.log("dcm add onChange OK!")

    return true;
}

/* 取消监听：DCM 数据 改变 */
function dcmSensOffChange(func_arr, onChange)
{
    console.log("dcmOffChange")

    if (func_arr == null)
    {
        console.log("dcmSensOnChange: func_arr is null!")
        return false;
    }

    if (onChange == null)
    {
        console.log("dcmSensOnChange: onChange is null!")
        return false;
    }

    func_arr.forEach(function(x, index, a)
    {
        if (onChange == func_arr[index].fun)
        {
            func_arr.splice(index, 1);
            console.log("dcmSensOnChange: func_arr removed!")
            return true;
        }
    })

    return true;
}

/* sensor:设置采样周期：period：0： 时停止采样； > 1：采样时间； */
function setSensorPeriod(emq_sens_name, emq_sens_cmd, dcm_sens_name, samplePeriod)
{
    data_pool = dcmlib.Open(dcm_pool_name);

    if(data_pool == null)
    {
        console.log("[setSensorPeriod] : data_pool is null!");
        return null;
    }

    /* set sensor sample period to dcm */
    data_pool.setItem(dcm_sens_name, samplePeriod);
    /* send emq cmd to sensor device */
    emqlib.send(emq_sens_name, emq_sens_cmd);

    return 0;
}

/* 设置计步采样周期 */
function setStepCounterSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_step_channel, ID_STEP_PERIOD, dcm_step_period_name, samplePeriod);
}

/* 获取计步 */
function getStepCounter()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var step = data_pool.getItem(dcm_step_data_name);
    if (step != null)
    {
        return step;
    }

    //console.log("getStepCounter");

    return 0;
}

/* 获取计步传感器ID */
function getStepCounterId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_step_channel, ID_STEP_ID);
    var step = data_pool.getItem(dcm_step_id_name);
    if (step != null)
    {
        return step;
    }

    //console.log("getStepCounterId");

    return 0;
}

/* 开启计步 */
function openStepCounter()
{
    emqlib.send(emq_step_channel, ID_STEP_ON);

    //console.log("openStepCounter");

    return true;
}

/* 关闭计步 */
function closeStepCounter()
{
    emqlib.send(emq_step_channel, ID_STEP_OFF);

    //console.log("closeStepCounter");

    return true;
}

/* 开启监听：计步变化 */
function onStepChange(onChange)
{
    //console.log("onStepChange");
    return dcmSensOnChange(step_func_arr, onChange, dcm_step_data_name);
}

/* 关闭监听：计步变化 */
function offStepChange(onChange)
{
    //console.log("offStepChange");
    return dcmSensOffChange(step_func_arr, onChange);
}

/* 设置心率采样周期 */
function setHeartRateSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_hr_channel, ID_HR_PERIOD, dcm_heartrate_period_name, samplePeriod);
}

/* 获取心率 */
function getHeartRate()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var step = data_pool.getItem(dcm_heartrate_data_name);
    if (step != null)
    {
        return step;
    }

    //console.log("getHeartRate");

    return 0;
}

/* 获取心率传感器ID */
function getHeartRateId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_hr_channel, ID_HR_ID);
    var step = data_pool.getItem(dcm_heartrate_id_name);
    if (step != null)
    {
        return step;
    }

    //console.log("getHeartRateId");

    return 0;
}

/* 打开心率传感器 */
function openHeartRateDetect()
{
    emqlib.send(emq_hr_channel, ID_HR_ON);

    //console.log("openHeartRateDetect");

    return true;
}

/* 关闭心率心率传感器 */
function closeHeartRateDetect()
{
    emqlib.send(emq_hr_channel, ID_HR_OFF);

    //console.log("closeHeartRateDetect");

    return true;
}

/* 开启监听：心率变化 */
function onHeartRateChange(onChange)
{
    //console.log("onHeartRateChange");
    return dcmSensOnChange(heartRate_func_arr, onChange, dcm_heartrate_data_name);
}

/* 关闭监听：心率变化 */
function offHeartRateChange(onChange)
{
    //console.log("offHeartRateChange");
    return dcmSensOffChange(heartRate_func_arr, onChange);
}

/* 设置温度采样周期 */
function setTemperatureSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_temp_channel, ID_TEMP_PERIOD, dcm_temp_period_name, samplePeriod);
}

/* 打开温度传感器 */
function openTemperature()
{
    emqlib.send(emq_temp_channel, ID_TEMP_ON);

    //console.log("openTemperature");

    return true;
}

/* 关闭温度传感器 */
function closeTemperature()
{
    emqlib.send(emq_temp_channel, ID_TEMP_OFF);

    //console.log("closeTemperature");

    return true;
}

/* 获取温度 */
function getTemperature()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var temp = data_pool.getItem(dcm_temp_data_name);

    //console.log("getTemperature");
    if (temp != null)
    {
        return temp;
    }

    return 0;
}

/* 获取温度 */
function getTemperatureId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_temp_channel, ID_TEMP_ID);
    var temp = data_pool.getItem(dcm_temp_id_name);

    //console.log("getTemperatureId");
    if (temp != null)
    {
        return temp;
    }

    return 0;
}

/* 开启监听：温度变化 */
function onTemperatureChange(onChange)
{
    //console.log("onTemperatureChange");
    return dcmSensOnChange(temp_func_arr, onChange, dcm_temp_data_name);
}

/* 关闭监听：温度变化 */
function offTemperatureChange(onChange)
{
    //console.log("offTemperatureChange");
    return dcmSensOffChange(temp_func_arr, onChange);
}

/* 设置湿度采样周期 */
function setHumiditySameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_humi_channel, ID_HUMI_PERIOD, dcm_humi_period_name, samplePeriod);
}

/* 打开湿度传感器 */
function openHumidity()
{
    emqlib.send(emq_humi_channel, ID_HUMI_ON);

    //console.log("openHumidity");

    return true;
}

/* 关闭湿度传感器 */
function closeHumidity()
{
    emqlib.send(emq_humi_channel, ID_HUMI_OFF);

    //console.log("closeHumidity");

    return true;
}

/* 获取湿度 */
function getHumidity()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var humi = data_pool.getItem(dcm_humi_data_name);

    //console.log("getHumidity");
    if (humi != null)
    {
        return humi;
    }

    return 0;
}

/* 获取湿度传感器ID */
function getHumidityId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_humi_channel, ID_HUMI_ID);
    var humi = data_pool.getItem(dcm_humi_id_name);

    //console.log("getHumidityId");
    if (humi != null)
    {
        return humi;
    }

    return 0;
}

/* 开启监听：湿度变化 */
function onHumidityChange(onChange)
{
    //console.log("onHumidityChange");
    return dcmSensOnChange(humidity_func_arr, onChange, dcm_humi_data_name);
}

/* 关闭监听：湿度变化 */
function offHumidityChange(onChange)
{
    //console.log("offHumidityChange");
    return dcmSensOffChange(humidity_func_arr, onChange);
}

/* 设置亮度采样周期 */
function setLightSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_light_channel, ID_LIGHT_PERIOD, dcm_light_period_name, samplePeriod);
}

/* 获取亮度 */
function getLight()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var ret = data_pool.getItem(dcm_light_data_name);

    //console.log("getLight");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取亮度传感器ID */
function getLightId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_light_channel, ID_LIGHT_ID);
    var ret = data_pool.getItem(dcm_light_id_name);

    //console.log("getLightId");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 开启亮度检测 */
function openLightDetect()
{
    emqlib.send(emq_light_channel, ID_LIGHT_ON);

    //console.log("openLightDetect");

    return true;
}

/* 关闭亮度检测 */
function closeLightDetect()
{
    emqlib.send(emq_light_channel, ID_LIGHT_OFF);

    //console.log("closeLightDetect");

    return true;
}

/* 开启监听：亮度变化 */
function onLightChange(onChange)
{
    //console.log("onLightChange");
    return dcmSensOnChange(light_func_arr, onChange, dcm_light_data_name);
}

/* 关闭监听：亮度变化 */
function offLightChange(onChange)
{
    //console.log("offLightChange");
    return dcmSensOffChange(light_func_arr, onChange);
}

/* 设置气压采样周期 */
function setBarometerSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_baro_channel, ID_BARO_PERIOD, dcm_baro_period_name, samplePeriod);
}

/* 获取大气压值 */
function getBarometer()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var ret = data_pool.getItem(dcm_baro_data_name);

    //console.log("getBarometer");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取气压计ID */
function getBarometerId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_baro_channel, ID_BARO_ID);
    var ret = data_pool.getItem(dcm_baro_id_name);

    //console.log("getBarometerId");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 开启大气压检测 */
function openBarometer()
{
    emqlib.send(emq_baro_channel, ID_BARO_ON);

    //console.log("openBarometer");

    return true;
}

/* 关闭大气压检测 */
function closeBarometer()
{
    emqlib.send(emq_baro_channel, ID_BARO_OFF);

    //console.log("closeBarometer");

    return true;
}

/* 开启监听：大气压变化 */
function onBarometerChange(onChange)
{
    //console.log("onBarometerChange");
    return dcmSensOnChange(baro_func_arr, onChange, dcm_baro_data_name);
}

/* 关闭监听：大气压变化 */
function offBarometerChange(onChange)
{
    //console.log("offBarometerChange");
    return dcmSensOffChange(baro_func_arr, onChange);
}

/* 设置磁传感器采样周期 */
function setMagnetometerSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_mag_channel, ID_MAG_PERIOD, dcm_mag_period_name, samplePeriod);
}

/* 获取磁传感器数据 */
function getMagnetometer()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var ret = data_pool.getItem(dcm_mag_data_name);

    //console.log("getMagnetometer");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取磁传感器ID */
function getMagnetometerId()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_mag_channel, ID_MAG_ID);
    var ret = data_pool.getItem(dcm_mag_id_name);

    //console.log("getMagnetometerId");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 打开罗盘 */
function openMagnetometer()
{
    emqlib.send(emq_mag_channel, ID_MAG_ON);

    //console.log("openMagnetometer");

    return true;
}

/* 关闭罗盘 */
function closeMagnetometer()
{
    emqlib.send(emq_mag_channel, ID_MAG_OFF);

    //console.log("closeMagnetometer");

    return true;
}

/* 开启监听：罗盘数据改变 */
function onMagnetometerChange(onChange)
{
    //console.log("onMagnetometerChange");
    return dcmSensOnChange(mag_func_arr, onChange, dcm_mag_data_name);
}

/* 关闭监听：罗盘数据改变 */
function offMagnetometerChange(onChange)
{
    //console.log("offMagnetometerChange");
    return dcmSensOffChange(mag_func_arr, onChange);
}

/* 设置SPO2采样周期 */
function setSPO2SameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_spo2_channel, ID_SPO2_PERIOD, dcm_spo2_period_name, samplePeriod);
}

/* 获取血氧数据 */
function getSPO2()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var ret = data_pool.getItem(dcm_spo2_data_name);

    //console.log("getSPO2");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取血氧传感器ID */
function getSPO2Id()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    emqlib.send(emq_spo2_channel, ID_SPO2_ID);
    var ret = data_pool.getItem(dcm_spo2_id_name);

    //console.log("getSPO2Id");
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 开启血氧检测 */
function openSPO2()
{
    emqlib.send(emq_spo2_channel, ID_SPO2_ON);

    //console.log("openSPO2");

    return true;
}

/* 关闭血氧检测 */
function closeSPO2()
{
    emqlib.send(emq_spo2_channel, ID_SPO2_OFF);

    //console.log("closeSPO2");

    return true;
}

/* 开启监听：血氧数据变化 */
function onSPO2Change(onChange)
{
    //console.log("onSPO2Change");
    return dcmSensOnChange(spo2_func_arr, onChange, dcm_spo2_data_name);
}

/* 关闭监听：血氧数据变化 */
function offSPO2Change(onChange)
{
    //console.log("onSPO2Change");
    return dcmSensOffChange(spo2_func_arr, onChange);
}

/* 设置Accelerometer采样周期 */
function setAccelerometerSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_acce_channel, ID_ACCE_PERIOD, dcm_acce_period_name, samplePeriod);
}

/* 获取加速度ID */
function getAccelerometerId()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    emqlib.send(emq_acce_channel, ID_ACCE_ID);
    var step = data_pool.getItem(dcm_acce_id_name);
    if (step != null)
    {
        return step;
    }

    return 0;
}

/* 获取加速度数据 */
function getAccelerometerData()
{
    //emqlib.send(emq_acce_channel, ID_ACCE_DATA);

    data_pool = dcmlib.Open(dcm_pool_name);

    var acce_data = data_pool.getItem(dcm_acce_data_name);
    if (acce_data == null)
    {
        acce_data = [0,0,0];
    }

    return acce_data;
}

/* 开启加速度传感器 */
function openAccelerometer()
{
    emqlib.send(emq_acce_channel, ID_ACCE_ON);

    //console.log("openAccelerometer");

    return true;
}

/* 关闭加速度传感器 */
function closeAccelerometer()
{
    emqlib.send(emq_acce_channel, ID_ACCE_OFF);

    //console.log("closeAccelerometer");

    return true;
}

/* 开启监听：加速度变化 */
function onAccelerometerDataChange(onChange)
{
    console.log("onAccelerometerDataChange");
    var ret = dcmSensOnChange(acce_func_arr, onChange, dcm_acce_data_name);
    //console.dir(acce_func_arr);
    return ret;
}

/* 关闭监听：加速度变化 */
function offAccelerometerDataChange(onChange)
{
    console.log("offAccelerometerDataChange");
    var ret = dcmSensOffChange(acce_func_arr, onChange);
    //console.dir(acce_func_arr);
    return ret;
}

/* 设置Gyroscope采样周期 */
function setGyroscopeSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_gyro_channel, ID_GYRO_PERIOD, dcm_gyro_period_name, samplePeriod);
}

/* 获取陀螺仪传感器ID */
function getGyroscopeId()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    emqlib.send(emq_gyro_channel, ID_GYRO_ID);
    var ret = data_pool.getItem(dcm_gyro_id_name);
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取陀螺仪数据 */
function getGyroscope()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    var sens_data = data_pool.getItem(dcm_gyro_data_name);
    if (sens_data == null)
    {
        sens_data = [0,0,0];
    }

    //console.dir(sens_data);
    return sens_data;
}

/* 开启陀螺仪传感器 */
function openGyroscope()
{
    emqlib.send(emq_gyro_channel, ID_GYRO_ON);

    return true;
}

/* 关闭陀螺仪传感器 */
function closeGyroscope()
{
    emqlib.send(emq_gyro_channel, ID_GYRO_OFF);

    return true;
}

/* 开启监听：陀螺仪数据变化 */
function onGyroscopeChange(onChange)
{
    console.log("onGyroscopeChange");
    var ret = dcmSensOnChange(gyro_func_arr, onChange, dcm_gyro_data_name);

    return ret;
}

/* 关闭监听：陀螺仪数据变化 */
function offGyroscopeChange(onChange)
{
    console.log("offGyroscopeChange");
    var ret = dcmSensOffChange(gyro_func_arr, onChange);

    return ret;
}

/* 设置TVOC采样周期 */
function setTVOCSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_tvoc_channel, ID_TVOC_PERIOD, dcm_tvoc_period_name, samplePeriod);
}

/* 获取 TVOC ID */
function getTVOCId()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    emqlib.send(emq_tvoc_channel, ID_TVOC_ID);
    var ret = data_pool.getItem(dcm_tvoc_id_name);
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取TVOC数据 */
function getTVOC()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    var sens_data = data_pool.getItem(dcm_tvoc_data_name);
    if (sens_data != null)
    {
        return sens_data;
    }

    return 0;
}

/* 开启TVOC感器 */
function openTVOC()
{
    emqlib.send(emq_tvoc_channel, ID_TVOC_ON);

    return true;
}

/* 关闭TVOC */
function closeTVOC()
{
    emqlib.send(emq_tvoc_channel, ID_TVOC_OFF);

    return true;
}

/* 开启监听：TVOC数据变化 */
function onTVOCChange(onChange)
{
    //console.log("onTVOCChange");
    var ret = dcmSensOnChange(tvoc_func_arr, onChange, dcm_tvoc_data_name);

    return ret;
}

/* 关闭监听：TVOC数据变化 */
function offTVOCChange(onChange)
{
    //console.log("offTVOCChange");
    var ret = dcmSensOffChange(tvoc_func_arr, onChange);

    return ret;
}

/* 设置proximity采样周期 */
function setProximitySameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_prox_channel, ID_PROX_PERIOD, dcm_prox_period_name, samplePeriod);
}

/* 获取 proximity ID */
function getProximityId()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    emqlib.send(emq_prox_channel, ID_PROX_ID);
    var ret = data_pool.getItem(dcm_prox_id_name);
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取proximity数据 */
function getProximity()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    var sens_data = data_pool.getItem(dcm_prox_data_name);
    if (sens_data != null)
    {
        return sens_data;
    }

    //console.dir(sens_data);
    return 0;
}

/* 开启proximity感器 */
function openProximity()
{
    emqlib.send(emq_prox_channel, ID_PROX_ON);

    return true;
}

/* 关闭Tproximity */
function closeProximity()
{
    emqlib.send(emq_prox_channel, ID_PROX_OFF);

    return true;
}

/* 开启监听：proximity 数据变化 */
function onProximityChange(onChange)
{
    //console.log("onProximityChange");
    var ret = dcmSensOnChange(prox_func_arr, onChange, dcm_prox_data_name);

    return ret;
}

/* 关闭监听：proximity 数据变化 */
function offProximityChange(onChange)
{
    //console.log("offProximityChange");
    var ret = dcmSensOffChange(prox_func_arr, onChange);

    return ret;
}

/* 设置Noise采样周期 */
function setNoiseSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_noise_channel, ID_NOISE_PERIOD, dcm_noise_period_name, samplePeriod);
}

/* 获取 Noise ID */
function getNoiseId()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    emqlib.send(emq_noise_channel, ID_NOISE_ID);
    var ret = data_pool.getItem(dcm_noise_id_name);
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取Noise数据 */
function getNoise()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    var sens_data = data_pool.getItem(dcm_noise_data_name);
    if (sens_data != null)
    {
        return sens_data;
    }

    return 0;
}

/* 开启Noise感器 */
function openNoise()
{
    emqlib.send(emq_noise_channel, ID_NOISE_ON);

    return true;
}

/* 关闭Noise */
function closeNoise()
{
    emqlib.send(emq_noise_channel, ID_NOISE_OFF);

    return true;
}

/* 开启监听：Noise数据变化 */
function onNoiseChange(onChange)
{
    var ret = dcmSensOnChange(noise_func_arr, onChange, dcm_noise_data_name);

    return ret;
}

/* 关闭监听：Noise数据变化 */
function offNoiseChange(onChange)
{
    var ret = dcmSensOffChange(noise_func_arr, onChange);

    return ret;
}

/* 设置Force采样周期 */
function setForceSameplePeriod(samplePeriod)
{
    setSensorPeriod(emq_force_channel, ID_FORCE_PERIOD, dcm_force_period_name, samplePeriod);
}

/* 获取 Force ID */
function getForceId()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    emqlib.send(emq_force_channel, ID_FORCE_ID);
    var ret = data_pool.getItem(dcm_force_id_name);
    if (ret != null)
    {
        return ret;
    }

    return 0;
}

/* 获取Force数据 */
function getForce()
{
    data_pool = dcmlib.Open(dcm_pool_name);

    var sens_data = data_pool.getItem(dcm_force_data_name);
    if (sens_data != null)
    {
        return sens_data;
    }

    return [0];
}

/* 开启Force感器 */
function openForce()
{
    emqlib.send(emq_force_channel, ID_FORCE_ON);

    return true;
}

/* 关闭Force */
function closeForce()
{
    emqlib.send(emq_force_channel, ID_FORCE_OFF);

    return true;
}

/* 开启监听：Force数据变化 */
function onForceChange(onChange)
{
    var ret = dcmSensOnChange(force_func_arr, onChange, dcm_force_data_name);

    return ret;
}

/* 关闭监听：Force数据变化 */
function offForceChange(onChange)
{
    var ret = dcmSensOffChange(force_func_arr, onChange);

    return ret;
}

module.exports =
{
    getAccelerometerId:getAccelerometerId,
    getAccelerometerData:getAccelerometerData,
    openAccelerometer:openAccelerometer,
    closeAccelerometer:closeAccelerometer,
    setAccelerometerSameplePeriod: setAccelerometerSameplePeriod,
    onAccelerometerDataChange:onAccelerometerDataChange,
    offAccelerometerDataChange:offAccelerometerDataChange,
    getGyroscopeId:getGyroscopeId,
    getGyroscope:getGyroscope,
    openGyroscope:openGyroscope,
    closeGyroscope:closeGyroscope,
    setGyroscopeSameplePeriod: setGyroscopeSameplePeriod,
    onGyroscopeChange:onGyroscopeChange,
    offGyroscopeChange:offGyroscopeChange,
    getMagnetometerId:getMagnetometerId,
    getMagnetometer: getMagnetometer,
    openMagnetometer: openMagnetometer,
    closeMagnetometer: closeMagnetometer,
    setMagnetometerSameplePeriod: setMagnetometerSameplePeriod,
    onMagnetometerChange: onMagnetometerChange,
    offMagnetometerChange: offMagnetometerChange,
    getTemperatureId:getTemperatureId,
    getTemperature: getTemperature,
    openTemperature:openTemperature,
    closeTemperature:closeTemperature,
    setTemperatureSameplePeriod: setTemperatureSameplePeriod,
    onTemperatureChange: onTemperatureChange,
    offTemperatureChange: offTemperatureChange,
    getHumidityId: getHumidityId,
    getHumidity: getHumidity,
    openHumidity: openHumidity,
    closeHumidity:closeHumidity,
    setHumiditySameplePeriod: setHumiditySameplePeriod,
    onHumidityChange: onHumidityChange,
    offHumidityChange: offHumidityChange,
    getBarometerId:getBarometerId,
    getBarometer: getBarometer,
    openBarometer: openBarometer,
    closeBarometer: closeBarometer,
    setBarometerSameplePeriod: setBarometerSameplePeriod,
    onBarometerChange: onBarometerChange,
    offBarometerChange: offBarometerChange,
    getLightId:getLightId,
    getLight: getLight,
    openLightDetect: openLightDetect,
    closeLightDetect: closeLightDetect,
    setLightSameplePeriod: setLightSameplePeriod,
    onLightChange: onLightChange,
    offLightChange: offLightChange,
    getProximityId:getProximityId,
    getProximity:getProximity,
    openProximity:openProximity,
    closeProximity:closeProximity,
    setProximitySameplePeriod: setProximitySameplePeriod,
    onProximityChange:onProximityChange,
    offProximityChange:offProximityChange,
    getHeartRateId:getHeartRateId,
    getHeartRate: getHeartRate,
    openHeartRateDetect: openHeartRateDetect,
    closeHeartRateDetect: closeHeartRateDetect,
    setHeartRateSameplePeriod: setHeartRateSameplePeriod,
    onHeartRateChange: onHeartRateChange,
    offHeartRateChange: offHeartRateChange,
    getTVOCId:getTVOCId,
    getTVOC:getTVOC,
    openTVOC:openTVOC,
    closeTVOC:closeTVOC,
    setTVOCSameplePeriod: setTVOCSameplePeriod,
    onTVOCChange:onTVOCChange,
    offTVOCChange:offTVOCChange,
    getNoiseId:getNoiseId,
    getNoise:getNoise,
    openNoise:openNoise,
    closeNoise:closeNoise,
    setNoiseSameplePeriod: setNoiseSameplePeriod,
    onNoiseChange:onNoiseChange,
    offNoiseChange:offNoiseChange,
    getStepCounterId:getStepCounterId,
    getStepCounter: getStepCounter,
    openStepCounter: openStepCounter,
    closeStepCounter: closeStepCounter,
    setStepCounterSameplePeriod: setStepCounterSameplePeriod,
    onStepChange: onStepChange,
    offStepChange: offStepChange,
    getForceId:getForceId,
    getForce:getForce,
    openForce:openForce,
    closeForce:closeForce,
    setForceSameplePeriod: setForceSameplePeriod,
    onForceChange:onForceChange,
    offForceChange:offForceChange,
    getSPO2Id:getSPO2Id,
    getSPO2: getSPO2,
    openSPO2: openSPO2,
    closeSPO2: closeSPO2,
    setSPO2SameplePeriod: setSPO2SameplePeriod,
    onSPO2Change: onSPO2Change,
    offSPO2Change: offSPO2Change
}
