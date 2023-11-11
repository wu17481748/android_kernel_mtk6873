/*
 * Copyright (C) 2018 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __TSPMIC_SETTINGS_H__
#define __TSPMIC_SETTINGS_H__

#include <mach/upmu_hw.h>
#include <mach/mtk_pmic_wrap.h>

/*=============================================================
 * Genernal
 *=============================================================
 */
#define MIN(_a_, _b_) ((_a_) > (_b_) ? (_b_) : (_a_))
#define MAX(_a_, _b_) ((_a_) > (_b_) ? (_a_) : (_b_))
#define _BIT_(_bit_)		(unsigned int)(1 << (_bit_))
#define _BITMASK_(_bits_)	(((unsigned int) -1 >> (31 - ((1) ?	\
				_bits_))) & ~((1U << ((0) ? _bits_)) - 1))

#define mtktspmic_TEMP_CRIT 150000 /* 150.000 degree Celsius */
#define y_pmic_repeat_times	1
#define THERMAL_USE_IIO_CHANNEL

#define mtktspmic_info(fmt, args...)   pr_info("[Thermal/TZ/PMIC] " fmt, ##args)


#define mtktspmic_dprintk(fmt, args...)   \
	do {								\
		if (mtktspmic_debug_log == 1) {				\
			pr_notice("[Thermal/TZ/PMIC] " fmt, ##args); \
		}							\
	} while (0)

extern int mtktspmic_debug_log;
extern void mtktspmic_cali_prepare(void);
extern void mtktspmic_cali_prepare2(void);
extern void mtktspmic_get_from_dts(void);
extern int mtktspmic_get_hw_temp(void);
extern int mt6366vcore_get_hw_temp(void);
extern int mt6366vproc_get_hw_temp(void);
extern int mt6366vgpu_get_hw_temp(void);
extern int mt6366tsx_get_hw_temp(void);
extern int mt6366dcxo_get_hw_temp(void);
extern u32 pmic_Read_Efuse_HPOffset(int i);

#endif	/* __TSPMIC_SETTINGS_H__ */
