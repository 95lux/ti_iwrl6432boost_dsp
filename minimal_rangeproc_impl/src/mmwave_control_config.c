/**
 * @file mmwave_control_config.c
 * @brief Configuration and initialization for mmWave chirp profiles.
 *
 * This file contains functions for setting up default chirp and profile
 * configurations for the Texas Instruments mmWave radar system. It includes
 * parameter definitions, initialization routines, and structure setup for
 * mmWave control.
 *
 * 
 * @details
 * This file is a modified version of the original from the motion and presence 
 * detection demo project. It has been adjusted to fit the current application 
 * needs while maintaining compatibility with TI's mmWave SDK.
 *
 * @copyright Copyright (C) 2022-24 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/* Copy of file with the same name from the motion and presence detection demo project with adjustments */
/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#define DebugP_LOG_ENABLED 1
#define RDIF_LANE_RATE_UPDATE 1

/* Standard Include Files. */
#include <math.h>

#include <drivers/hw_include/cslr_adcbuf.h>
#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/xwrL64xx/cslr_soc_baseaddress.h>

/* mmWave SDK Include Files: */
#include <control/mmwave/mmwave.h>
#include <kernel/dpl/DebugP.h>
#include <utils/testlogger/logger.h>

#include "system.h"
#include "defines.h"
#include "common/sys_defs.h"
#include "mmwave_control_config.h"

/*! @brief  Sensor Perchirp LUT */
T_SensPerChirpLut* sensPerChirpLuTable = (T_SensPerChirpLut*)(0x21880000U);

