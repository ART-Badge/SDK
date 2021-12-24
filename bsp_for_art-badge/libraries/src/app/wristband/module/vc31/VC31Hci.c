
#include "VC31Hci.h"
#include "string.h"
#include "trace.h"

bool isJustStarted = false;

static uint32_t intDiffTimeVec[VC3120kLength];
static uint8_t sarCalCnt;
static uint8_t oscDfSetting;
static uint16_t f20kOsc;
static uint32_t VC31SampleCnt = 0;;
static bool oscIsCaled = false;
static bool oscCalFinish = false;
static uint32_t mcuOscValue = 0;
static uint8_t VC31UnWearToIsWearFlag = 0;
static uint32_t oscCheckCnt = 0;
static uint8_t intDiffTimeSize = 4;
static uint8_t VC31TmpD2[11];
//static uint16_t VC31SampleRate = 10;

vc31_read_cb user_vc31_read_cb __attribute__((weak)) = NULL;
vc31_write_cb user_vc31_write_cb __attribute__((weak)) = NULL;
vc31_gettime_cb user_vc31_gettime_cb __attribute__((weak)) = NULL;

static VC31Ret_t VC31Adjust(VC31_t *pVC31)
{
    VC31Ret_t ret = VC31Ret_isOK;

    uint16_t adjustParam = 0;
    uint32_t adjustStep  = 0;

    pVC31->sampleData.currentValue += 10;

    if (pVC31->sampleData.ppgValue > VC31_PPG_LIMIT_H)
    {
        if (pVC31->sampleData.currentValue < VC31_CURRENT_LIMIT_H)
        {
            if (pVC31->adjustInfo.directionLast == AdjustDirection_Down)
            {
                pVC31->adjustInfo.step *= VC31_ADJUST_FACTOR_DECREASE;
            }
            else if ((pVC31->adjustInfo.directionLast == AdjustDirection_Up) &&
                     (pVC31->adjustInfo.directionLastBefore == AdjustDirection_Up))
            {
                pVC31->adjustInfo.step *= VC31_ADJUST_FACTOR_INCREASE;
            }
            else
            {
                pVC31->adjustInfo.step *= 16;
            }
            pVC31->adjustInfo.step = pVC31->adjustInfo.step >> 4;
            pVC31->adjustInfo.step = (pVC31->adjustInfo.step > VC31_ADJUST_FACTOR_MAX) ?
                                     VC31_ADJUST_FACTOR_MAX : pVC31->adjustInfo.step;
            pVC31->adjustInfo.step = (pVC31->adjustInfo.step < VC31_ADJUST_FACTOR_MIN) ?
                                     VC31_ADJUST_FACTOR_MIN : pVC31->adjustInfo.step;
            adjustStep  = (pVC31->adjustInfo.step / (3072 - pVC31->sampleData.currentValue));
            adjustStep = (adjustStep <= VC31_ADJUST_STEP_MIN) ? VC31_ADJUST_STEP_MIN : adjustStep;
            adjustStep = (adjustStep >= VC31_ADJUST_STEP_MAX) ? VC31_ADJUST_STEP_MAX : adjustStep;
            adjustParam = (uint16_t)(adjustStep) | VC31_GREEN_ADJ_ENABLE | VC31_GREEN_ADJ_UP;
            ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *)&adjustParam, VC31_GREEN_ADJ_SIZE);
            pVC31->sampleData.ppgValue |= 0x1000; // Adjust Flag.
        }
        pVC31->adjustInfo.directionLastBefore = pVC31->adjustInfo.directionLast;
        pVC31->adjustInfo.directionLast = AdjustDirection_Up;
    }
    else if (pVC31->sampleData.ppgValue < VC31_PPG_LIMIT_L)
    {
        //if (pVC31->sampleData.currentValue > VC31_CURRENT_LIMIT_L)
        {
            if (pVC31->adjustInfo.directionLast == AdjustDirection_Up)
            {
                pVC31->adjustInfo.step *= VC31_ADJUST_FACTOR_DECREASE;
            }
            else if ((pVC31->adjustInfo.directionLast == AdjustDirection_Down) &&
                     (pVC31->adjustInfo.directionLastBefore == AdjustDirection_Down))
            {
                pVC31->adjustInfo.step *= VC31_ADJUST_FACTOR_INCREASE;
            }
            else
            {
                pVC31->adjustInfo.step *= 16;
            }
            pVC31->adjustInfo.step = pVC31->adjustInfo.step >> 4;
            pVC31->adjustInfo.step = (pVC31->adjustInfo.step > VC31_ADJUST_FACTOR_MAX) ?
                                     VC31_ADJUST_FACTOR_MAX : pVC31->adjustInfo.step;
            pVC31->adjustInfo.step = (pVC31->adjustInfo.step < VC31_ADJUST_FACTOR_MIN) ?
                                     VC31_ADJUST_FACTOR_MIN : pVC31->adjustInfo.step;
            adjustStep  = (pVC31->adjustInfo.step / (1024 + pVC31->sampleData.currentValue));
            adjustStep = (adjustStep <= VC31_ADJUST_STEP_MIN) ? VC31_ADJUST_STEP_MIN : adjustStep;
            adjustStep = (adjustStep >= VC31_ADJUST_STEP_MAX) ? VC31_ADJUST_STEP_MAX : adjustStep;
            adjustParam = (uint16_t)(adjustStep) | VC31_GREEN_ADJ_ENABLE | VC31_GREEN_ADJ_DOWN;
            ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *)&adjustParam, VC31_GREEN_ADJ_SIZE);
            pVC31->sampleData.ppgValue |= 0x1000;
        }
        pVC31->adjustInfo.directionLastBefore = pVC31->adjustInfo.directionLast;
        pVC31->adjustInfo.directionLast = AdjustDirection_Down;
    }
    else
    {
        pVC31->adjustInfo.directionLastBefore = pVC31->adjustInfo.directionLast;
        pVC31->adjustInfo.directionLast = AdjustDirection_Null;
    }

    return ret;
}


