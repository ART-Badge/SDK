/* JS API For HardWare */

var dcmlib = require("dcm");
var emqlib = require("emq");

var data_pool;

var ID_VBAT_READ = 0x0601;

var ID_LCD_ON = 0x0621;
var ID_LCD_OFF = 0x0622;
var ID_LCD_SET_MODE = 0x0623;
var ID_LCD_SET_BRIGHTNESS = 0x0624;

var ID_TP_ON = 0x0631;
var ID_TP_OFF = 0x0632;

var ID_REBOOT = 0x0641;
var ID_SHUTDOWN = 0x0642;
var ID_VIBRATE = 0x0643;
var ID_SYSINFO = 0x0644;

var ID_BEEP_ON = 0x0651;
var ID_BEEP_OFF = 0x0652;
var ID_BEEP_SET_PERIOD_DUTY = 0x0653;

var emq_key_channel = "hws.0";
var emq_rtc_channel = "hws.1";
var emq_tp_channel = "hws.2";
var emq_lcd_channel = "hws.3";
var emq_pm_channel = "hws.4";
var emq_beep_channel = "hws.5";


var dcm_pool_name = "hws";

var realtime_dcm_name = "realtime";
var lcdpower_dcm_name = "lcdpower";
var lcdmode_dcm_name = "lcdmode";
var brightness_dcm_name = "brightness"
var tppower_dcm_name = "tppower"
var battery_dcm_name = "battery"
var charge_dcm_name = "charge"
var vibstart_dcm_name = "vibstart"
var vibstop_dcm_name = "vibstop"
var vibcount_dcm_name = "vibcount"
var vibrate_dcm_name = "vibrate"
var sysinfo_dcm_name = "sysinfo"
var beep_period_dcm_name = "beepperiod"
var beep_duty_dcm_name = "beepduty"

var key_change_arr = [];
var screen_func_arr = [];
var touch_func_arr = [];
var battery_func_arr = [];
var charge_func_arr = [];
var realtime_func_arr = [];

/* 开启监听：增加onChange事件 */
function emqHWOnChange(func_arr, onChange, emq_channel)
{
    var obj =
    {
        fun : null,
        ep : null
    };

    if (func_arr == null)
    {
        ////console.log("emqHWOnChange: func_arr is null!")
        return false;
    }

    if (onChange == null)
    {
        //console.log("emqHWOnChange: onChange is null!")
        return false;
    }

    if (emq_channel == null)
    {
        //console.log("emqHWOnChange: emq_channel is null!")
        return false;
    }

    func_arr.forEach(function(x, index, a)
    {
        if (onChange == func_arr[index].fun)
        {
            //console.log("emq onChange func exists!")
            return true;
        }
    })

    obj.fun = onChange
    obj.ep =  emqlib.createEP();
    obj.ep.onMessage(emq_channel, onChange);

    func_arr.push(obj);
    //console.log("add onChange OK!")

    return true;
}

/* 取消监听：取消onChange事件 */
function emqHWOffChange(func_arr, onChange)
{
    //console.log("emqHWOffChange")

    if (func_arr == null)
    {
        //console.log("emqHWOffChange: func_arr is null!")
        return false;
    }

    if (onChange == null)
    {
        //console.log("emqHWOffChange: onChange is null!")
        return false;
    }

    func_arr.forEach(function(x, index, a)
    {
        if (onChange == func_arr[index].fun)
        {
            func_arr.splice(index, 1);

            return true;
        }
    })

    return true;
}

/* 开启监听：DCM 数据 改变 */
function dcmHWOnChange(func_arr, onChange, dcm_name)
{
    var obj =
    {
        name : null,
        fun : null,
        pool : null
    };

    if (func_arr == null)
    {
        //console.log("dcmHWOnChange: func_arr is null!")
        return false;
    }

    if (onChange == null)
    {
        //console.log("dcmHWOnChange: onChange is null!")
        return false;
    }

    if (dcm_name == null)
    {
        //console.log("dcmHWOnChange: dcm_name is null!")
        return false;
    }

    func_arr.forEach(function(x, index, a)
    {
        if (onChange == func_arr[index].fun)
        {
            //console.log("dcm onChange func exists!")
            return true;
        }
    })

    obj.name = dcm_name;
    obj.fun = onChange;
    obj.pool = dcmlib.Open(dcm_pool_name);
    obj.pool.onChange(dcm_name, onChange);

    func_arr.push(obj);
    //console.log("dcm add onChange OK!")

    return true;
}