/**
 *  @b Description
 *  @n
 *      Utility function which populates the profile configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrProfileCfg
 *      Pointer to the populated profile configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultProfileCfg (T_RL_API_SENS_CHIRP_PROF_COMN_CFG* ptrProfileCfg, T_RL_API_SENS_CHIRP_PROF_TIME_CFG* ptrProfileTimeCfg) {
    float rfBandwidth;
    float rampDownTime;
    float scale = 65536./(3*100*100);

    /* Populate the *default* profile configuration: */
    gSysContext.profileComCfg.c_DigOutputSampRate           = CLI_DIG_OUT_SAMPLING_RATE;
    gSysContext.profileComCfg.c_DigOutputBitsSel            = CLI_DIG_OUT_BITS_SEL;
    gSysContext.profileComCfg.c_DfeFirSel                   = CLI_DFE_FIR_SEL;
    gSysContext.profileComCfg.h_NumOfAdcSamples             = CLI_NUM_ADC_SAMPLES;
    gSysContext.profileComCfg.c_ChirpTxMimoPatSel           = CLI_MIMO_SEL;
    gSysContext.profileComCfg.c_MiscSettings                = CLI_C_MISC_SETTINGS;
    gSysContext.profileComCfg.c_HpfFastInitDuration         = CLI_HPF_FAST_INIT_DURATION;
    gSysContext.profileComCfg.h_ChirpRampEndTime            = CLI_CHIRP_RAMP_END_TIME;
    gSysContext.profileComCfg.c_ChirpRxHpfSel               = CLI_CHIRP_RX_HPF_SEL;

    /* Populate the *default* timing configuration: */
    gSysContext.profileTimeCfg.h_ChirpIdleTime              = CLI_CHIRP_IDLE_TIME;
    gSysContext.profileTimeCfg.h_ChirpAdcStartTime          = CLI_CHIRP_ADC_START_TIME;
    gSysContext.profileTimeCfg.xh_ChirpTxStartTime          = CLI_CHIRP_TX_START_TIME;
    gSysContext.profileTimeCfg.xh_ChirpRfFreqSlope          = CLI_CHIRP_SLOPE;
    /* Front End Firmware expects Start freq (MHz) as 1 LSB = (3 x APLL_FREQ / 2^16) * 2^6 resolution  */
    gSysContext.profileTimeCfg.w_ChirpRfFreqStart           = CLI_CHIRP_START_FREQ;
    gSysContext.profileTimeCfg.h_ChirpTxEnSel               = CLI_CHA_CFG_TX_BITMASK;
    gSysContext.profileTimeCfg.h_ChirpTxBpmEnSel            = 0x0U; // MIMO BPM enable (hardcoded to 0 in demo project);

    rfBandwidth = (gSysContext.profileComCfg.h_ChirpRampEndTime*0.1) * CLI_CHIRP_SLOPE; //In MHz/usec
    rampDownTime = MIN((gSysContext.profileTimeCfg.h_ChirpIdleTime*0.1-1.0), 6.0); //In usec
    gSysContext.profileComCfg.h_CrdNSlopeMag = (uint16_t) fabs((scale * rfBandwidth / rampDownTime + 0.5));

    gSysContext.profileTimeCfg.xh_ChirpRfFreqSlope  = CLI_CHIRP_FREQ_SLOPE;


    /* Initialize the profile configuration: */
    memset ((void*)ptrProfileCfg, 0, sizeof(T_RL_API_SENS_CHIRP_PROF_COMN_CFG));

    /* Populate the *default* profile configuration: */
    ptrProfileCfg->c_DigOutputSampRate = gSysContext.profileComCfg.c_DigOutputSampRate; //23; //8; //M_RL_SENS_DIG_OUT_SAMP_RATE_MAX_12P5M;
    ptrProfileCfg->c_DigOutputBitsSel = gSysContext.profileComCfg.c_DigOutputBitsSel; //0; //M_RL_SENS_DIG_OUT_12BITS_4LSB_ROUND;
    ptrProfileCfg->c_DfeFirSel = gSysContext.profileComCfg.c_DfeFirSel; //0; //M_RL_SENS_DFE_FIR_LONG_FILT;
    ptrProfileCfg->c_VcoMultiChipMode = 0; //M_RL_SENS_VCO_MULT_CHIP_SINGLE;
    ptrProfileCfg->h_NumOfAdcSamples = gSysContext.profileComCfg.h_NumOfAdcSamples; //128; //256U; /* 2.56us */
    ptrProfileCfg->c_ChirpTxMimoPatSel = gSysContext.profileComCfg.c_ChirpTxMimoPatSel; //4; //0; //M_RL_SENS_TX_MIMO_PATRN_DIS;

    ptrProfileCfg->c_MiscSettings = gSysContext.profileComCfg.c_MiscSettings; //0U; /* HPF FINIT, CRD ena, PA blank dis */
    ptrProfileCfg->c_HpfFastInitDuration = gSysContext.profileComCfg.c_HpfFastInitDuration; //15U; /* 1.5us */
    ptrProfileCfg->h_CrdNSlopeMag = gSysContext.profileComCfg.h_CrdNSlopeMag; //0; //0x800U; /* default slope */

    //ptrProfileCfg->h_ChirpRampEndTime = 200U; /* 4us high res */
    ptrProfileCfg->h_ChirpRampEndTime = gSysContext.profileComCfg.h_ChirpRampEndTime; //361; //600; //250U; /* 25us low res */
    ptrProfileCfg->c_ChirpRxHpfSel = gSysContext.profileComCfg.c_ChirpRxHpfSel; //1; //M_RL_SENS_RX_HPF_SEL_350KHZ;

    /*ptrProfileCfg->c_ChirpRxGainSel = (36U | \
                    (M_RL_SENS_RF_GAIN_TARG_1 << M_RL_SENS_RF_GAIN_OFFSET));
    ptrProfileCfg->c_ChirpTxBackOffSel[0] = 0U;
    ptrProfileCfg->c_ChirpTxBackOffSel[1] = 0U;*/

    /* Initialize the profile time configuration: */
    memset ((void*)ptrProfileTimeCfg, 0, sizeof(T_RL_API_SENS_CHIRP_PROF_TIME_CFG));

    //ptrProfileTimeCfg->h_ChirpIdleTime = 325U; /* 6.5us high res */
    ptrProfileTimeCfg->h_ChirpIdleTime = gSysContext.profileTimeCfg.h_ChirpIdleTime; //80; //400; //65U; /* 6.5us low res */
    ptrProfileTimeCfg->h_ChirpAdcStartTime = gSysContext.profileTimeCfg.h_ChirpAdcStartTime; //300; //30770;
    /*((UINT16)25U | \
            ((UINT16)5U << M_RL_SENS_CHIRP_ADC_SKIP_SAMP_OFFSET)); 0.5us Fract + 0.4us skip */
    ptrProfileTimeCfg->xh_ChirpTxStartTime = gSysContext.profileTimeCfg.xh_ChirpTxStartTime; //0; //-10; /* -0.2us */
    ptrProfileTimeCfg->xh_ChirpRfFreqSlope = gSysContext.profileTimeCfg.xh_ChirpRfFreqSlope; //419; //699; //3495; /* 100MHz/us , 77G - 2621 */
    //ptrProfileTimeCfg->w_ChirpRfFreqStart  = M_RL_SENS_CHIRP_RFFREQ_HR_57G; /* 57GHz / 76GHz High */
    ptrProfileTimeCfg->w_ChirpRfFreqStart  = gSysContext.profileTimeCfg.w_ChirpRfFreqStart; //51200; //50347; //M_RL_SENS_CHIRP_RFFREQ_LR_57G; /* 57GHz / 76GHz low */
    ptrProfileTimeCfg->h_ChirpTxEnSel = gSysContext.profileTimeCfg.h_ChirpTxEnSel;//0x3U; /* 2 TX enable in chirp */
    ptrProfileTimeCfg->h_ChirpTxBpmEnSel = gSysContext.profileTimeCfg.h_ChirpTxBpmEnSel; //0x3U; //0; //0x2U; /* TX1 BPM enable in chirp */
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the chirp configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrChirpCfg
 *      Pointer to the populated chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultChirpCfg (T_RL_API_SENS_PER_CHIRP_CFG* ptrChirpCfg, T_RL_API_SENS_PER_CHIRP_CTRL* ptrChirpCtrl) {
    /* Initialize the chirp configuration: */
    memset ((void*)ptrChirpCfg, 0, sizeof(T_RL_API_SENS_PER_CHIRP_CFG));

    /* Populate the chirp configuration: */
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_FREQ_START] = 4;
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_FREQ_SLOPE] = 4;
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_IDLE_TIME] = 4;
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_ADC_START_TIME] = 4;
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_TX_START_TIME] = 4;
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_TX_ENABLE] = 4;
    ptrChirpCfg->h_ParamArrayLen[M_RL_SENS_PER_CHIRP_BPM_ENABLE] = 4;

    /* repeat count is not applicable for acc chirps, so new chirp param is picked after 2*2 chirps */
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_FREQ_START] = 1;
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_FREQ_SLOPE] = 1;
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_IDLE_TIME] = 1;
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_ADC_START_TIME] = 1;
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_TX_START_TIME] = 1;
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_TX_ENABLE] = 1;
    ptrChirpCfg->h_ParamRptCount[M_RL_SENS_PER_CHIRP_BPM_ENABLE] = 1;

    /* Initialize the chirp control configuration: */
    memset ((void*)ptrChirpCtrl, 0, sizeof(T_RL_API_SENS_PER_CHIRP_CTRL));

    /* Sensor per chirp control api */
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_FREQ_START] = \
        ((UINT32)(&sensPerChirpLuTable->StartFreqLowRes[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_FREQ_SLOPE] = \
        ((UINT32)(&sensPerChirpLuTable->ChirpSlope[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_IDLE_TIME] = \
        ((UINT32)(&sensPerChirpLuTable->ChirpIdleTime[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_ADC_START_TIME] = \
        ((UINT32)(&sensPerChirpLuTable->ChirpAdcStartTime[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_TX_START_TIME] = \
        ((UINT32)(&sensPerChirpLuTable->ChirpTxStartTime[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_TX_ENABLE] = \
        ((UINT32)(&sensPerChirpLuTable->ChirpTxEn[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);
    ptrChirpCtrl->h_ParamArrayStartAdd[M_RL_SENS_PER_CHIRP_BPM_ENABLE] = \
        ((UINT32)(&sensPerChirpLuTable->ChirpBpmEn[0]) & M_RL_SENS_PER_CHIRP_LUT_ADD_MASK);

    ptrChirpCtrl->h_PerChirpParamCtrl = M_RL_SENS_PER_CHIRP_CTRL_MAX;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default open configuration.
 *
 *  @param[out]  ptrOpenCfg
 *      Pointer to the open configuration
 *
 *  @retval
 *      Not applicable
 */
 // Derived from hwa_fft1d example project - Deactivate Factory Calibration
void Mmwave_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg) {
    ptrOpenCfg->useRunTimeCalib = false;
    ptrOpenCfg->useCustomCalibration = false;
    ptrOpenCfg->runTxCLPCCalib = false;
    ptrOpenCfg->ptrfecTxclpcCalCmd = &gSysContext.fecTxclpcCalCmd;
    ptrOpenCfg->customCalibrationEnableMask = 0U;
    ptrOpenCfg->fecRDIFCtrlCmd.c_RdifEnable = M_RL_FECSS_RDIF_DIS;
    ptrOpenCfg->fecRDIFCtrlCmd.h_RdifSampleCount = CLI_NUM_ADC_SAMPLES; //profileComCfg.h_NumOfAdcSamples;
}

static void Mmwave_EnChannelSetOffset(
    CSL_app_hwa_adcbuf_ctrlRegs *ptrAdcBufCtrlRegs,
    uint8_t channel,
    uint16_t offset
) {
    switch(channel) {
        case 0U:
            /* Enable the channel */
            CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG1,
                    APP_HWA_ADCBUF_CTRL_ADCBUFCFG1_ADCBUFCFG1_RX0EN,
                    1);

            /* Setup the offset */
            CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG2,
                    APP_HWA_ADCBUF_CTRL_ADCBUFCFG2_ADCBUFCFG2_ADCBUFADDRX0,
                    (offset >> 4));
            break;
        case 1U:
            /* Enable the channel */
            CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG1,
                    APP_HWA_ADCBUF_CTRL_ADCBUFCFG1_ADCBUFCFG1_RX1EN,
                    1);

            /* Setup the offset */
            CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG2,
                    APP_HWA_ADCBUF_CTRL_ADCBUFCFG2_ADCBUFCFG2_ADCBUFADDRX1,
                    (offset >> 4));
            break;
        case 2U:
            /* Enable the channel */
            CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG1,
                    APP_HWA_ADCBUF_CTRL_ADCBUFCFG1_ADCBUFCFG1_RX2EN,
                    1);

            /* Setup the offset */
            CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG3,
                    APP_HWA_ADCBUF_CTRL_ADCBUFCFG3_ADCBUFCFG3_ADCBUFADDRX2,
                    (offset >> 4));
            break;

        default:
            /* Not  supported channels, code should not end up here */
            DebugP_assert(0);
            break;
    }
}

/**
  * @brief  Configures the ADC buffer for enabled Rx channels
  *
  * @param  rxChannelEn   Enabled Rx channel bitmask
  *
  * @param  chanDataSize  Size of data per channel
  *
  * @return None
*/
static void Mmwave_ADCBufConfig
(
    uint16_t rxChannelEn,
    uint32_t chanDataSize
){
    CSL_app_hwa_adcbuf_ctrlRegs *ptrAdcBufCtrlRegs = (CSL_app_hwa_adcbuf_ctrlRegs *)CSL_APP_HWA_ADCBUF_CTRL_U_BASE;
    uint8_t channel = 0;
    uint16_t offset = 0;

    /* Disable all ADCBuf channels */
    CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG1, APP_HWA_ADCBUF_CTRL_ADCBUFCFG1_ADCBUFCFG1_RX0EN, 0);
    CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG1, APP_HWA_ADCBUF_CTRL_ADCBUFCFG1_ADCBUFCFG1_RX1EN, 0);
    CSL_FINS(ptrAdcBufCtrlRegs->ADCBUFCFG1, APP_HWA_ADCBUF_CTRL_ADCBUFCFG1_ADCBUFCFG1_RX2EN, 0);

    /* Enable Rx Channels */
    for (channel = 0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++) {
        if (rxChannelEn & (0x1U << channel)) {
            /* Enable Channel and configure offset. */
            Mmwave_EnChannelSetOffset(ptrAdcBufCtrlRegs, channel, offset);

            /* Calculate offset for the next channel */
            offset  += chanDataSize;
        }
    }

    return;
}

/**
  *  @brief  Populates the channel configuration structure
  *  
  *  @return None
*/
void MMWave_populateChannelCfg() {
    gSysContext.channelCfg.h_TxChCtrlBitMask  = CLI_CHA_CFG_TX_BITMASK;
    gSysContext.channelCfg.h_RxChCtrlBitMask  = CLI_CHA_CFG_RX_BITMASK;
    gSysContext.channelCfg.c_MiscCtrl         = CLI_CHA_CFG_MISC_CTRL;

    // calculate number RX and TX antennas from bitmask
    gSysContext.numTxAntennas = 0;
    gSysContext.numRxAntennas = 0;
    for (uint32_t i = 0; i < SYS_COMMON_NUM_TX_ANTENNAS; i++) {
        if ((gSysContext.channelCfg.h_TxChCtrlBitMask >> i) & 0x1) {
            gSysContext.numTxAntennas++;
        }
    }

    for (uint32_t i = 0; i < SYS_COMMON_NUM_RX_CHANNEL; i++) {
        if ((gSysContext.channelCfg.h_RxChCtrlBitMask >> i) & 0x1) {
            gSysContext.numRxAntennas++;
        }
    }


    // omitted "calculation" of rxAntOrder, since doppler is not used in this project
}


/**
 *  @b Description
 *  @n
 *      The function is used to populate the default control configuration
 *      in chirp configuration mode
 *
 *  @param[out]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg) {
    T_RL_API_SENS_CHIRP_PROF_COMN_CFG       profileCfg;
    T_RL_API_SENS_CHIRP_PROF_TIME_CFG       profileTimeCfg;
    T_RL_API_SENS_PER_CHIRP_CFG             chirpCfg;
    T_RL_API_SENS_PER_CHIRP_CTRL            chirpCtrl;
    int32_t             errCode;
    MMWave_ChirpHandle  chirpHandle;

    Mmwave_ADCBufConfig(gSysContext.channelCfg.h_RxChCtrlBitMask, (gSysContext.profileComCfg.h_NumOfAdcSamples *2));

    /* Initialize the control configuration: */
    memset ((void*)ptrCtrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* Populate the frame configuration: */
    gSysContext.frameCfg.h_NumOfChirpsInBurst      = CLI_NUM_CHIRPS_PER_BURST;
    gSysContext.frameCfg.c_NumOfChirpsAccum        = CLI_NUM_CHIRPS_ACCUM;
    gSysContext.frameCfg.w_BurstPeriodicity        = CLI_W_BURST_PERIOD; 
    gSysContext.frameCfg.h_NumOfBurstsInFrame      = CLI_NUM_BURSTS_PER_FRAME;
    gSysContext.frameCfg.w_FramePeriodicity        = CLI_FRAME_PERIOD;
    gSysContext.frameCfg.h_NumOfFrames             = CLI_NUM_FRAMES;

    /* Populate the profile configuration: */
    Mmwave_populateDefaultProfileCfg (&profileCfg, &profileTimeCfg);

    /* Create the profile: */
    ptrCtrlCfg->frameCfg[0].profileHandle[0] = MMWave_addProfile(gSysContext.gCtrlHandle, &profileCfg, &profileTimeCfg, &errCode);
    if (ptrCtrlCfg->frameCfg[0].profileHandle[0] == NULL) {
        DebugP_logError ("Error: Unable to add the profile [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Profile Error");
        return;
    }
    DebugP_log ("MMWave Add Profile Success");

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultChirpCfg (&chirpCfg, &chirpCtrl);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->frameCfg[0].profileHandle[0], &chirpCfg, &chirpCtrl, &errCode);
    if (chirpHandle == NULL) {
        DebugP_logError ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Chirp Error");
        return;
    }
    DebugP_log ("MMWave Add Chirp Success");

    /* Populate the frame configuration: */
    ptrCtrlCfg->frameCfg[0].frameCfg.h_NumOfChirpsInBurst = gSysContext.frameCfg.h_NumOfChirpsInBurst; //2; //10; //2U;
    ptrCtrlCfg->frameCfg[0].frameCfg.c_NumOfChirpsAccum = gSysContext.frameCfg.c_NumOfChirpsAccum; //0U;
    //ptrCtrlCfg->frameCfg[0].frameCfg.w_BurstPeriodicity = 2500U; /* 8 chirps = 40us + 10us idle */
    ptrCtrlCfg->frameCfg[0].frameCfg.w_BurstPeriodicity = gSysContext.frameCfg.w_BurstPeriodicity; //1698; //12000; //3480U; /* 4 chirps = 148us + 200us idle , (12 + 25) chirp*/
    ptrCtrlCfg->frameCfg[0].frameCfg.h_NumOfBurstsInFrame = gSysContext.frameCfg.h_NumOfBurstsInFrame; //64; //2; //32U;
    ptrCtrlCfg->frameCfg[0].frameCfg.w_FramePeriodicity = gSysContext.frameCfg.w_FramePeriodicity; //10000000; //120000; //29440U; /* 2 bursts = 696us + 40us idle, 40M XTAL */
    ptrCtrlCfg->frameCfg[0].frameCfg.h_NumOfFrames = gSysContext.frameCfg.h_NumOfFrames; //0; //10; //50U;
    ptrCtrlCfg->frameCfg[0].frameCfg.w_FrameEvent0TimeCfg = 0;

    /*\
        (ptrCtrlCfg->frameCfg[0].frameCfg.w_FramePeriodicity - 240U); frame period - 6us */
    ptrCtrlCfg->frameCfg[0].frameCfg.w_FrameEvent1TimeCfg = 0;

    /*\
        (ptrCtrlCfg->frameCfg[0].frameCfg.w_FramePeriodicity - 120U); frame period - 3us */

    ptrCtrlCfg->frameCfg[0].tempCfg.h_TempCtrlBitMask = 0x311;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default calibration
 *      configuration which is passed to start the mmWave module
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg) {
    /* Populate the calibration configuration: */
    ptrCalibrationCfg->chirpCalibrationCfg.enableCalibration    = false;
    ptrCalibrationCfg->chirpCalibrationCfg.enablePeriodicity    = false;
    ptrCalibrationCfg->chirpCalibrationCfg.periodicTimeInFrames = 10U;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default start
 *      configuration which is passed to start the mmWave module
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultStartCfg (MMWave_StrtCfg* ptrStartCfg) {
    /* Populate the start configuration: */
    ptrStartCfg->frameTrigMode      = CLI_SENSOR_START_FRM_TRIG;
    ptrStartCfg->chirpStartSigLbEn  = CLI_SENSOR_START_LB_EN;
    ptrStartCfg->frameLivMonEn      = CLI_SENSOR_START_MON_EN;
    ptrStartCfg->frameTrigTimerVal  = CLI_SENSOR_START_TRIG_TIMER;

    return;
}