static VC31Ret_t VC31WearStatusDetection(VC31_t *pVC31)
{
    VC31Ret_t ret = VC31Ret_isOK;

    switch (pVC31->wearStatus)
    {
    case wearST_isWear:
        if ((pVC31->sampleData.envValue >= VC31_ENV_LIMIT) ||
            (pVC31->sampleData.psValue < pVC31->sampleData.envValue + VC31_PS_LIMIT))
        {
            if (--pVC31->unWearCnt <= 0) // 3 times.
            {
                pVC31->wearStatus   = wearST_unWear;
                pVC31->unWearCnt        = VC31_UNWEAR_CNT;
                pVC31->isWearCnt    = VC31_ISWEAR_CNT;
                pVC31->ctrl             &= ~VC31_CTRL_ENABLE_PPG;
                ret |= VC31WriteRegisters(VC31_CTRL, &(pVC31->ctrl), VC31_CTRL_SIZE);
                ret |= VC31Ret_isWearToUnWear;
            }
        }
        else
        {
            pVC31->unWearCnt = VC31_UNWEAR_CNT;
        }
        break;
    case wearST_unWear:

        if (pVC31->sampleData.psValue >= pVC31->sampleData.envValue + VC31_PS_LIMIT)
        {
            if (--pVC31->isWearCnt <= 0)
            {
                pVC31->wearStatus   = wearST_isWear;
                pVC31->unWearCnt        = VC31_UNWEAR_CNT;
                pVC31->isWearCnt        = VC31_ISWEAR_CNT;
                pVC31->ctrl             |= VC31_CTRL_ENABLE_PPG;
                ret |= VC31WriteRegisters(VC31_CTRL, &(pVC31->ctrl), VC31_CTRL_SIZE);
                ret |= VC31Ret_unWearToIsWear;
                VC31UnWearToIsWearFlag = 1;
                pVC31->adjustInfo.directionLastBefore = AdjustDirection_Null;
                pVC31->adjustInfo.directionLast = AdjustDirection_Null;
            }
        }
        else
        {
            pVC31->isWearCnt = VC31_ISWEAR_CNT;
        }
        break;
    }

    return ret;
}