/* 取消监听：DCM 数据 改变 */
function dcmHWOffChange(func_arr, onChange)
{
    //console.log("dcmOffChange")

    if (func_arr == null)
    {
        //console.log("dcmHWOffChange: func_arr is null!")
        return false;
    }

    if (onChange == null)
    {
        //console.log("dcmHWOffChange: onChange is null!")
        return false;
    }

    func_arr.forEach(function(x, index, a)
    {
        if (onChange == func_arr[index].fun)
        {
            var dcm_name = func_arr[index].name;
            func_arr[index].pool.offChange(dcm_name);
            func_arr.splice(index, 1);
            return true;
        }
    })

    return true;
}

function getRealTime()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var data = data_pool.getItem(realtime_dcm_name);

    if (data == null || data == "undefine")
    {
        data_pool.setItem(realtime_dcm_name, "1601366586");
        data = data_pool.getItem(realtime_dcm_name);
    }

    return data;
}

function setRealTime(time)
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var data = data_pool.getItem(realtime_dcm_name);
    if (data == null || data == "undefine")
    {
        //console.log("getItem null or undefine")
        data_pool.setItem(realtime_dcm_name, time);
    }

    //console.log("setRealTime");

    return true;
}

/* 开启监听：realtime改变 */
function onRealTimeChange(onChange)
{
    return dcmHWOnChange(realtime_func_arr, onChange, realtime_dcm_name);
}

/* 取消监听：realtime改变 */
function offRealTimeChange(onChange)
{
    return dcmHWOffChange(realtime_func_arr, onChange);
}

/* 获取屏幕电源状态 */
function getScreenStatus()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var lcdpower = data_pool.getItem(lcdpower_dcm_name);

    //console.log("getScreenStatus");
    if (lcdpower != null)
    {
        return lcdpower;
    }

    return 0;
}

/* 获取屏幕模式 */
function getScreenMode()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var lcdmode = data_pool.getItem(lcdmode_dcm_name);
    if (lcdmode != null)
    {
        console.log("getScreenMode:" + lcdmode);
        return lcdmode;
    }

    return null;
}

/* 设置屏幕模式 */
function setScreenMode(mode)
{
    data_pool = dcmlib.Open(dcm_pool_name);
    data_pool.setItem(lcdmode_dcm_name, mode);

    emqlib.send(emq_lcd_channel, ID_LCD_SET_MODE);
    console.log("[hardware.js] setScreenMode");

    return 0;
}

/* 打开屏幕 */
function openScreen()
{
    //console.log("---------" + msgid.MSG_ID_LCD_ON)
    emqlib.send(emq_lcd_channel, ID_LCD_ON);

    //console.log("openScreen");

    return true;
}

/* 打开蜂鸣器 */
function openBeep()
{
    emqlib.send(emq_beep_channel, ID_BEEP_ON);

    //console.log("JS OnBeep be invoked");

    return true;
}

/* 关闭蜂鸣器 */
function closeBeep()
{
    emqlib.send(emq_beep_channel, ID_BEEP_OFF);

    //console.log("JS closeBeep be invoked");

    return true;
}



/* 设置蜂鸣器值 */
function setBeep(period, duty)
{
    data_pool = dcmlib.Open(dcm_pool_name);

    data_pool.setItem(beep_period_dcm_name, period);
    data_pool.setItem(beep_duty_dcm_name, duty);

    console.log("JS setBeep be invoked")
    emqlib.send(emq_beep_channel, ID_BEEP_SET_PERIOD_DUTY);
    return true;
}

/* 关闭屏幕 */
function closeScreen()
{
    emqlib.send(emq_lcd_channel, ID_LCD_OFF);

    //console.log("closeScreen");

    return true;
}

/* 开启监听：屏幕状态改变 */
function onScreenStatusChange(onChange)
{
    return dcmHWOnChange(screen_func_arr, onChange, lcdpower_dcm_name);
}

/* 取消监听：屏幕状态改变 */
function offScreenStatusChange(onChange)
{
    //console.log("offScreenPowerChange");
    return dcmHWOffChange(screen_func_arr, onChange);
}

/* 获取屏幕亮度 */
function getScreenBrightness()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var brightness = data_pool.getItem(brightness_dcm_name);
    if (brightness != null)
    {
        return brightness;
    }

    //console.log("getScreenBrightness");

    return 0;
}

/* 设置屏幕亮度 */
function setScreenBrightness(bright)
{
    data_pool = dcmlib.Open(dcm_pool_name);
    data_pool.setItem(brightness_dcm_name, bright);

    emqlib.send(emq_lcd_channel, ID_LCD_SET_BRIGHTNESS);
    console.log("setScreenBrightness:" + bright);

    return true;
}

/* 获取触摸屏电源状态 */
function getTouchPanelStatus()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var tppower = data_pool.getItem(tppower_dcm_name);
    if (tppower != null)
    {
        return tppower;
    }

    //console.log("getTouchPanelStatus");

    return 0;
}

