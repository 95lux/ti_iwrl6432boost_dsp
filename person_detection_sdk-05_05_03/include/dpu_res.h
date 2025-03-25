#ifndef DPU_RES_H
#define DPU_RES_H

/**
 * @file dpu_res.h
 * @brief Resource definitions for Data Processing Unit (DPU) configurations.
 *
 * This file defines the hardware resources and configurations for the Data Processing Units (DPUs)
 * used in the radar signal processing pipeline. It includes EDMA (Enhanced Direct Memory Access)
 * channel assignments, shadow configurations, and event queue mappings for various DPUs such as
 * Range Processing, Direction of Arrival (DoA), CFAR (Constant False Alarm Rate), and Micro Doppler.
 *
 * The configurations are derived from the Motion and Presence Detection Demo provided in the
 * TI mmWave SDK. These resources are critical for ensuring proper signal processing and data
 * transfer between the radar front-end and the processing units.
 *
 * @note This file is adapted from the Motion and Presence Detection Demo:
 *       ${MMWAVE_SDK_INSTALL_PATH}\examples\mmw_demo\motion_and_presence_detection\source\mmw_res.h
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



#ifdef __cplusplus
extern "C" {
#endif

#include <drivers/edma.h>
#include <drivers/hw_include/cslr_soc.h>

/*******************************************************************************
 * Resources for Object Detection DPC, currently the only DPC and hwa/edma
 * resource used in the demo.
 *******************************************************************************/

#define DPC_OBJDET_HWA_WINDOW_RAM_OFFSET                               0

#define DPC_OBJDET_EDMA_SHADOW_BASE                                    SOC_EDMA_NUM_DMACH

/* Range DPU */
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_CH                             EDMA_APPSS_TPCC_B_EVT_CHIRP_AVAIL_IRQ
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SHADOW_PING                    (DPC_OBJDET_EDMA_SHADOW_BASE + 0)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SHADOW_PONG                    (DPC_OBJDET_EDMA_SHADOW_BASE + 1)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_EVENT_QUE                      0
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SIG_CH                         EDMA_APPSS_TPCC_B_EVT_FREE_0
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SIG_SHADOW                     (DPC_OBJDET_EDMA_SHADOW_BASE + 2)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SIG_EVENT_QUE                  0

#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MAJOR_PING_CH                 EDMA_APPSS_TPCC_B_EVT_HWA_DMA_REQ0
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MAJOR_PING_SHADOW             (DPC_OBJDET_EDMA_SHADOW_BASE + 3)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MAJOR_PING_EVENT_QUE          0

#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MINOR_PING_CH                 EDMA_APPSS_TPCC_B_EVT_FREE_1
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MINOR_PING_SHADOW             (DPC_OBJDET_EDMA_SHADOW_BASE + 4)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MINOR_PING_EVENT_QUE          0

#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PING_CH                     EDMA_APPSS_TPCC_B_EVT_FREE_2
#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PING_SHADOW_0               (DPC_OBJDET_EDMA_SHADOW_BASE + 5)
#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PING_SHADOW_1               (DPC_OBJDET_EDMA_SHADOW_BASE + 6)
#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PING_EVENT_QUE              0

#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MAJOR_PONG_CH                 EDMA_APPSS_TPCC_B_EVT_HWA_DMA_REQ1
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MAJOR_PONG_SHADOW             (DPC_OBJDET_EDMA_SHADOW_BASE + 7)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MAJOR_PONG_EVENT_QUE          0

#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MINOR_PONG_CH                 EDMA_APPSS_TPCC_B_EVT_FREE_3
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MINOR_PONG_SHADOW             (DPC_OBJDET_EDMA_SHADOW_BASE + 8)
#define DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_MINOR_PONG_EVENT_QUE          0

#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PONG_CH                     EDMA_APPSS_TPCC_B_EVT_FREE_4
#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PONG_SHADOW_0               (DPC_OBJDET_EDMA_SHADOW_BASE + 9)
#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PONG_SHADOW_1               (DPC_OBJDET_EDMA_SHADOW_BASE + 10)
#define DPC_OBJDET_DPU_RANGEPROC_EVT_DECIM_PONG_EVENT_QUE              0

/* DoA DPU */
#define DPC_OBJDET_DPU_DOAPROC_EDMAIN_PING_CH                         EDMA_APPSS_TPCC_B_EVT_FREE_5
#define DPC_OBJDET_DPU_DOAPROC_EDMAIN_PING_SHADOW                     (DPC_OBJDET_EDMA_SHADOW_BASE + 11)
#define DPC_OBJDET_DPU_DOAPROC_EDMAIN_PING_EVENT_QUE                  0

#define DPC_OBJDET_DPU_DOAPROC_EDMAIN_PONG_CH                         EDMA_APPSS_TPCC_B_EVT_FREE_6
#define DPC_OBJDET_DPU_DOAPROC_EDMAIN_PONG_SHADOW                     (DPC_OBJDET_EDMA_SHADOW_BASE + 12)
#define DPC_OBJDET_DPU_DOAPROC_EDMAIN_PONG_EVENT_QUE                  0

#define DPC_OBJDET_DPU_DOAPROC_EDMA_HOT_SIG_CH                        EDMA_APPSS_TPCC_B_EVT_FREE_7
#define DPC_OBJDET_DPU_DOAPROC_EDMA_HOT_SIG_SHADOW                    (DPC_OBJDET_EDMA_SHADOW_BASE + 13)
#define DPC_OBJDET_DPU_DOAPROC_EDMA_HOT_SIG_EVENT_QUE                 0