VC31Ret_t VC31Init(VC31_t *pVC31, VC31Mode_t workMode)
{
    VC31Ret_t ret = VC31Ret_isOK;

    pVC31->workMode  = workMode;
    pVC31->isRunning = false;

    return ret;
}

VC31Ret_t VC31StartSample(VC31_t *pVC31)
{
    uint8_t greenWait;
    uint8_t tiaWait;
    uint16_t ppgDiv;
    uint8_t psDiv;
    uint8_t psWait;
    uint8_t ampWait;
    uint8_t greenIrWait;

    VC31Ret_t ret = VC31Ret_isOK;

    if (pVC31->isRunning == true)
    {
        return ret;
    }
    pVC31->isRunning = true;

    VC31SampleCnt = 0;;
    oscIsCaled = false;
    oscCalFinish = false;
    mcuOscValue = 0;
    VC31UnWearToIsWearFlag = 0;
    oscCheckCnt = 0;
    intDiffTimeSize = 4;

    greenWait = 0xB4;
    ret |= VC31WriteRegisters(VC31_GREEN_WAIT, &greenWait, VC31_GREEN_WAIT_SIZE);
    tiaWait = 0x3C;
    ret |= VC31WriteRegisters(VC31_TIA_WAIT, &tiaWait, VC31_TIA_WAIT_SIZE);

    psDiv = 0x09;
    ret |= VC31WriteRegisters(VC31_PS_DIV, &psDiv, VC31_PS_DIV_SIZE);
    psWait = 0xA0;
    ret |= VC31WriteRegisters(VC31_IR_WAIT, &psWait, VC31_IR_WAIT_SIZE);
    ppgDiv = 0x0A35;
    ret |= VC31WriteRegisters(VC31_PPG_DIV, (uint8_t *)&ppgDiv, VC31_PPG_DIV_SIZE);

    greenIrWait = 0x20;
    ret |= VC31WriteRegisters(VC31_GREEN_IR_GAP, &greenIrWait, VC31_GREEN_IR_GAP_SIZE);
    ampWait = 0x14;
    ret |= VC31WriteRegisters(VC31_AMP_WAIT, &ampWait, VC31_AMP_WAIT_SIZE);

    pVC31->wearStatus = wearST_isWear;
    pVC31->ctrl = VC31_CTRL_OPA_GAIN_25 | VC31_CTRL_ENABLE_PPG | VC31_CTRL_ENABLE_PRE |
                  VC31_CTRL_WORK_MODE | VC31_CTRL_INT_DIR_RAISING;
    ret |= VC31WriteRegisters(VC31_CTRL, &(pVC31->ctrl), VC31_CTRL_SIZE);
    uint8_t readBuf[1] = {0};
    VC31ReadRegisters(VC31_CTRL, readBuf, VC31_CTRL_SIZE);
    APP_PRINT_INFO2("[VC31] CTRL = %d, LINE = %d", readBuf[0], __LINE__);

    switch (pVC31->workMode)
    {
    case VC31Mode_NormalWork:
        // VC31 adjust
        pVC31->adjustInfo.step  = 307200;
        pVC31->adjustInfo.directionLastBefore = AdjustDirection_Null;
        pVC31->adjustInfo.directionLast = AdjustDirection_Null;

        pVC31->unWearCnt                = VC31_UNWEAR_CNT;
        pVC31->isWearCnt                = VC31_ISWEAR_CNT;

        break;

    case VC31Mode_CrossTalkTest:

        break;
    }

    isJustStarted = true;

    pVC31->isRunning = false;

    return ret;
}

VC31Ret_t VC31StopSample(VC31_t *pVC31)
{
    VC31Ret_t ret = VC31Ret_isOK;

    if (pVC31->isRunning == true)
    {
        return ret;
    }
    pVC31->isRunning = true;

    pVC31->wearStatus = wearST_unWear;
    pVC31->ctrl &= ~VC31_CTRL_ENABLE_PPG;
    pVC31->ctrl &= ~VC31_CTRL_WORK_MODE;
    uint8_t bugBuf[2] = {0, 0};
    ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *)bugBuf, VC31_GREEN_ADJ_SIZE);
    //ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *)(0x0000), VC31_GREEN_ADJ_SIZE);
    ret |= VC31WriteRegisters(VC31_CTRL, &(pVC31->ctrl), VC31_CTRL_SIZE);

    isJustStarted = false;

    pVC31->isRunning = false;

    return ret;
}

