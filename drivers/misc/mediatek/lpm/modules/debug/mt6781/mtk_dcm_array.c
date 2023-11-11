/*
 * Copyright (C) 2021 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

/* This file is generated by GenLP_setting.pl v1.5.7 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

const unsigned int AP_DCM_Golden_Setting_tcl_gs_dpidle_data[] = {
/*  Address     Mask        Golden Setting Value */
	0x0C53A2A0, 0x03000800, 0x03000800,/* CPU_PLLDIV_CFG0 */
	0x0C53A2A4, 0x03000800, 0x03000800,/* CPU_PLLDIV_CFG1 */
	0x0C53A2E0, 0x83000800, 0x83000800,/* BUS_PLLDIV_CFG */
	0x0C53A418, 0x00000001, 0x00000000,/* MCSI_CFG2 */
	0x0C53A440, 0x0000FFFF, 0x0000FFFF,/* MCSIC_DCM0 */
	0x0C53A510, 0x00270000, 0x00270000,/* MP_ADB_DCM_CFG4 */
	0x0C53A518, 0x0000003F, 0x0000003F,/* MP_MISC_DCM_CFG0 */
	0x0C53A5C0, 0x00070100, 0x00070100,/* MCUSYS_DCM_CFG0 */
	0x0C53A900, 0x00000005, 0x00000005,/* EMI_WFIFO */
	0x0C53C880, 0x0001000F, 0x0001000F,/* MP0_DCM_CFG0 */
	0x0C53C89C, 0x00000011, 0x00000011,/* MP0_DCM_CFG7 */
	0x0D0A007C, 0x00000002, 0x00000000,/* dbg_mode */
	0x10001070, 0x40F07C03, 0x40F00003,/* INFRA_BUS_DCM_CTRL */
	0x10001074, 0x30107C03, 0x30100003,/* PERI_BUS_DCM_CTRL */
	0x10001078, 0xBFFF01C1, 0x0BE00180,/* MEM_DCM_CTRL */
	0x100010A0, 0x0000000F, 0x00000000,/* P2P_RX_CLK_ON */
	0x1000A000, 0x80000000, 0x80000000,/* SEJ_CON */
	0x1000D1EC, 0x007FFFFF, 0x00000000,/* DCM_EN */
	0x10015008, 0x00000001, 0x00000001,/* DCM_CTRL */
	0x10015010, 0xFFFFFFFF, 0xFFFFFFFF,/* INF_CK_DCM_EN */
	0x10015014, 0x0000007F, 0x0000007F,/* OTHER_CK_DCM_EN */
	0x1001A208, 0x0000FFFF, 0x0000FFFF,/* DXCC_NEW_HWDCM_CFG */
	0x10219060, 0xFF000000, 0x00000000,/* EMI_CONM */
	0x10219068, 0xFF000000, 0x00000000,/* EMI_CONN */
	0x10219830, 0x00002000, 0x00000000,/* EMI_THRO_CTRL0 */
	0x10220038, 0xC4000007, 0xC0000007,/* DRAMC_PD_CTRL */
	0x1022003C, 0x80000000, 0x80000000,/* CLKAR */
	0x10225008, 0xFF000000, 0x00000000,/* CHN_EMI_CONB */
	0x10228284, 0x000BFF00, 0x00000000,/* MISC_CG_CTRL0 */
	0x1022828C, 0x07E000C0, 0x01000000,/* MISC_CG_CTRL2 */
	0x102282A4, 0x00000003, 0x00000000,/* MISC_CTRL2 */
	0x1022C0F0, 0x0000FFFF, 0x0000FFFF,/* GCE_CTL_INT0 */
	0x10230038, 0xC4000007, 0xC0000007,/* DRAMC_PD_CTRL */
	0x1023003C, 0x80000000, 0x80000000,/* CLKAR */
	0x10238284, 0x000BFF00, 0x00000000,/* MISC_CG_CTRL0 */
	0x1023828C, 0x07E000C0, 0x01000000,/* MISC_CG_CTRL2 */
	0x102382A4, 0x00000003, 0x00000000,/* MISC_CTRL2 */
	0x10443004, 0x00000100, 0x00000100,/* SSPM_MCLK_DIV */
	0x10443008, 0x001FFFFF, 0x001FBFFF,/* SSPM_DCM_CTRL */
	0x105C5F88, 0x00000007, 0x00000007,/* I3C0_CHN_HW_CG_EN */
	0x105C6F88, 0x00000007, 0x00000007,/* I2C1_CHN_HW_CG_EN */
	0x11008F88, 0x00000007, 0x00000007,/* I2C1_CHN_HW_CG_EN */
	0x1100DF88, 0x00000007, 0x00000007,/* I2C6_CHN_HW_CG_EN */
	0x1100FF88, 0x00000007, 0x00000007,/* I2C3_CHN_HW_CG_EN */
	0x11016F88, 0x00000007, 0x00000007,/* I2C5_CHN_HW_CG_EN */
	0x11200700, 0x000F0000, 0x00000000,/* RESREG */
	0x11210000, 0x60000000, 0x60000000,/* AUDIO_TOP_CON0 */
	0x112300B4, 0xFFA00000, 0x00000000,/* PATCH_BIT1 */
	0x112400B4, 0xFFA00000, 0x00000000,/* PATCH_BIT1 */
	0x13FBF010, 0x0003FF7F, 0x0000C03F,/* MFG_DCM_CON_0 */
	0x13FBF024, 0x00000001, 0x00000001,/* MFG_ASYNC_CON_1 */
	0x13FBF0B0, 0x00000700, 0x00000000,/* MFG_GLOBAL_CON */
	0x13FBF0B4, 0x00000010, 0x00000010,/* MFG_QCHANNEL_CON */
	0x14000120, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS0 */
	0x14000130, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS1 */
	0x14000140, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS0 */
	0x14000150, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS1 */
	0x140001B0, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS2 */
	0x140001C0, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS2 */
	0x14002300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x14003014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x14004014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1400E1F0, 0xFFFFFFFF, 0x00000000,/* DISP_POSTMASK_FUNC_DCM0 */
	0x1400E1F4, 0xFFFFFFFF, 0x00000000,/* DISP_POSTMASK_FUNC_DCM1 */
	0x14016050, 0x007FFFFF, 0x00000000,/* MMU_DCM_DIS */
	0x1401B300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1401C300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1401E300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1502E014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1502F300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1582E014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1602E014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1602F018, 0x00000001, 0x00000000,/* VDEC_DCM_CON */
	0x1602F218, 0x00000001, 0x00000000,/* LAT_DCM_CON */
	0x17010014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x17030300, 0x00000001, 0x00000000,/* JPGENC_DCM_CTRL */
	0x1A001014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1A002014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1A00C300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1A00D300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1A00F014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1A010014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1B000150, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS0 */
	0x1B000160, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS1 */
	0x1B000170, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS2 */
	0x1B000180, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS3 */
	0x1B000190, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS4 */
	0x1B0001A0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS0 */
	0x1B0001B0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS1 */
	0x1B0001C0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS2 */
	0x1B0001D0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS3 */
	0x1B0001E0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS4 */
	0x1B002014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1C00E300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1C00F014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1C10F014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
};