#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_DET_MATRIX_CH                  EDMA_APPSS_TPCC_B_EVT_HWA_DMA_REQ2
#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_DET_MATRIX_SHADOW              (DPC_OBJDET_EDMA_SHADOW_BASE + 14)
#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_DET_MATRIX_EVENT_QUE           0

#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_ELEVIND_MATRIX_CH              EDMA_APPSS_TPCC_B_EVT_FREE_8
#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_ELEVIND_MATRIX_SHADOW          (DPC_OBJDET_EDMA_SHADOW_BASE + 15)
#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_ELEVIND_MATRIX_EVENT_QUE       0

#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_DOPIND_MATRIX_CH               EDMA_APPSS_TPCC_B_EVT_FREE_9
#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_DOPIND_MATRIX_SHADOW           (DPC_OBJDET_EDMA_SHADOW_BASE + 16)
#define DPC_OBJDET_DPU_DOAPROC_EDMAOUT_DOPIND_MATRIX_EVENT_QUE        0

#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMAOUT_DET_MATRIX_CH          EDMA_APPSS_TPCC_B_EVT_HWA_DMA_REQ3
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMAOUT_DET_MATRIX_SHADOW      (DPC_OBJDET_EDMA_SHADOW_BASE + 17)
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMAOUT_DET_MATRIX_EVENT_QUE   0

#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMAIN_CH                      EDMA_APPSS_TPCC_B_EVT_FREE_10
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMAIN_SHADOW                  (DPC_OBJDET_EDMA_SHADOW_BASE + 18)
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMAIN_EVENT_QUE               0

#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMA_HOT_SIG_CH                EDMA_APPSS_TPCC_B_EVT_FREE_11
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMA_HOT_SIG_SHADOW            (DPC_OBJDET_EDMA_SHADOW_BASE + 19)
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMA_HOT_SIG_EVENT_QUE         0

#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMA_CHAIN_BACK_CH             EDMA_APPSS_TPCC_B_EVT_FREE_12
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMA_CHAIN_BACK_SHADOW         (DPC_OBJDET_EDMA_SHADOW_BASE + 20)
#define DPC_OBJDET_DPU_DOAPROC_INTER_LOOP_EDMA_CHAIN_BACK_EVENT_QUE      0

/* CFAR DPU */
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAIN_CH                               EDMA_APPSS_TPCC_B_EVT_FREE_13
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAIN_SHADOW                           (DPC_OBJDET_EDMA_SHADOW_BASE + 21)
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAIN_EVENT_QUE                        0

#define DPC_OBJDET_DPU_CFAR_PROC_EDMAIN_SIG_CH                           EDMA_APPSS_TPCC_B_EVT_FREE_14
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAIN_SIG_SHADOW                       (DPC_OBJDET_EDMA_SHADOW_BASE + 22)
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAIN_SIG_EVENT_QUE                    0

#define DPC_OBJDET_DPU_CFAR_PROC_EDMAOUT_RNG_PROFILE_CH                  EDMA_APPSS_TPCC_B_EVT_HWA_DMA_REQ4
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAOUT_RNG_PROFILE_SHADOW              (DPC_OBJDET_EDMA_SHADOW_BASE + 23)
#define DPC_OBJDET_DPU_CFAR_PROC_EDMAOUT_RNG_PROFILE_EVENT_QUE           0

/* Micro Doppler DPU */
#define DPC_OBJDET_DPU_UDOP_PROC_EDMA_RESET_CH                           EDMA_APPSS_TPCC_B_EVT_FREE_15
#define DPC_OBJDET_DPU_UDOP_PROC_EDMA_RESET_SHADOW                       (DPC_OBJDET_EDMA_SHADOW_BASE + 24)
#define DPC_OBJDET_DPU_UDOP_PROC_EDMARESET_EVENT_QUE                     0

#define DPC_OBJDET_DPU_UDOP_PROC_EDMAIN_CH                               EDMA_APPSS_TPCC_B_EVT_FREE_16
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAIN_SHADOW                           (DPC_OBJDET_EDMA_SHADOW_BASE + 25)
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAIN_EVENT_QUE                        0

#define DPC_OBJDET_DPU_UDOP_PROC_EDMAIN_SIG_CH                           EDMA_APPSS_TPCC_B_EVT_FREE_17
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAIN_SIG_SHADOW                       (DPC_OBJDET_EDMA_SHADOW_BASE + 26)
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAIN_SIG_EVENT_QUE                    0

#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_CHAIN_CH                        EDMA_APPSS_TPCC_B_EVT_HWA_DMA_REQ5
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_CHAIN0_SHADOW                   (DPC_OBJDET_EDMA_SHADOW_BASE + 27)
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_CHAIN1_SHADOW                   (DPC_OBJDET_EDMA_SHADOW_BASE + 28)
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_CHAIN_EVENT_QUE                 0

#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_UDOPPLER_CH                     EDMA_APPSS_TPCC_B_EVT_FREE_18
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_UDOPPLER_SHADOW                 (DPC_OBJDET_EDMA_SHADOW_BASE + 29)
#define DPC_OBJDET_DPU_UDOP_PROC_EDMAOUT_UDOPPLER_EVENT_QUE              0

#ifdef __cplusplus
}
#endif

#endif /* DPU_RES_H */