VC31Ret_t VC31GetSampleValues(VC31_t *pVC31)
{
    VC31Ret_t ret                   = VC31Ret_isOK;

    if (pVC31->isRunning == true)
    {
        return ret;
    }
    pVC31->isRunning = true;

    uint8_t status;
    uint16_t adjustParam;
    int currentTest;

    switch (pVC31->workMode)
    {
    case VC31Mode_NormalWork:
        ret |= VC31ReadRegisters(VC31_STATUS, (uint8_t *) & (VC31TmpD2), 11);
        status = VC31TmpD2[0];
        pVC31->sampleData.ppgValue = ((VC31TmpD2[2] << 8) | VC31TmpD2[1]);
        pVC31->sampleData.currentValue = ((VC31TmpD2[4] << 8) | VC31TmpD2[3]) + 10;
        pVC31->sampleData.preValue = ((VC31TmpD2[6] << 8) | VC31TmpD2[5]);
        pVC31->sampleData.psValue = ((VC31TmpD2[8] << 8) | VC31TmpD2[7]);
        pVC31->sampleData.envValue = ((VC31TmpD2[10] << 8) | VC31TmpD2[9]);
        if (status & VC31_STATUS_D_PPG_OK)
        {
            ret |= VC31Adjust(pVC31);
        }

#if (VC31_WEAR_STATUS_DETECTION_MODE != VC31_WEAR_STATUS_DETECTION_OFF)
        if (status & VC31_STATUS_D_PS_OK)
        {
            ret |= VC31WearStatusDetection(pVC31);
        }
#endif

        break;

    case VC31Mode_CrossTalkTest:
        adjustParam = 0;
        ret |= VC31ReadRegisters(VC31_PRE, (uint8_t *) & (pVC31->sampleData.preValue), VC31_PRE_SIZE);
        ret |= VC31ReadRegisters(VC31_CURRENT, (uint8_t *) & (pVC31->sampleData.currentValue),
                                 VC31_CURRENT_SIZE);
        ret |= VC31ReadRegisters(VC31_PS, (uint8_t *) & (pVC31->sampleData.psValue), VC31_PS_SIZE);
        ret |= VC31ReadRegisters(VC31_ENV, (uint8_t *) & (pVC31->sampleData.envValue), VC31_ENV_SIZE);

        currentTest = 1000;//250;//1562;//620;
        if (pVC31->sampleData.currentValue <= currentTest - 200)
        {
            adjustParam = 1000 | VC31_GREEN_ADJ_ENABLE | VC31_GREEN_ADJ_UP;
            ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *) &adjustParam, VC31_GREEN_ADJ_SIZE);
        }
        else if (pVC31->sampleData.currentValue >= currentTest + 200)
        {
            adjustParam = 1000 | VC31_GREEN_ADJ_ENABLE | VC31_GREEN_ADJ_DOWN;
            ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *) &adjustParam, VC31_GREEN_ADJ_SIZE);
        }
        else if (pVC31->sampleData.currentValue <= currentTest - 20)
        {
            adjustParam = 400 | VC31_GREEN_ADJ_ENABLE | VC31_GREEN_ADJ_UP;
            ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *) &adjustParam, VC31_GREEN_ADJ_SIZE);
        }
        else if (pVC31->sampleData.currentValue >= currentTest + 20)
        {
            adjustParam = 400 | VC31_GREEN_ADJ_ENABLE | VC31_GREEN_ADJ_DOWN;
            ret |= VC31WriteRegisters(VC31_GREEN_ADJ, (uint8_t *) &adjustParam, VC31_GREEN_ADJ_SIZE);
        }

        break;
    }

    if (isJustStarted)
    {
        isJustStarted = false;
        ret = VC31Ret_unWearToIsWear;
    }

    pVC31->isRunning = false;

    return ret;
}


