#ifndef MMWAVE_CONTROL_CONFIG_H
#define MMWAVE_CONTROL_CONFIG_H

// #include <drivers/hw_include/cslr_adcbuf.h>
// #include <drivers/hw_include/xwrL64xx/cslr_soc_baseaddress.h>

// /* mmWave SDK Include Files: */
// #include <control/mmwave/mmwave.h>
// #include <kernel/dpl/DebugP.h>
// #include <utils/testlogger/logger.h>

// #include "common/sys_defs.h"

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/*!
 * @brief  Sensor Perchirp LUT, total 64 bytes used, 4 values per params
 */
typedef struct
{
    uint32_t StartFreqHighRes[4]; /* LUT address 0 */
    uint32_t StartFreqLowRes[4]; /* LUT address 16 */
    int16_t ChirpSlope[4]; /* LUT address 32 */
    uint16_t ChirpIdleTime[4]; /* LUT address 40 */
    uint16_t ChirpAdcStartTime[4]; /* LUT address 48 */
    int16_t ChirpTxStartTime[4]; /* LUT address 56 */
    uint8_t ChirpTxEn[4]; /* LUT address 64 */
    uint8_t ChirpBpmEn[4]; /* LUT address 68 */
} T_SensPerChirpLut;

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern MMWave_Handle gCtrlHandle;
extern T_SensPerChirpLut* sensPerChirpLuTable;

T_RL_API_SENS_CHIRP_PROF_COMN_CFG profileComCfg;
T_RL_API_SENS_CHIRP_PROF_TIME_CFG profileTimeCfg;
T_RL_API_FECSS_RF_PWR_CFG_CMD channelCfg;
T_RL_API_SENS_FRAME_CFG frameCfg;


/**************************************************************************
 ************************* Function Declarations **************************
 **************************************************************************/
static void Mmwave_populateDefaultProfileCfg (T_RL_API_SENS_CHIRP_PROF_COMN_CFG* ptrProfileCfg, T_RL_API_SENS_CHIRP_PROF_TIME_CFG* ptrProfileTimeCfg);
static void Mmwave_populateDefaultChirpCfg (T_RL_API_SENS_PER_CHIRP_CFG* ptrChirpCfg, T_RL_API_SENS_PER_CHIRP_CTRL* ptrChirpCtrl);
void MMWave_populateChannelCfg();
void Mmwave_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg);
void Mmwave_populateDefaultStartCfg (MMWave_StrtCfg* ptrStartCfg);
void Mmwave_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg);
void Mmwave_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg);

#endif /* MMWAVE_CONTROL_CONFIG_H */