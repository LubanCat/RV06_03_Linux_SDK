#include <assert.h>
/*
 * Copyright 2021 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef __SAMPLE_COMM_ISP_H__
#define __SAMPLE_COMM_ISP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <fcntl.h>
#ifdef RKAIQ_GRP
#include <rk_aiq_user_api_camgroup.h>
#endif
#include <rk_aiq_user_api_imgproc.h>
#include <rk_aiq_user_api_sysctl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

/*
 * stream on:
 * 1) ISP Init
 * 2) ISP Run
 * 3) VI enable and stream on
 *
 * stream off:
 * 4) VI disable
 * 5) ISP Stop
 */
/*
typedef enum {
 RK_AIQ_WORKING_MODE_NORMAL,
 RK_AIQ_WORKING_MODE_ISP_HDR2    = 0x10,
 RK_AIQ_WORKING_MODE_ISP_HDR3    = 0x20,
 //RK_AIQ_WORKING_MODE_SENSOR_HDR = 10, // sensor built-in hdr mode
} rk_aiq_working_mode_t;
*/

typedef struct _rkIspLdchPath {
	RK_CHAR *pCLdchPath;
	RK_CHAR *pCLdchName;
} rk_isp_ldch_path;

#ifdef RKAIQ_GRP
typedef struct _rkIspGroupParamCtx {
	RK_S32 CamGroupId;
	RK_S32 fps;
	rk_aiq_working_mode_t WDRMode;
	bool MultiCam;
	rk_aiq_ldch_update_lut_mode_t update_lut_mode;
	union {
		rk_isp_ldch_path path[2];
		void *buffer[2];
	} mesh_ldch;
	rk_aiq_camgroup_instance_cfg_t *pCamGroupCfg;
} rk_isp_group_param_ctx;
#endif
RK_S32 SAMPLE_COMM_PreInit_devBufCnt(RK_S32 CamId, RK_S32 Bufcnt);
RK_S32 SAMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
                            const char *iq_file_dir);