void VC31OscCalibrationInit(uint32_t mcuOsc)
{
    uint8_t i;
    for (i = 0; i < intDiffTimeSize; i++)
    {
        intDiffTimeVec[i] = 0;
    }
    f20kOsc = 0;
    sarCalCnt = 0;
    oscDfSetting = 0x20;
    oscCalFinish = false;
    mcuOscValue = mcuOsc;
}


uint16_t VC3120kOscFreqCalculation(void)
{
    uint8_t i;
    bool diffIsOK;
    uint32_t totIntDiff = 0;
    uint16_t fOsc;
    uint16_t ppgDivReg;
    for (i = intDiffTimeSize; i > 1; i--)
    {
        intDiffTimeVec[i - 1] = intDiffTimeVec[i - 2];
    }
    intDiffTimeVec[0] = VC31GetTimeIntervalFromMCU();
    VC31ReadRegisters(VC31_PPG_DIV, (uint8_t *)&ppgDivReg, VC31_PPG_DIV_SIZE);
    ppgDivReg = (ppgDivReg > 4087) ? 4095 : (ppgDivReg + 8);
    diffIsOK = ((intDiffTimeVec[intDiffTimeSize - 1] >= mcuOscValue * ppgDivReg / VC3120kFmax - 500) &&
                (intDiffTimeVec[intDiffTimeSize - 1] <= mcuOscValue * ppgDivReg / VC3120kFmin + 500));
    diffIsOK = ((intDiffTimeVec[0] - intDiffTimeVec[intDiffTimeSize - 1] <= VC3120kVecDiff) ||
                (intDiffTimeVec[intDiffTimeSize - 1] - intDiffTimeVec[0] <= VC3120kVecDiff)) && diffIsOK;
    for (i = 0; i < intDiffTimeSize - 1; i++)
    {
        diffIsOK = ((intDiffTimeVec[i] - intDiffTimeVec[i + 1] <= VC3120kVecDiff) ||
                    (intDiffTimeVec[i + 1] - intDiffTimeVec[i] <= VC3120kVecDiff)) && diffIsOK;
    }
    if (diffIsOK)
    {
        for (i = 0; i < intDiffTimeSize; i++)
        {
            totIntDiff += intDiffTimeVec[i];
        }
        fOsc = (uint32_t)ppgDivReg * mcuOscValue / (totIntDiff / intDiffTimeSize);
    }
    else
    {
        fOsc = 0;
    }
    return fOsc;
}
bool VC312MOscCalibration(uint16_t fOsc)
{
    uint16_t oscNrtReg = 4 * 2000000 / fOsc;
    uint16_t oscCntTmp;
    if (sarCalCnt == 0)
    {
        uint8_t oscDfRegSetting = 0x80 | oscDfSetting;
        VC31WriteRegisters(0x2B, (uint8_t *)&oscDfRegSetting, 1);
    }
    else if (sarCalCnt < 6)
    {
        VC31ReadRegisters(0x0C, (uint8_t *)&oscCntTmp, 2);
        if (oscCntTmp > oscNrtReg)
        {
            oscDfSetting &= ~(0x20 >> (sarCalCnt - 1));
        }
        oscDfSetting |= (0x20 >> sarCalCnt);
        oscDfSetting = (oscDfSetting == 0) ? 1 : oscDfSetting;
        oscDfSetting = (oscDfSetting >= 63) ? 63 : oscDfSetting;
        uint8_t oscDfRegSetting = 0x80 | oscDfSetting;
        VC31WriteRegisters(0x2B, (uint8_t *)&oscDfRegSetting, 1);
    }
    sarCalCnt++;
    return (sarCalCnt >= 6);
}
bool VC31OscCalibration(uint16_t sampleRate)
{
    bool oscCalIsDone = false;
    uint16_t ppgDivReg;

    if (f20kOsc == 0)
    {
        f20kOsc = VC3120kOscFreqCalculation();
    }
    if (f20kOsc != 0)
    {
        oscCalIsDone = VC312MOscCalibration(f20kOsc);
    }
    if (oscCalIsDone)
    {
        ppgDivReg = (f20kOsc) / sampleRate;
        ppgDivReg = (ppgDivReg > 8) ? (ppgDivReg - 8) : 0;
        VC31WriteRegisters(VC31_PPG_DIV, (uint8_t *)&ppgDivReg, VC31_PPG_DIV_SIZE);
    }
    return oscCalIsDone;
}