/* 打开触摸屏电源 */
function openTouchPanel()
{
    emqlib.send(emq_tp_channel, ID_TP_ON);
    //console.log("openTouchPanel");

    return true;
}

/* 关闭触摸屏电源 */
function closeTouchPanel()
{
    emqlib.send(emq_tp_channel, ID_TP_OFF);
    //console.log("closeTouchPanel");

    return true;
}

/* 开启监听：触摸屏电源状态 */
function onTouchPanelStatusChange(onChange)
{
    return dcmHWOnChange(touch_func_arr, onChange, tppower_dcm_name);
}

/* 取消监听：触摸屏电源状态 */
function offTouchPanelStatusChange(onChange)
{
    //console.log("offTouchPanelPowerChange");
    return dcmHWOffChange(touch_func_arr, onChange);
}

/* 获取电量 */
function getBatteryLevel()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var level = data_pool.getItem(battery_dcm_name);
    if (level != null)
    {
        return level;
    }

    //console.log("getBatteryLevel");

    return 0;
}

/* 开启监听：电量变化 */
function onBatteryLevelChange(onChange)
{
    //console.log("onBatteryLevelChange")
    return dcmHWOnChange(battery_func_arr, onChange, battery_dcm_name);
}

/* 取消监听：电量变化 */
function offBatteryLevelChange(onChange)
{
    //console.log("offBatteryLevelChange");
    return dcmHWOffChange(battery_func_arr, onChange);
}

/* 获取充电状态 */
function getChargeStatus()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var status = data_pool.getItem(charge_dcm_name);
    if (status != null)
    {
        return status;
    }

    //console.log("getChargeStatus");

    return 0;
}

/* 开启监听：充电状态 */
function onChargeChange(onChange)
{
    //console.log("onChargeChange")
    return dcmHWOnChange(charge_func_arr, onChange, charge_dcm_name);
}

/* 取消监听：充电状态 */
function offChargeChange(onChange)
{
    //console.log("offChargeChange");
    return dcmHWOffChange(charge_func_arr, onChange);
}

/* 系统重启 */
function reboot()
{
    emqlib.send(emq_pm_channel, ID_REBOOT);
    //console.log("reboot")

    return true;
}

/* 系统关机 */
function powerOff()
{
    emqlib.send(emq_pm_channel, ID_SHUTDOWN);
    //console.log("powerOff")

    return true;
}

/* 振动 */
function vibrate(start, stop, count)
{
    data_pool = dcmlib.Open(dcm_pool_name);

    data_pool.setItem(vibstart_dcm_name, start);
    data_pool.setItem(vibstop_dcm_name, stop);
    data_pool.setItem(vibcount_dcm_name, count);

    //console.log("vibrate")
    emqlib.send(emq_pm_channel, ID_VIBRATE);
    return true;
}

/* 开启监听：按键事件 */
function onKeyChange(onChange)
{
    return emqHWOnChange(key_change_arr, onChange, emq_key_channel);
}

/* 取消监听：按键事件 */
function offKeyChange(onChange)
{
    return emqHWOffChange(key_change_arr, onChange);
}

/* 获取系统信息 */
function getInfo()
{
    data_pool = dcmlib.Open(dcm_pool_name);
    var info = data_pool.getItem(sysinfo_dcm_name);
    if (info != null)
    {
        return info;
    }

    return null;
}

module.exports =
{
    getRealTime: getRealTime,
    setRealTime: setRealTime,
    onRealTimeChange:onRealTimeChange,
    offRealTimeChange:offRealTimeChange,
    getScreenStatus: getScreenStatus,
    getScreenMode: getScreenMode,
    setScreenMode: setScreenMode,
    openScreen: openScreen,
    closeScreen: closeScreen,
    onScreenStatusChange: onScreenStatusChange,
    offScreenStatusChange: offScreenStatusChange,
    getScreenBrightness: getScreenBrightness,
    setScreenBrightness: setScreenBrightness,
    getTouchPanelStatus: getTouchPanelStatus,
    openTouchPanel: openTouchPanel,
    closeTouchPanel: closeTouchPanel,
    onTouchPanelStatusChange: onTouchPanelStatusChange,
    offTouchPanelStatusChange: offTouchPanelStatusChange,
    getBatteryLevel: getBatteryLevel,
    onBatteryLevelChange: onBatteryLevelChange,
    offBatteryLevelChange: offBatteryLevelChange,
    getChargeStatus: getChargeStatus,
    onChargeChange: onChargeChange,
    offChargeChange: offChargeChange,
    reboot: reboot,
    powerOff: powerOff,
    vibrate: vibrate,
    onKeyChange: onKeyChange,
    offKeyChange: offKeyChange,
    getInfo: getInfo,
    setBeep: setBeep,
    closeBeep: closeBeep,
    openBeep: openBeep,

}
