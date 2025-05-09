/**
 * @file factory_cal.c
 * @brief Factory Calibration Restoration and Configuration.
 *
 * This file implements the functionality to restore factory calibration data
 * for the radar system. It reads calibration data from flash memory, validates
 * its integrity using a magic number, and configures the radar front-end
 * (FECSS) with the restored calibration parameters. The calibration process
 * ensures optimal performance of the radar system by compensating for
 * manufacturing variations and environmental factors.
 *
 *
 * @note This function in this file is adapted from the Motion and Presence Detection Demo:
 *       ${MMWAVE_SDK_INSTALL_PATH}\examples\mmw_demo\motion_and_presence_detection\source\calibration\factory_cal.c
 *
 * @copyright (C) 2022-24 Texas Instruments Incorporated
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


#include "kernel/dpl/DebugP.h"
#include "kernel/dpl/SystemP.h"
#include "control/mmwave/mmwave.h"
#include "ti_board_open_close.h"

#include <board/flash.h>
#include <mmwavelink/mmwavelink.h>
#include <mmwavelink/include/rl_device.h>
#include <mmwavelink/include/rl_sensor.h>
#include <kernel/dpl/CacheP.h>

#include "system.h"
#include "defines.h"
#include "mmwave_basic.h"
#include "mmwave_control_config.h"
#include "factory_cal.h"



Mmw_calibData calibData __attribute__((aligned(8))) = {0};


int32_t restoreFactoryCal(void)
{
    uint16_t         calRfFreq = 0U;
    MMWave_calibCfg  factoryCalCfg = {0U};
    int32_t          retVal = SystemP_SUCCESS;
    int32_t          errCode;
    MMWave_ErrorLevel   errorLevel;
    int16_t          mmWaveErrorCode;
    int16_t          subsysErrorCode;

    /* Enable sensor boot time calibration: */
    factoryCalCfg.isFactoryCalEnabled = true;

    /*
    * @brief  FECSS RFS Boot calibration control:
    * | bits [0] | RESERVED
    * | bits [1] | VCO calibration ON/OFF control
    * | bits [2] | PD calibration ON/OFF control
    * | bits [3] | LODIST calibration ON/OFF control
    * | bits [4] | RESERVED 
    * | bits [5] | RX IFA calibration ON/OFF control
    * | bits [6] | RX Gain calibration ON/OFF control
    * | bits [7] | TX power calibration ON/OFF control
    */
    /* As part of Factory Calibration, enable all calibrations except RX IFA calibration */
    factoryCalCfg.fecRFFactoryCalCmd.h_CalCtrlBitMask = 0xCEU;
    factoryCalCfg.fecRFFactoryCalCmd.c_MiscCalCtrl = 0x0U;

    factoryCalCfg.fecRFFactoryCalCmd.c_CalRxGainSel = CLI_FACCALCFG_RX_GAIN;
    factoryCalCfg.fecRFFactoryCalCmd.c_CalTxBackOffSel[0] = CLI_FACCALCFG_TX_BACKOFF_SEL;
    factoryCalCfg.fecRFFactoryCalCmd.c_CalTxBackOffSel[1] = CLI_FACCALCFG_TX_BACKOFF_SEL;

    /* Calculate Calibration Rf Frequency. Use Center frequency of the bandwidth(being used in demo) for calibration */
    calRfFreq = (gSysContext.profileTimeCfg.w_ChirpRfFreqStart) + \
                ((((CLI_CHIRP_SLOPE * 256.0)/300) * (gSysContext.profileComCfg.h_ChirpRampEndTime * 0.1)) / 2);
    factoryCalCfg.fecRFFactoryCalCmd.xh_CalRfSlope = 0x4Du; /* 2.2Mhz per uSec*/


    factoryCalCfg.fecRFFactoryCalCmd.h_CalRfFreq = calRfFreq;
    if (gSysContext.channelCfg.h_TxChCtrlBitMask == 0x3) {
        factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[0] = 0x3;
        factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[1] = 0x1;
    } else {
        if (gSysContext.channelCfg.h_TxChCtrlBitMask == 0x1) {
            factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[0] = 0x1;
            factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[1] = 0x1;
        }
        if (gSysContext.channelCfg.h_TxChCtrlBitMask == 0x2) {
            factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[0] = 0x2;
            factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[1] = 0x2;
        }
    }

    /* Check if the device is RF-Trimmed */
    
    factoryCalCfg.ptrAteCalibration = NULL;
    factoryCalCfg.isATECalibEfused  = true;

    /* Read flash memory */
    retVal = Flash_read(gFlashHandle[0], CLI_FACCALCFG_FLASH_OFFSET, (uint8_t *) &calibData, sizeof(Mmw_calibData));
    CacheP_wb((uint8_t *) &calibData, sizeof(Mmw_calibData), CacheP_TYPE_ALL);

    if(retVal == SystemP_FAILURE)
    {
        DebugP_log("Could not read from Flash to restore Calibration data!");
        return -1;
    }

    /* Validate Calib data Magic number */
    if(calibData.magic != MMWDEMO_CALIB_STORE_MAGIC)
    {
        /* Header validation failed */
        DebugP_log("Error: MmwDemo Factory calibration data header validation failed.\r\n");
        return -1;
    }

    /* Populate calibration data pointer. */
    factoryCalCfg.ptrFactoryCalibData = &calibData.calibData;

    /* Disable factory calibration. */
    factoryCalCfg.isFactoryCalEnabled = false;

    retVal = MMWave_factoryCalibConfig(gSysContext.gCtrlHandle, &factoryCalCfg, &errCode);
    if (retVal != SystemP_SUCCESS)
    {

        /* Error: Unable to perform boot calibration */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Error: Unable to initialize the mmWave control module */
        DebugP_log("Error: mmWave Control Initialization failed [Error code %d] [errorLevel %d] [mmWaveErrorCode %d] [subsysErrorCode %d]\n", errCode, errorLevel, mmWaveErrorCode, subsysErrorCode);
        if (mmWaveErrorCode == MMWAVE_ERFSBOOTCAL)
        {
            DebugP_log("Error: Factory Calibration failure\n");
            return -1;
        }
        else
        {
            DebugP_log("Error: Invalid Factory calibration arguments\n");
            return -1;
        }
    }

    /* Configuring command for Run time CLPC calibration (Required if CLPC calib is enabled) */
    gSysContext.fecTxclpcCalCmd.c_CalMode = 0x0u; /* No Override */
    gSysContext.fecTxclpcCalCmd.c_CalTxBackOffSel[0] = factoryCalCfg.fecRFFactoryCalCmd.c_CalTxBackOffSel[0];
    gSysContext.fecTxclpcCalCmd.c_CalTxBackOffSel[1] = factoryCalCfg.fecRFFactoryCalCmd.c_CalTxBackOffSel[1];
    gSysContext.fecTxclpcCalCmd.h_CalRfFreq = factoryCalCfg.fecRFFactoryCalCmd.h_CalRfFreq;
    gSysContext.fecTxclpcCalCmd.xh_CalRfSlope = factoryCalCfg.fecRFFactoryCalCmd.xh_CalRfSlope;
    gSysContext.fecTxclpcCalCmd.c_TxPwrCalTxEnaMask[0] = factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[0];
    gSysContext.fecTxclpcCalCmd.c_TxPwrCalTxEnaMask[1] = factoryCalCfg.fecRFFactoryCalCmd.c_TxPwrCalTxEnaMask[1];

    return retVal;
}