const unsigned int *AP_DCM_Golden_Setting_tcl_gs_dpidle =
		AP_DCM_Golden_Setting_tcl_gs_dpidle_data;

unsigned int AP_DCM_Golden_Setting_tcl_gs_dpidle_len = 288;

const unsigned int AP_DCM_Golden_Setting_tcl_gs_suspend_data[] = {
/*  Address     Mask        Golden Setting Value */
	0x0C53A2A0, 0x03000800, 0x03000800,/* CPU_PLLDIV_CFG0 */
	0x0C53A2A4, 0x03000800, 0x03000800,/* CPU_PLLDIV_CFG1 */
	0x0C53A2E0, 0x83000800, 0x83000800,/* BUS_PLLDIV_CFG */
	0x0C53A418, 0x00000001, 0x00000000,/* MCSI_CFG2 */
	0x0C53A440, 0x0000FFFF, 0x0000FFFF,/* MCSIC_DCM0 */
	0x0C53A510, 0x00270000, 0x00270000,/* MP_ADB_DCM_CFG4 */
	0x0C53A518, 0x0000003F, 0x0000003F,/* MP_MISC_DCM_CFG0 */
	0x0C53A5C0, 0x00070100, 0x00070100,/* MCUSYS_DCM_CFG0 */
	0x0C53A900, 0x00000005, 0x00000005,/* EMI_WFIFO */
	0x0C53C880, 0x0001000F, 0x0001000F,/* MP0_DCM_CFG0 */
	0x0C53C89C, 0x00000011, 0x00000011,/* MP0_DCM_CFG7 */
	0x0D0A007C, 0x00000002, 0x00000000,/* dbg_mode */
	0x10001070, 0x40F07C03, 0x40F00003,/* INFRA_BUS_DCM_CTRL */
	0x10001074, 0x30107C03, 0x30100003,/* PERI_BUS_DCM_CTRL */
	0x10001078, 0xBFFF01C1, 0x0BE00180,/* MEM_DCM_CTRL */
	0x100010A0, 0x0000000F, 0x00000000,/* P2P_RX_CLK_ON */
	0x1000A000, 0x80000000, 0x80000000,/* SEJ_CON */
	0x1000D1EC, 0x007FFFFF, 0x00000000,/* DCM_EN */
	0x10015008, 0x00000001, 0x00000001,/* DCM_CTRL */
	0x10015010, 0xFFFFFFFF, 0xFFFFFFFF,/* INF_CK_DCM_EN */
	0x10015014, 0x0000007F, 0x0000007F,/* OTHER_CK_DCM_EN */
	0x1001A208, 0x0000FFFF, 0x0000FFFF,/* DXCC_NEW_HWDCM_CFG */
	0x10219060, 0xFF000000, 0x00000000,/* EMI_CONM */
	0x10219068, 0xFF000000, 0x00000000,/* EMI_CONN */
	0x10219830, 0x00002000, 0x00000000,/* EMI_THRO_CTRL0 */
	0x10220038, 0xC4000007, 0xC0000007,/* DRAMC_PD_CTRL */
	0x1022003C, 0x80000000, 0x80000000,/* CLKAR */
	0x10225008, 0xFF000000, 0x00000000,/* CHN_EMI_CONB */
	0x10228284, 0x000BFF00, 0x00000000,/* MISC_CG_CTRL0 */
	0x1022828C, 0x07E000C0, 0x01000000,/* MISC_CG_CTRL2 */
	0x102282A4, 0x00000003, 0x00000000,/* MISC_CTRL2 */
	0x1022C0F0, 0x0000FFFF, 0x0000FFFF,/* GCE_CTL_INT0 */
	0x10230038, 0xC4000007, 0xC0000007,/* DRAMC_PD_CTRL */
	0x1023003C, 0x80000000, 0x80000000,/* CLKAR */
	0x10238284, 0x000BFF00, 0x00000000,/* MISC_CG_CTRL0 */
	0x1023828C, 0x07E000C0, 0x01000000,/* MISC_CG_CTRL2 */
	0x102382A4, 0x00000003, 0x00000000,/* MISC_CTRL2 */
	0x10443004, 0x00000100, 0x00000100,/* SSPM_MCLK_DIV */
	0x10443008, 0x001FFFFF, 0x001FBFFF,/* SSPM_DCM_CTRL */
	0x105C5F88, 0x00000007, 0x00000007,/* I3C0_CHN_HW_CG_EN */
	0x105C6F88, 0x00000007, 0x00000007,/* I2C1_CHN_HW_CG_EN */
	0x11008F88, 0x00000007, 0x00000007,/* I2C1_CHN_HW_CG_EN */
	0x1100DF88, 0x00000007, 0x00000007,/* I2C6_CHN_HW_CG_EN */
	0x1100FF88, 0x00000007, 0x00000007,/* I2C3_CHN_HW_CG_EN */
	0x11016F88, 0x00000007, 0x00000007,/* I2C5_CHN_HW_CG_EN */
	0x11200700, 0x000F0000, 0x00000000,/* RESREG */
	0x11210000, 0x60000000, 0x60000000,/* AUDIO_TOP_CON0 */
	0x112300B4, 0xFFA00000, 0x00000000,/* PATCH_BIT1 */
	0x112400B4, 0xFFA00000, 0x00000000,/* PATCH_BIT1 */
	0x13FBF010, 0x0003FF7F, 0x0000C03F,/* MFG_DCM_CON_0 */
	0x13FBF024, 0x00000001, 0x00000001,/* MFG_ASYNC_CON_1 */
	0x13FBF0B0, 0x00000700, 0x00000000,/* MFG_GLOBAL_CON */
	0x13FBF0B4, 0x00000010, 0x00000010,/* MFG_QCHANNEL_CON */
	0x14000120, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS0 */
	0x14000130, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS1 */
	0x14000140, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS0 */
	0x14000150, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS1 */
	0x140001B0, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS2 */
	0x140001C0, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS2 */
	0x14002300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x14003014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x14004014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1400E1F0, 0xFFFFFFFF, 0x00000000,/* DISP_POSTMASK_FUNC_DCM0 */
	0x1400E1F4, 0xFFFFFFFF, 0x00000000,/* DISP_POSTMASK_FUNC_DCM1 */
	0x14016050, 0x007FFFFF, 0x00000000,/* MMU_DCM_DIS */
	0x1401B300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1401C300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1401E300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1502E014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1502F300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1582E014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1602E014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1602F018, 0x00000001, 0x00000000,/* VDEC_DCM_CON */
	0x1602F218, 0x00000001, 0x00000000,/* LAT_DCM_CON */
	0x17010014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x17030300, 0x00000001, 0x00000000,/* JPGENC_DCM_CTRL */
	0x1A001014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1A002014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1A00C300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1A00D300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1A00F014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1A010014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1B000150, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS0 */
	0x1B000160, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS1 */
	0x1B000170, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS2 */
	0x1B000180, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS3 */
	0x1B000190, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS4 */
	0x1B0001A0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS0 */
	0x1B0001B0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS1 */
	0x1B0001C0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS2 */
	0x1B0001D0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS3 */
	0x1B0001E0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS4 */
	0x1B002014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1C00E300, 0x000000FE, 0x00000000,/* SMI_DCM */
	0x1C00F014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
	0x1C10F014, 0x0000FFF0, 0x0000FFF0,/* SMI_LARB_CON_SET */
};