RK_S32 SAMPLE_COMM_ISP_RegMemsSensorIntf(RK_S32 CamId, rk_aiq_mems_sensor_intf_t *api);
RK_S32 SAMPLE_COMM_ISP_UpdateIq(RK_S32 CamId, char *iqfile);
RK_S32 SAMPLE_COMM_ISP_Stop(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_SetFecEn(RK_S32 CamId, RK_BOOL bFECEnable);
RK_S32 SAMPLE_COMM_ISP_Run(RK_S32 CamId); // isp stop before vi streamoff
RK_S32 SAMPLE_COMM_ISP_DumpExpInfo(RK_S32 CamId, rk_aiq_working_mode_t WDRMode);
RK_S32 SAMPLE_COMM_ISP_SetFrameRate(RK_S32 CamId, RK_U32 uFps);
RK_S32 SAMPLE_COMM_ISP_GetFrameRate(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_SetLDCH(RK_S32 CamId, RK_U32 level, RK_BOOL bIfEnable);
RK_S32 SAMPLE_COMM_ISP_SetWDRModeDyn(RK_S32 CamId, rk_aiq_working_mode_t WDRMode);
RK_S32 SAMPLE_COMM_ISP_SET_Brightness(RK_S32 CamId, RK_U32 value);
RK_S32 SAMPLE_COMM_ISP_SET_Contrast(RK_S32 CamId, RK_U32 value);
RK_S32 SAMPLE_COMM_ISP_SET_Saturation(RK_S32 CamId, RK_U32 value);
RK_S32 SAMPLE_COMM_ISP_SET_Sharpness(RK_S32 CamId, RK_U32 value);
RK_S32 SAMPLE_COMM_ISP_SET_ManualExposureAutoGain(RK_S32 CamId, RK_U32 u32Shutter);
RK_S32 SAMPLE_COMM_ISP_SET_ManualExposureManualGain(RK_S32 CamId, RK_U32 u32Shutter,
                                                    RK_U32 u32Gain);
RK_S32 SAMPLE_COMM_ISP_SET_AutoExposure(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_SET_Exposure(RK_S32 CamId, RK_BOOL bIsAutoExposure, RK_U32 bIsAGC,
                                    RK_U32 u32ElectronicShutter, RK_U32 u32Agc);
RK_S32 SAMPLE_COMM_ISP_SET_BackLight(RK_S32 CamId, RK_BOOL bEnable, RK_U32 u32Strength);
RK_S32 SAMPLE_COMM_ISP_SET_LightInhibition(RK_S32 CamId, RK_BOOL bEnable,
                                           RK_U8 u8Strength, RK_U8 u8Level);
RK_S32 SAMPLE_COMM_ISP_SET_CPSL_CFG(RK_S32 CamId, rk_aiq_cpsl_cfg_t *cpsl);
RK_S32 SAMPLE_COMM_ISP_SET_OpenColorCloseLed(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_SET_GrayOpenLed(RK_S32 CamId, RK_U8 u8Strength);
RK_S32 SAMPLE_COMMON_ISP_SET_AutoWhiteBalance(RK_S32 CamId);
RK_S32 SAMPLE_COMMON_ISP_SET_ManualWhiteBalance(RK_S32 CamId, RK_U32 u32RGain,
                                                RK_U32 u32GGain, RK_U32 u32BGain);
RK_S32 SAMPLE_COMMON_ISP_GET_WhiteBalanceGain(RK_S32 CamId, rk_aiq_wb_gain_t *gain);
RK_S32 SAMPLE_COMMON_ISP_SET_DNRStrength(RK_S32 CamId, RK_U32 u32Mode, RK_U32 u322DValue,
                                         RK_U32 u323Dvalue);
RK_S32 SAMPLE_COMMON_ISP_GET_DNRStrength(RK_S32 CamId, RK_U32 *u322DValue,
                                         RK_U32 *u323Dvalue);

RK_S32 SAMPLE_COMMON_ISP_SET_Flicker(RK_S32 CamId, RK_U8 u32Flicker);
RK_S32 SAMPLE_COMM_ISP_SET_HDR(RK_S32 CamId, rk_aiq_working_mode_t mode);
RK_S32 SAMPLE_COMM_ISP_SET_DefogEnable(RK_S32 CamId, RK_U32 u32Mode);
RK_S32 SAMPLE_COMM_ISP_SET_DefogStrength(RK_S32 CamId, RK_U32 u32Mode, RK_U32 u32Value);
RK_S32 SAMPLE_COMM_ISP_SET_Correction(RK_S32 CamId, RK_U32 u32Mode, RK_U32 u32Value);
RK_S32 SAMPLE_COMM_ISP_SetMirrorFlip(int CamId, RK_U32 u32Value);
RK_S32 SAMPLE_COMM_ISP_SET_BypassStreamRotation(RK_S32 CamId, RK_S32 S32Rotation);
RK_S32 SAMPLE_COMM_ISP_SET_Crop(RK_S32 CamId, rk_aiq_rect_t rect);

RK_S32 SAMPLE_COMM_ISP_GetAINrParams(RK_S32 CamId, rk_ainr_param *param);
#ifdef RKAIQ_GRP
XCamReturn SAMPLE_COMM_ISP_CamGroup_setMeshToLdch(int CamGrpId, uint8_t SetLdchMode,
                                                  uint16_t **LdchMesh);
RK_S32 SAMPLE_COMM_ISP_CamGroup_Init(RK_S32 CamGroupId, rk_aiq_working_mode_t WDRMode,
                                     bool MultiCam, int OpenLdch, void *LdchMesh[],
                                     rk_aiq_camgroup_instance_cfg_t *pCamGroupCfg);
RK_S32 SAMPLE_COMM_ISP_CamGroup_Stop(RK_S32 CamGroupId);
RK_S32 SAMPLE_COMM_ISP_CamGroup_SetFrameRate(RK_S32 CamId, RK_U32 uFps);
#endif

RK_S32 SAMPLE_COMM_ISP_SingleFrame(int cam_id);
RK_S32 SAMPLE_COMM_ISP_MultiFrame(int cam_id);
RK_BOOL SAMPLE_COMM_ISP_IsExpBigChange(int cam_id);
RK_BOOL SAMPLE_COMM_ISP_IsExpConverge(int cam_id);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __SAMPLE_COMMON_ISP_H__ */