bool VC31OscVerify(uint16_t sampleRate)
{
    bool oscCalIsValid;
    uint32_t intDiff;
    intDiff = VC31GetTimeIntervalFromMCU();
    oscCalIsValid = (intDiff >= (mcuOscValue / sampleRate - 100)) &&
                    (intDiff <= (mcuOscValue / sampleRate + 100));
    return oscCalIsValid;
}

static void VC31OscCheckRegularTime(uint16_t sampleRate)
{
    uint16_t ppgDivReg;
    uint8_t i;
    int32_t secondCnt = sampleRate * 300; //5min
    if (!oscCheckCnt)
    {
        intDiffTimeSize = 10;
        f20kOsc = 0;
        for (i = 0; i < intDiffTimeSize; i++)
        {
            intDiffTimeVec[i] = 0;
        }
    }
    oscCheckCnt++;
    if (oscCheckCnt >= secondCnt)
    {
        if (oscCheckCnt == secondCnt)
        {
            VC31UnWearToIsWearFlag = 0;
        }
        if ((oscCheckCnt >= secondCnt + sampleRate * 120) || (VC31UnWearToIsWearFlag))
        {
            for (i = 0; i < intDiffTimeSize; i++)
            {
                intDiffTimeVec[i] = 0;
            }
            oscCheckCnt = 0;
            VC31UnWearToIsWearFlag = 0;
        }
        if (f20kOsc == 0)
        {
            f20kOsc = VC3120kOscFreqCalculation();
        }
        else
        {
            ppgDivReg = (f20kOsc) / sampleRate;
            ppgDivReg = (ppgDivReg > 8) ? (ppgDivReg - 8) : 0;
            VC31WriteRegisters(VC31_PPG_DIV, (uint8_t *)&ppgDivReg, VC31_PPG_DIV_SIZE);
            oscCheckCnt = 0;
        }
    }
}
bool VC31OscCheck(uint32_t mcuOsc, VC31_t *pVC31, uint8_t VC31SampleRate)
{
    if (!oscCalFinish)
    {
        if (VC31SampleCnt <= 10)
        {
            VC31OscCalibrationInit(mcuOsc);
            oscIsCaled = false;
        }
        if ((VC31SampleCnt > 10) && (!oscIsCaled))
        {
            oscIsCaled = VC31OscCalibration(VC31SampleRate);
        }
        if (oscIsCaled)
        {
            oscCalFinish = VC31OscVerify(VC31SampleRate);
        }
        if (!oscCalFinish)
        {
            oscIsCaled = false;
        }
        VC31SampleCnt = (VC31SampleCnt >= 100) ? VC31SampleCnt : VC31SampleCnt + 1;
    }
    else
    {
        VC31OscCheckRegularTime(VC31SampleRate);
    }
    return oscCalFinish;
}

VC31Ret_t VC31WriteRegisters(uint8_t startAddress, uint8_t *pRegisters, uint8_t len)
{
    if (user_vc31_write_cb != NULL)
    {
        return user_vc31_write_cb(startAddress, pRegisters, len);
    }
    return VC31Ret_isERR;
}

VC31Ret_t VC31ReadRegisters(uint8_t startAddress, uint8_t *pRegisters, uint8_t len)
{
    if (user_vc31_read_cb != NULL)
    {
        return user_vc31_read_cb(startAddress, pRegisters, len);
    }
    return VC31Ret_isERR;
}

uint32_t VC31GetTimeIntervalFromMCU(void)
{
    if (user_vc31_gettime_cb != NULL)
    {
        return user_vc31_gettime_cb();
    }
    return 0;
}