const unsigned int *AP_DCM_Golden_Setting_tcl_gs_suspend =
		AP_DCM_Golden_Setting_tcl_gs_suspend_data;

unsigned int AP_DCM_Golden_Setting_tcl_gs_suspend_len = 288;

const unsigned int AP_DCM_Golden_Setting_tcl_gs_sodi_data[] = {
/*  Address     Mask        Golden Setting Value */
	0x0D0A007C, 0x00000002, 0x00000000,/* dbg_mode */
	0x1000D1EC, 0x007FFFFF, 0x00000000,/* DCM_EN */
	0x10015008, 0x00000001, 0x00000001,/* DCM_CTRL */
	0x10015010, 0xFFFFFFFF, 0xFFFFFFFF,/* INF_CK_DCM_EN */
	0x10015014, 0x0000007F, 0x0000007F,/* OTHER_CK_DCM_EN */
	0x1001A208, 0x0000FFFF, 0x0000FFFF,/* DXCC_NEW_HWDCM_CFG */
	0x10219060, 0xFF000000, 0x00000000,/* EMI_CONM */
	0x10219068, 0xFF000000, 0x00000000,/* EMI_CONN */
	0x10219830, 0x00002000, 0x00000000,/* EMI_THRO_CTRL0 */
	0x10225008, 0xFF000000, 0x00000000,/* CHN_EMI_CONB */
	0x10228284, 0x000BFF00, 0x00000000,/* MISC_CG_CTRL0 */
	0x1022828C, 0x07E000C0, 0x01000000,/* MISC_CG_CTRL2 */
	0x102282A4, 0x00000003, 0x00000000,/* MISC_CTRL2 */
	0x10238284, 0x000BFF00, 0x00000000,/* MISC_CG_CTRL0 */
	0x1023828C, 0x07E000C0, 0x01000000,/* MISC_CG_CTRL2 */
	0x102382A4, 0x00000003, 0x00000000,/* MISC_CTRL2 */
	0x112300B4, 0xFFA00000, 0x00000000,/* PATCH_BIT1 */
	0x112400B4, 0xFFA00000, 0x00000000,/* PATCH_BIT1 */
	0x14000120, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS0 */
	0x14000130, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS1 */
	0x14000140, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS0 */
	0x14000150, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS1 */
	0x140001B0, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_1ST_DIS2 */
	0x140001C0, 0xFFFFFFFF, 0x00000000,/* MMSYS_HW_DCM_2ND_DIS2 */
	0x1400E1F0, 0xFFFFFFFF, 0x00000000,/* DISP_POSTMASK_FUNC_DCM0 */
	0x1400E1F4, 0xFFFFFFFF, 0x00000000,/* DISP_POSTMASK_FUNC_DCM1 */
	0x14016050, 0x007FFFFF, 0x00000000,/* MMU_DCM_DIS */
	0x1602F018, 0x00000001, 0x00000000,/* VDEC_DCM_CON */
	0x1602F218, 0x00000001, 0x00000000,/* LAT_DCM_CON */
	0x17030300, 0x00000001, 0x00000000,/* JPGENC_DCM_CTRL */
	0x1B000150, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS0 */
	0x1B000160, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS1 */
	0x1B000170, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS2 */
	0x1B000180, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS3 */
	0x1B000190, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_1ST_DIS4 */
	0x1B0001A0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS0 */
	0x1B0001B0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS1 */
	0x1B0001C0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS2 */
	0x1B0001D0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS3 */
	0x1B0001E0, 0xFFFFFFFF, 0x00000000,/* MDPSYS_HW_DCM_2ND_DIS4 */
};

const unsigned int *AP_DCM_Golden_Setting_tcl_gs_sodi =
		AP_DCM_Golden_Setting_tcl_gs_sodi_data;

unsigned int AP_DCM_Golden_Setting_tcl_gs_sodi_len = 120;
