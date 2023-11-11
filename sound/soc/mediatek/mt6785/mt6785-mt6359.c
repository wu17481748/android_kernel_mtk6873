// SPDX-License-Identifier: GPL-2.0
//
// mt6785-mt6359.c  --  mt6785 mt6359 ALSA SoC machine driver
//
// Copyright (c) 2018 MediaTek Inc.
// Author: Eason Yen <eason.yen@mediatek.com>

#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "mt6785-afe-common.h"
#include "mt6785-afe-clk.h"
#include "mt6785-afe-gpio.h"
#include "../../codecs/mt6359.h"
#include "../common/mtk-sp-spk-amp.h"

#ifdef OPLUS_ARCH_EXTENDS
#include "../../codecs/audio_extend_drv.h"
#endif /* OPLUS_ARCH_EXTENDS */

#ifdef CONFIG_SND_SOC_MT8185_EVB
#include <linux/of_gpio.h>
#include <linux/gpio.h>

#define EXT_SPK_HP_AMP_W_NAME "Ext_Headphone_Amp_Switch"

struct pinctrl *pinctrl_ext_hp_amp;
struct audhpamp_gpio_attr {
	const char *name;
	bool gpio_prepare;
	struct pinctrl_state *gpioctrl;
};

enum audhpamp_gpio_type {
	GPIO_EXTHPAMP_OFF = 0,
	GPIO_EXTHPAMP_ON,
	GPIO_NUM
};

static struct audhpamp_gpio_attr audhpamp_gpios[GPIO_NUM] = {
	[GPIO_EXTHPAMP_OFF] = {"ext_hp_amp_off", false, NULL},
	[GPIO_EXTHPAMP_ON] = {"ext_hp_amp_on", false, NULL},
};

static inline int audio_exthpamp_setup_gpio(struct platform_device *device)
{
	int index_gpio = 0;
	int ret;

	pinctrl_ext_hp_amp = devm_pinctrl_get(&device->dev);
	if (IS_ERR(pinctrl_ext_hp_amp)) {
		ret = PTR_ERR(pinctrl_ext_hp_amp);
		pr_info("[audio] Cannot find ext_hp_amp ret = %d !\n", ret);
		return ret;
	}
	for (index_gpio = 0; index_gpio < ARRAY_SIZE(audhpamp_gpios);
			index_gpio++) {
		audhpamp_gpios[index_gpio].gpioctrl =
			pinctrl_lookup_state(pinctrl_ext_hp_amp,
			audhpamp_gpios[index_gpio].name);
		if (IS_ERR(audhpamp_gpios[index_gpio].gpioctrl)) {
			ret = PTR_ERR(audhpamp_gpios[index_gpio].gpioctrl);
			pr_info("[audio] %s lookup_state %s fail %d\n",
			__func__, audhpamp_gpios[index_gpio].name, ret);
		} else {
			audhpamp_gpios[index_gpio].gpio_prepare = true;
			pr_debug("[audio] %s lookup_state %s success!\n",
				 __func__, audhpamp_gpios[index_gpio].name);
		}
	}
	return 0;
}

static void audio_exthpamp_enable(void)
{
	if (audhpamp_gpios[GPIO_EXTHPAMP_ON].gpio_prepare) {
		pinctrl_select_state(pinctrl_ext_hp_amp,
			audhpamp_gpios[GPIO_EXTHPAMP_ON].gpioctrl);
		pr_info("[audio] set audhpamp_gpios[GPIO_EXTHPAMP_ON] pins\n");
	} else {
		pr_info("[audio] audhpamp_gpios[GPIO_EXTHPAMP_ON] pins are not prepared!\n");
	}
}

static void audio_exthpamp_disable(void)
{
	if (audhpamp_gpios[GPIO_EXTHPAMP_OFF].gpio_prepare) {
		pinctrl_select_state(pinctrl_ext_hp_amp,
			audhpamp_gpios[GPIO_EXTHPAMP_OFF].gpioctrl);
		pr_info("[audio] set aud_gpios[GPIO_EXTHPAMP_OFF] pins\n");
	} else {
		pr_info("[audio] aud_gpios[GPIO_EXTHPAMP_OFF] pins are not prepared!\n");
	}
}
#endif
/*
 * if need additional control for the ext spk amp that is connected
 * after Lineout Buffer / HP Buffer on the codec, put the control in
 * mt6785_mt6359_spk_amp_event()
 */
#define EXT_SPK_AMP_W_NAME "Ext_Speaker_Amp"

static const char *const mt6785_spk_type_str[] = {MTK_SPK_NOT_SMARTPA_STR,
						  MTK_SPK_RICHTEK_RT5509_STR,
						  MTK_SPK_MEDIATEK_MT6660_STR,
						  MTK_SPK_NXP_TFA98XX_STR
						  };
static const char *const mt6785_spk_i2s_type_str[] = {MTK_SPK_I2S_0_STR,
						      MTK_SPK_I2S_1_STR,
						      MTK_SPK_I2S_2_STR,
						      MTK_SPK_I2S_3_STR,
						      MTK_SPK_I2S_5_STR};

static const struct soc_enum mt6785_spk_type_enum[] = {
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(mt6785_spk_type_str),
			    mt6785_spk_type_str),
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(mt6785_spk_i2s_type_str),
			    mt6785_spk_i2s_type_str),
};

#ifdef OPLUS_BUG_STABILITY
#ifdef CONFIG_SND_SOC_AW87339
extern unsigned char aw87339_audio_kspk(void);
extern unsigned char aw87339_audio_drcv(void);
extern unsigned char aw87339_audio_off(void);
extern unsigned char aw87339_audio_voicespk(void);

enum {
	SPEAKER_SCENE_PLAYBACK = 0,
	SPEAKER_SCENE_VOICE,
	SPEAKER_SCENE_NUM
};
static int aw87339_speaker_scene = SPEAKER_SCENE_PLAYBACK;
static const char *const aw87339_spk_scene[] = { "Playback", "Voice" };
static const struct soc_enum aw87339_spk_scene_enum =
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(aw87339_spk_scene), aw87339_spk_scene);

static int aw87339_spk_scene_get(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s() = %d\n", __func__, aw87339_speaker_scene);
	ucontrol->value.integer.value[0] = aw87339_speaker_scene;
	return 0;
}

static int aw87339_spk_scene_set(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: ucontrol = %ld\n", __func__, ucontrol->value.integer.value[0]);

	if(SPEAKER_SCENE_NUM <= ucontrol->value.integer.value[0]) {
		aw87339_speaker_scene = SPEAKER_SCENE_PLAYBACK;
		pr_err("%s(): set speaker scene val = %ld !!! \r\n", __func__, ucontrol->value.integer.value[0]);
	} else {
		aw87339_speaker_scene = ucontrol->value.integer.value[0];
	}

	return 0;
}
extern int aw87339_audio_probe_get(void);
static const char *const aw87339_Probe[] = { "Off", "On" };
static const struct soc_enum aw87339_spk_probe_enum =
        SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(aw87339_Probe), aw87339_Probe);
static int aw87339_spk_probe_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        int probe = aw87339_audio_probe_get();
        pr_debug("%s() = %d\n", __func__, probe);
        ucontrol->value.integer.value[0] = probe;
        return 0;
}
#endif
#ifdef CONFIG_SND_SOC_AW87359
extern unsigned char aw87359_audio_dspk(void);
extern unsigned char aw87359_audio_drcv(void);
extern unsigned char aw87359_audio_abspk(void);
extern unsigned char aw87359_audio_dspk_ftm(void);
extern unsigned char aw87359_audio_off(void);

enum {
	SPEAKER_SCENE_NORMAL = 0,
	SPEAKER_SCENE_FTM,
	SPEAKER_SCENE_N
};

static int aw87359_speaker_scene = SPEAKER_SCENE_NORMAL;
static const char *const aw87359_spk_scene[] = { "Normal", "FTM" };
static const struct soc_enum aw87359_spk_scene_enum =
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(aw87359_spk_scene), aw87359_spk_scene);

static int aw87359_spk_scene_get(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s() = %d\n", __func__, aw87359_speaker_scene);
	ucontrol->value.integer.value[0] = aw87359_speaker_scene;
	return 0;
}

static int aw87359_spk_scene_set(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: ucontrol = %ld\n", __func__, ucontrol->value.integer.value[0]);

	if(SPEAKER_SCENE_N <= ucontrol->value.integer.value[0]) {
		aw87359_speaker_scene = SPEAKER_SCENE_NORMAL;
		pr_err("%s(): set speaker scene val = %ld !!! \n", __func__, ucontrol->value.integer.value[0]);
	} else {
		aw87359_speaker_scene = ucontrol->value.integer.value[0];
	}

	return 0;
}

#endif
#endif /* OPLUS_BUG_STABILITY */

#ifdef OPLUS_BUG_COMPATIBILITY//OPLUS_BUG_COMPATIBILITY
enum oplus_pa_type_def {
	OPLUS_PA_NXP = 0,
	OPLUS_PA_AWINIC,
	OPLUS_PA_SIA,
	OPLUS_PA_AWINIC_DIGITAL,
	OPLUS_PA_TYPE_NUM
};
static int oplus_pa_type = OPLUS_PA_NXP;

//if more config values, set a bigger number
#define AUDIO_EXTERN_CONFIG_MAX_NUM  4
#define OPLUS_PA_TYPE_OFFSET 0
int audio_extern[AUDIO_EXTERN_CONFIG_MAX_NUM] = {0};

static int read_audio_extern_config_dts(struct platform_device *pdev)
{
	int ret;
	int count, i;
	count = of_property_count_u32_elems(pdev->dev.of_node, "audio_extern_config");
	if (count <= 0) {
		dev_err(&pdev->dev, "%s: no property match audio_extern_config\n", __func__);
		return -ENODATA;
	} else if (count > AUDIO_EXTERN_CONFIG_MAX_NUM) {
		dev_err(&pdev->dev, "%s: audio_extern_config num=%d > %d(max numbers)\n",
				__func__, count, AUDIO_EXTERN_CONFIG_MAX_NUM);
		return -EINVAL;
	}

	ret = of_property_read_u32_array(pdev->dev.of_node, "audio_extern_config",
			audio_extern, count);
	if (ret) {
		dev_err(&pdev->dev, "%s: read audio_extern_config error = %d\n", __func__, ret);
		return ret;
	}
	for (i = 0; i < count; i++) {
		dev_info(&pdev->dev, "%s: audio_extern[%d] = %d\n",
				__func__, i ,audio_extern[i]);
	}

	if (OPLUS_PA_TYPE_OFFSET < count) {
		oplus_pa_type = audio_extern[OPLUS_PA_TYPE_OFFSET];
		dev_info(&pdev->dev, "%s: pa_type = audio_extern[%d] = %d\n",
				__func__, OPLUS_PA_TYPE_OFFSET , oplus_pa_type);
	}

	return ret;
}

static int mt6785_audio_extern_config_get(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	int i;

	for (i = 0; i < AUDIO_EXTERN_CONFIG_MAX_NUM; i++) {
		ucontrol->value.integer.value[i] = audio_extern[i];
		pr_info("%s(), OPLUS_AUDIO_EXTERN_CONFIG get value(%d) = %d",
				__func__, i, audio_extern[i]);
	}

	return 0;
}

static int mt6785_audio_extern_config_ctl(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = AUDIO_EXTERN_CONFIG_MAX_NUM;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 0x7fffffff; /* 32 bit value,  */

	return 0;
}
#endif  /*OPLUS_BUG_COMPATIBILITY*/
static int mt6785_spk_type_get(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	int idx = mtk_spk_get_type();

	pr_debug("%s() = %d\n", __func__, idx);
	ucontrol->value.integer.value[0] = idx;
	return 0;
}

static int mt6785_spk_i2s_out_type_get(struct snd_kcontrol *kcontrol,
				       struct snd_ctl_elem_value *ucontrol)
{
	int idx = mtk_spk_get_i2s_out_type();

	pr_debug("%s() = %d\n", __func__, idx);
	ucontrol->value.integer.value[0] = idx;
	return 0;
}

static int mt6785_spk_i2s_in_type_get(struct snd_kcontrol *kcontrol,
				      struct snd_ctl_elem_value *ucontrol)
{
	int idx = mtk_spk_get_i2s_in_type();

	pr_debug("%s() = %d\n", __func__, idx);
	ucontrol->value.integer.value[0] = idx;
	return 0;
}

static int mt6785_mt6359_spk_amp_event(struct snd_soc_dapm_widget *w,
				       struct snd_kcontrol *kcontrol,
				       int event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;

	dev_info(card->dev, "%s(), event %d\n", __func__, event);

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/* spk amp on control */
		#ifdef OPLUS_BUG_STABILITY
		#ifdef CONFIG_SND_SOC_AW87339
		if(aw87339_speaker_scene == SPEAKER_SCENE_PLAYBACK)
			aw87339_audio_kspk();
		else if(aw87339_speaker_scene == SPEAKER_SCENE_VOICE)
			aw87339_audio_voicespk();
		else
			aw87339_audio_kspk();
		#endif

		#ifdef CONFIG_SND_SOC_AW87359
		if(aw87359_speaker_scene == SPEAKER_SCENE_NORMAL)
			aw87359_audio_dspk();
		else if(aw87359_speaker_scene == SPEAKER_SCENE_FTM)
			aw87359_audio_dspk_ftm();
		else
			aw87359_audio_dspk();
		#endif
		#endif /* OPLUS_BUG_STABILITY */
		break;
	case SND_SOC_DAPM_PRE_PMD:
		/* spk amp off control */
		#ifdef OPLUS_BUG_STABILITY
		#ifdef CONFIG_SND_SOC_AW87339
		aw87339_audio_off();
		#endif

		#ifdef CONFIG_SND_SOC_AW87359
		aw87359_audio_off();
		#endif
		#endif /* OPLUS_BUG_STABILITY */
		break;
	default:
		break;
	}

	return 0;
};

#ifdef CONFIG_SND_SOC_MT8185_EVB
static int mt6785_mt6359_headphone_amp_event(struct snd_soc_dapm_widget *w,
				       struct snd_kcontrol *kcontrol,
				       int event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;

	dev_info(card->dev, "%s(), event %d\n", __func__, event);

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/* spk amp on control */
#ifdef CONFIG_SND_SOC_MT8185_EVB
		audio_exthpamp_enable();
#endif
		break;
	case SND_SOC_DAPM_PRE_PMD:
		/* spk amp off control */
#ifdef CONFIG_SND_SOC_MT8185_EVB
		audio_exthpamp_disable();
#endif
		break;
	default:
		break;
	}

	return 0;
};
#endif

static const struct snd_soc_dapm_widget mt6785_mt6359_widgets[] = {
	SND_SOC_DAPM_SPK(EXT_SPK_AMP_W_NAME, mt6785_mt6359_spk_amp_event),
#ifdef CONFIG_SND_SOC_MT8185_EVB
	SND_SOC_DAPM_SPK(EXT_SPK_HP_AMP_W_NAME,
		     mt6785_mt6359_headphone_amp_event),
#endif
};

static const struct snd_soc_dapm_route mt6785_mt6359_routes[] = {
	{EXT_SPK_AMP_W_NAME, NULL, "LINEOUT L"},
	{EXT_SPK_AMP_W_NAME, NULL, "Headphone L Ext Spk Amp"},
	{EXT_SPK_AMP_W_NAME, NULL, "Headphone R Ext Spk Amp"},
#ifdef CONFIG_SND_SOC_MT8185_EVB
	{EXT_SPK_HP_AMP_W_NAME, NULL, "LINEOUT L"},
	{EXT_SPK_HP_AMP_W_NAME, NULL, "Headphone L Ext Spk Amp"},
	{EXT_SPK_HP_AMP_W_NAME, NULL, "Headphone R Ext Spk Amp"},
#endif
};

static const struct snd_kcontrol_new mt6785_mt6359_controls[] = {
	SOC_DAPM_PIN_SWITCH(EXT_SPK_AMP_W_NAME),
#ifdef CONFIG_SND_SOC_MT8185_EVB
	SOC_DAPM_PIN_SWITCH(EXT_SPK_HP_AMP_W_NAME),
#endif
	SOC_ENUM_EXT("MTK_SPK_TYPE_GET", mt6785_spk_type_enum[0],
		     mt6785_spk_type_get, NULL),
	SOC_ENUM_EXT("MTK_SPK_I2S_OUT_TYPE_GET", mt6785_spk_type_enum[1],
		     mt6785_spk_i2s_out_type_get, NULL),
	SOC_ENUM_EXT("MTK_SPK_I2S_IN_TYPE_GET", mt6785_spk_type_enum[1],
		     mt6785_spk_i2s_in_type_get, NULL),
#ifdef OPLUS_BUG_STABILITY
	SOC_ENUM_EXT("AW87339 Spk Scene", aw87339_spk_scene_enum,
			aw87339_spk_scene_get, aw87339_spk_scene_set),
       SOC_ENUM_EXT("AW87359 Spk Scene", aw87359_spk_scene_enum,
			aw87359_spk_scene_get, aw87359_spk_scene_set),
	SOC_ENUM_EXT("AW87339 Spk Probe Get", aw87339_spk_probe_enum,
			aw87339_spk_probe_get, NULL),
#endif /* OPLUS_BUG_STABILITY */
#ifdef OPLUS_BUG_COMPATIBILITY
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "OPLUS_AUDIO_EXTERN_CONFIG",
		.access = SNDRV_CTL_ELEM_ACCESS_READ,
		.info = mt6785_audio_extern_config_ctl,
		.get = mt6785_audio_extern_config_get
	},
#endif //OPLUS_BUG_COMPATIBILITY
};

/*
 * define mtk_spk_i2s_mck node in dts when need mclk,
 * BE i2s need assign snd_soc_ops = mt6785_mt6359_i2s_ops
 */
static int mt6785_mt6359_i2s_hw_params(struct snd_pcm_substream *substream,
				       struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	unsigned int rate = params_rate(params);
#ifdef CONFIG_SND_SOC_CS35L41
	unsigned int mclk_fs_ratio = 256;
#else
	unsigned int mclk_fs_ratio = 128;
#endif
	unsigned int mclk_fs = rate * mclk_fs_ratio;

	return snd_soc_dai_set_sysclk(rtd->cpu_dai,
				      0, mclk_fs, SND_SOC_CLOCK_OUT);
}

static const struct snd_soc_ops mt6785_mt6359_i2s_ops = {
	.hw_params = mt6785_mt6359_i2s_hw_params,
};

static int mt6785_mt6359_mtkaif_calibration(struct snd_soc_pcm_runtime *rtd)
{
	struct mtk_base_afe *afe = snd_soc_platform_get_drvdata(rtd->platform);
	struct mt6785_afe_private *afe_priv = afe->platform_priv;
	int phase;
	unsigned int monitor;
	int test_done_1, test_done_2, test_done_3;
	int cycle_1, cycle_2, cycle_3;
	int prev_cycle_1, prev_cycle_2, prev_cycle_3;
	int counter;
	int mtkaif_calib_ok;

	dev_info(afe->dev, "%s(), start\n", __func__);

	pm_runtime_get_sync(afe->dev);
	mt6785_afe_gpio_request(afe, true, MT6785_DAI_ADDA, 1);
	mt6785_afe_gpio_request(afe, true, MT6785_DAI_ADDA, 0);
	mt6785_afe_gpio_request(afe, true, MT6785_DAI_ADDA_CH34, 1);
	mt6785_afe_gpio_request(afe, true, MT6785_DAI_ADDA_CH34, 0);

	mt6359_mtkaif_calibration_enable(&rtd->codec->component);

	/* set clock protocol 2 */
	regmap_update_bits(afe->regmap, AFE_AUD_PAD_TOP, 0xff, 0x38);
	regmap_update_bits(afe->regmap, AFE_AUD_PAD_TOP, 0xff, 0x39);

	/* set test type to synchronizer pulse */
	set_cksys_reg(CKSYS_AUD_TOP_CFG, 0xffff, 0x4);

	mtkaif_calib_ok = true;
	afe_priv->mtkaif_calibration_num_phase = 42;	/* mt6359: 0 ~ 42 */
	afe_priv->mtkaif_chosen_phase[0] = -1;
	afe_priv->mtkaif_chosen_phase[1] = -1;
	afe_priv->mtkaif_chosen_phase[2] = -1;

	for (phase = 0;
	     phase <= afe_priv->mtkaif_calibration_num_phase &&
	     mtkaif_calib_ok;
	     phase++) {
		mt6359_set_mtkaif_calibration_phase(&rtd->codec->component,
						    phase, phase, phase);

		set_cksys_reg(CKSYS_AUD_TOP_CFG, 0x1, 0x1);

		test_done_1 = 0;
		test_done_2 = 0;
		test_done_3 = 0;
		cycle_1 = -1;
		cycle_2 = -1;
		cycle_3 = -1;
		counter = 0;
		while (test_done_1 == 0 ||
		       test_done_2 == 0 ||
		       test_done_3 == 0) {
			monitor = get_cksys_reg(CKSYS_AUD_TOP_MON);

			test_done_1 = (monitor >> 28) & 0x1;
			test_done_2 = (monitor >> 29) & 0x1;
			test_done_3 = (monitor >> 30) & 0x1;
			if (test_done_1 == 1)
				cycle_1 = monitor & 0xf;

			if (test_done_2 == 1)
				cycle_2 = (monitor >> 4) & 0xf;

			if (test_done_3 == 1)
				cycle_3 = (monitor >> 8) & 0xf;

			/* handle if never test done */
			if (++counter > 10000) {
				dev_err(afe->dev, "%s(), test fail, cycle_1 %d, cycle_2 %d, cycle_3 %d, monitor 0x%x\n",
					__func__,
					cycle_1, cycle_2, cycle_3, monitor);
				mtkaif_calib_ok = false;
				break;
			}
		}

		if (phase == 0) {
			prev_cycle_1 = cycle_1;
			prev_cycle_2 = cycle_2;
			prev_cycle_3 = cycle_3;
		}

		if (cycle_1 != prev_cycle_1 &&
		    afe_priv->mtkaif_chosen_phase[0] < 0) {
			afe_priv->mtkaif_chosen_phase[0] = phase - 1;
			afe_priv->mtkaif_phase_cycle[0] = prev_cycle_1;
		}

		if (cycle_2 != prev_cycle_2 &&
		    afe_priv->mtkaif_chosen_phase[1] < 0) {
			afe_priv->mtkaif_chosen_phase[1] = phase - 1;
			afe_priv->mtkaif_phase_cycle[1] = prev_cycle_2;
		}

		if (cycle_3 != prev_cycle_3 &&
		    afe_priv->mtkaif_chosen_phase[2] < 0) {
			afe_priv->mtkaif_chosen_phase[2] = phase - 1;
			afe_priv->mtkaif_phase_cycle[2] = prev_cycle_3;
		}

		set_cksys_reg(CKSYS_AUD_TOP_CFG, 0x1, 0x0);

		if (afe_priv->mtkaif_chosen_phase[0] >= 0 &&
		    afe_priv->mtkaif_chosen_phase[1] >= 0 &&
		    afe_priv->mtkaif_chosen_phase[2] >= 0)
			break;
	}

	mt6359_set_mtkaif_calibration_phase(&rtd->codec->component,
		(afe_priv->mtkaif_chosen_phase[0] < 0) ?
		0 : afe_priv->mtkaif_chosen_phase[0],
		(afe_priv->mtkaif_chosen_phase[1] < 0) ?
		0 : afe_priv->mtkaif_chosen_phase[1],
		(afe_priv->mtkaif_chosen_phase[2] < 0) ?
		0 : afe_priv->mtkaif_chosen_phase[2]);

	/* disable rx fifo */
	regmap_update_bits(afe->regmap, AFE_AUD_PAD_TOP, 0xff, 0x38);

	mt6359_mtkaif_calibration_disable(&rtd->codec->component);

	mt6785_afe_gpio_request(afe, false, MT6785_DAI_ADDA, 1);
	mt6785_afe_gpio_request(afe, false, MT6785_DAI_ADDA, 0);
	mt6785_afe_gpio_request(afe, false, MT6785_DAI_ADDA_CH34, 1);
	mt6785_afe_gpio_request(afe, false, MT6785_DAI_ADDA_CH34, 0);
	pm_runtime_put(afe->dev);

	dev_info(afe->dev, "%s(), mtkaif_chosen_phase[0/1/2]:%d/%d/%d\n",
		 __func__,
		 afe_priv->mtkaif_chosen_phase[0],
		 afe_priv->mtkaif_chosen_phase[1],
		 afe_priv->mtkaif_chosen_phase[2]);

	return 0;
}

static int mt6785_mt6359_init(struct snd_soc_pcm_runtime *rtd)
{
	struct mt6359_codec_ops ops;
	struct mtk_base_afe *afe = snd_soc_platform_get_drvdata(rtd->platform);
	struct mt6785_afe_private *afe_priv = afe->platform_priv;
	struct snd_soc_dapm_context *dapm = &rtd->card->dapm;

	ops.enable_dc_compensation = mt6785_enable_dc_compensation;
	ops.set_lch_dc_compensation = mt6785_set_lch_dc_compensation;
	ops.set_rch_dc_compensation = mt6785_set_rch_dc_compensation;
	ops.adda_dl_gain_control = mt6785_adda_dl_gain_control;
	mt6359_set_codec_ops(&rtd->codec->component, &ops);

	/* set mtkaif protocol */
	mt6359_set_mtkaif_protocol(&rtd->codec->component,
				   MT6359_MTKAIF_PROTOCOL_2_CLK_P2);
	afe_priv->mtkaif_protocol = MTKAIF_PROTOCOL_2_CLK_P2;

	/* mtkaif calibration */
	if (afe_priv->mtkaif_protocol == MTKAIF_PROTOCOL_2_CLK_P2)
		mt6785_mt6359_mtkaif_calibration(rtd);

	/* disable ext amp connection */
	snd_soc_dapm_disable_pin(dapm, EXT_SPK_AMP_W_NAME);

	return 0;
}

static int mt6785_i2s_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
				      struct snd_pcm_hw_params *params)
{
	dev_info(rtd->dev, "%s(), fix format to 32bit\n", __func__);

	/* fix BE i2s format to 32bit, clean param mask first */
	snd_mask_reset_range(hw_param_mask(params, SNDRV_PCM_HW_PARAM_FORMAT),
			     0, SNDRV_PCM_FORMAT_LAST);

	params_set_format(params, SNDRV_PCM_FORMAT_S32_LE);
	return 0;
}

#ifdef CONFIG_MTK_VOW_SUPPORT
static const struct snd_pcm_hardware mt6785_mt6359_vow_hardware = {
	.info = (SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_INTERLEAVED |
		 SNDRV_PCM_INFO_MMAP_VALID),
	.period_bytes_min = 256,
	.period_bytes_max = 2 * 1024,
	.periods_min = 2,
	.periods_max = 4,
	.buffer_bytes_max = 2 * 2 * 1024,
};

static int mt6785_mt6359_vow_startup(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct mtk_base_afe *afe = snd_soc_platform_get_drvdata(rtd->platform);
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;

	dev_info(afe->dev, "%s(), start\n", __func__);
	snd_soc_set_runtime_hwparams(substream, &mt6785_mt6359_vow_hardware);

	mt6785_afe_gpio_request(afe, true, MT6785_DAI_VOW, 0);

	/* ASoC will call pm_runtime_get, but vow don't need */
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;
		pm_runtime_put_autosuspend(component->dev);
	}

	return 0;
}

static void mt6785_mt6359_vow_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct mtk_base_afe *afe = snd_soc_platform_get_drvdata(rtd->platform);
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;

	dev_info(afe->dev, "%s(), end\n", __func__);
	mt6785_afe_gpio_request(afe, false, MT6785_DAI_VOW, 0);

	/* restore to fool ASoC */
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;
		pm_runtime_get_sync(component->dev);
	}
}

static const struct snd_soc_ops mt6785_mt6359_vow_ops = {
	.startup = mt6785_mt6359_vow_startup,
	.shutdown = mt6785_mt6359_vow_shutdown,
};
#endif  // #ifdef CONFIG_MTK_VOW_SUPPORT

#ifdef CONFIG_SND_SOC_CS35L41
static int cirrus_prince_devs = 4;
static struct snd_soc_codec_conf *mt_prince_codec_conf;

#define CS35L41_REG_ASP_EN        0x4800
#define CS35L41_ASP_TX_MASK       0x03
#define CS35L41_ASP_TX_EN         0x01
#define CS35L41_ASP_TX_DISABLE    0x0

#define CS35L41_ASP_CTL2          0x4808
#define CS35L41_ASP_DATA_CTL1     0x4830

#define CS35L41_ASP_WIDTH_MASK    0xff0000
#define CS35L41_ASP_WIDTH_16BIT   0x100000
#define CS35L41_ASP_TX_WL_MASK    0x3f
#define CS35L41_ASP_TX_WL_16BIT   0x10

static int cs35l41_enable_tx(struct snd_soc_codec *codec, int ch_num)
{
	unsigned int value = 0;

//TODO:   By default, Amp TX is 24bit.
//If need change to 16bit, use the same way to update ASP_WIDTH & ASP_TX_WL reg
#ifdef PRINCE_TX_16BIT
	/* Change to 16Bit data width */
	dev_info(codec->dev, "%s: Set echo ref to 16bit data format\n",
				__func__);
	snd_soc_update_bits(codec, CS35L41_ASP_CTL2, CS35L41_ASP_WIDTH_MASK,
				CS35L41_ASP_WIDTH_16BIT);
	snd_soc_update_bits(codec, CS35L41_ASP_DATA_CTL1, CS35L41_ASP_TX_WL_MASK,
				CS35L41_ASP_TX_WL_16BIT);
#endif
	value = snd_soc_read(codec, CS35L41_REG_ASP_EN);
	dev_info(codec->dev, "%s: before enable_tx reg = 0x%x\n",
				__func__, value);
	snd_soc_update_bits(codec, CS35L41_REG_ASP_EN, CS35L41_ASP_TX_MASK,
				CS35L41_ASP_TX_EN << ch_num);
	value = snd_soc_read(codec, CS35L41_REG_ASP_EN);
	dev_info(codec->dev, "%s: after enable_tx reg = 0x%x\n",
				__func__, value);
	return 0;
}

static int cs35l41_disable_tx(struct snd_soc_codec *codec)
{
	unsigned int value = 0;

	value = snd_soc_read(codec, CS35L41_REG_ASP_EN);

	dev_info(codec->dev, "%s: before disable_tx reg = 0x%x\n",
				__func__, value);
	snd_soc_update_bits(codec, CS35L41_REG_ASP_EN, CS35L41_ASP_TX_MASK, CS35L41_ASP_TX_DISABLE);
	value = snd_soc_read(codec, CS35L41_REG_ASP_EN);
	dev_info(codec->dev, "%s: after disable_tx reg = 0x%x\n",
				__func__, value);
	return 0;
}

static int cs35l41_snd_startup(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *card = rtd->card;
	struct snd_soc_dai **codec_dais = rtd->codec_dais;
	int ret, i;

	for (i = 0; i < rtd->num_codecs; i++) {
		// Set codec_dai as slave
		ret = snd_soc_dai_set_fmt(codec_dais[i],
						SND_SOC_DAIFMT_CBS_CFS | SND_SOC_DAIFMT_I2S);
		if (ret < 0) {
			dev_info(card->dev, "%s: Failed to set fmt codec dai: %d\n",
				__func__, ret);
			return ret;
		}

		ret = snd_soc_codec_set_sysclk(codec_dais[i]->codec, 0, 0,
						1536000*2,
						SND_SOC_CLOCK_IN);
		if (ret < 0) {
			dev_info(card->dev, "%s: Failed to set codec sysclk: %d\n",
					__func__, ret);
			return ret;
		}

		ret = snd_soc_dai_set_sysclk(codec_dais[i], 0,
						1536000*2,
						SND_SOC_CLOCK_IN);
		if (ret < 0) {
			dev_info(card->dev, "%s: Failed to set dai sysclk: %d\n",
					__func__, ret);
			return ret;
		}
		/* Enable Amp TX path */
		if (i < 2) { /* Enable CH0 for SPK1 & SPK2 */
			cs35l41_enable_tx(codec_dais[i]->codec, 0);
		} else {  /* Enable CH1 for SPK1 & SPK2 */
			cs35l41_enable_tx(codec_dais[i]->codec, 1);
		}
	}

	dev_info(card->dev, "-%s\n", __func__);
	return 0;
}

void cs35l41_snd_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *card = rtd->card;
	struct snd_soc_dai **codec_dais = rtd->codec_dais;
	int i;

	for (i = 0; i < rtd->num_codecs; i++) {
		/* Disable Amp TX path */
		cs35l41_disable_tx(codec_dais[i]->codec);
	}

	dev_info(card->dev, "-%s\n", __func__);
}

static struct snd_soc_ops cs35l41_be_ops = {
	.startup = cs35l41_snd_startup,
	.shutdown = cs35l41_snd_shutdown,
};

static int cs35l41_mi2s_snd_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_card *card = rtd->card;
	struct snd_soc_codec *spk1_cdc = rtd->codec_dais[0]->codec;
	struct snd_soc_dapm_context *spk1_dapm = snd_soc_codec_get_dapm(spk1_cdc);
	struct snd_soc_codec *spk2_cdc = rtd->codec_dais[1]->codec;
	struct snd_soc_dapm_context *spk2_dapm = snd_soc_codec_get_dapm(spk2_cdc);
	struct snd_soc_codec *spk3_cdc = rtd->codec_dais[2]->codec;
	struct snd_soc_dapm_context *spk3_dapm = snd_soc_codec_get_dapm(spk3_cdc);
	struct snd_soc_codec *spk4_cdc = rtd->codec_dais[3]->codec;
	struct snd_soc_dapm_context *spk4_dapm = snd_soc_codec_get_dapm(spk4_cdc);


	dev_info(card->dev, "%s: found codec[%s]\n", __func__, dev_name(spk1_cdc->dev));
	snd_soc_dapm_ignore_suspend(spk1_dapm, "SPK1 AMP Playback");
	snd_soc_dapm_ignore_suspend(spk1_dapm, "SPK1 SPK");
	snd_soc_dapm_ignore_suspend(spk1_dapm, "SPK1 VMON ADC");
	snd_soc_dapm_ignore_suspend(spk1_dapm, "SPK1 AMP Capture");
	snd_soc_dapm_sync(spk1_dapm);

	dev_info(card->dev, "%s: found codec[%s]\n", __func__, dev_name(spk2_cdc->dev));
	snd_soc_dapm_ignore_suspend(spk2_dapm, "SPK2 AMP Playback");
	snd_soc_dapm_ignore_suspend(spk2_dapm, "SPK2 SPK");
	snd_soc_dapm_ignore_suspend(spk2_dapm, "SPK2 VMON ADC");
	snd_soc_dapm_ignore_suspend(spk2_dapm, "SPK2 AMP Capture");
	snd_soc_dapm_sync(spk2_dapm);

	dev_info(card->dev, "%s: found codec[%s]\n", __func__, dev_name(spk3_cdc->dev));
	snd_soc_dapm_ignore_suspend(spk3_dapm, "SPK3 AMP Playback");
	snd_soc_dapm_ignore_suspend(spk3_dapm, "SPK3 SPK");
	snd_soc_dapm_ignore_suspend(spk3_dapm, "SPK3 VMON ADC");
	snd_soc_dapm_ignore_suspend(spk3_dapm, "SPK3 AMP Capture");
	snd_soc_dapm_sync(spk3_dapm);

	dev_info(card->dev, "%s: found codec[%s]\n", __func__, dev_name(spk4_cdc->dev));
	snd_soc_dapm_ignore_suspend(spk4_dapm, "SPK4 AMP Playback");
	snd_soc_dapm_ignore_suspend(spk4_dapm, "SPK4 SPK");
	snd_soc_dapm_ignore_suspend(spk4_dapm, "SPK4 VMON ADC");
	snd_soc_dapm_ignore_suspend(spk4_dapm, "SPK4 AMP Capture");
	snd_soc_dapm_sync(spk4_dapm);

	return 0;
}

static struct snd_soc_dai_link_component cirrus_prince[] = {
	{
		.name = "cs35l41.6-0040",
		.dai_name = "cs35l41-pcm",
	},
	{
		.name = "cs35l41.6-0041",
		.dai_name = "cs35l41-pcm",
	},
	{
		.name = "cs35l41.6-0042",
		.dai_name = "cs35l41-pcm",
	},
	{
		.name = "cs35l41.6-0043",
		.dai_name = "cs35l41-pcm",
	},
};


#endif
static struct snd_soc_dai_link mt6785_mt6359_dai_links[] = {
	/* Front End DAI links */
	{
		.name = "Playback_1",
		.stream_name = "Playback_1",
		.cpu_dai_name = "DL1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_12",
		.stream_name = "Playback_12",
		.cpu_dai_name = "DL12",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_2",
		.stream_name = "Playback_2",
		.cpu_dai_name = "DL2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_3",
		.stream_name = "Playback_3",
		.cpu_dai_name = "DL3",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_4",
		.stream_name = "Playback_4",
		.cpu_dai_name = "DL4",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_5",
		.stream_name = "Playback_5",
		.cpu_dai_name = "DL5",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_6",
		.stream_name = "Playback_6",
		.cpu_dai_name = "DL6",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_7",
		.stream_name = "Playback_7",
		.cpu_dai_name = "DL7",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Playback_8",
		.stream_name = "Playback_8",
		.cpu_dai_name = "DL8",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Capture_1",
		.stream_name = "Capture_1",
		.cpu_dai_name = "UL1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_2",
		.stream_name = "Capture_2",
		.cpu_dai_name = "UL2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_3",
		.stream_name = "Capture_3",
		.cpu_dai_name = "UL3",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_4",
		.stream_name = "Capture_4",
		.cpu_dai_name = "UL4",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_5",
		.stream_name = "Capture_5",
		.cpu_dai_name = "UL5",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_6",
		.stream_name = "Capture_6",
		.cpu_dai_name = "UL6",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_7",
		.stream_name = "Capture_7",
		.cpu_dai_name = "UL7",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_8",
		.stream_name = "Capture_8",
		.cpu_dai_name = "UL8",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_Mono_1",
		.stream_name = "Capture_Mono_1",
		.cpu_dai_name = "UL_MONO_1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_Mono_2",
		.stream_name = "Capture_Mono_2",
		.cpu_dai_name = "UL_MONO_2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Capture_Mono_3",
		.stream_name = "Capture_Mono_3",
		.cpu_dai_name = "UL_MONO_3",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "Playback_HDMI",
		.stream_name = "Playback_HDMI",
		.cpu_dai_name = "HDMI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
	},
	{
		.name = "Hostless_LPBK",
		.stream_name = "Hostless_LPBK",
		.cpu_dai_name = "Hostless LPBK DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_FM",
		.stream_name = "Hostless_FM",
		.cpu_dai_name = "Hostless FM DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_Speech",
		.stream_name = "Hostless_Speech",
		.cpu_dai_name = "Hostless Speech DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_Sph_Echo_Ref",
		.stream_name = "Hostless_Sph_Echo_Ref",
		.cpu_dai_name = "Hostless_Sph_Echo_Ref_DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_Spk_Init",
		.stream_name = "Hostless_Spk_Init",
		.cpu_dai_name = "Hostless_Spk_Init_DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_ADDA_DL_I2S_OUT",
		.stream_name = "Hostless_ADDA_DL_I2S_OUT",
		.cpu_dai_name = "Hostless_ADDA_DL_I2S_OUT DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_SRC_1",
		.stream_name = "Hostless_SRC_1",
		.cpu_dai_name = "Hostless_SRC_1_DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_SRC_Bargein",
		.stream_name = "Hostless_SRC_Bargein",
		.cpu_dai_name = "Hostless_SRC_Bargein_DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	/* Back End DAI links */
	{
		.name = "Primary Codec",
		.cpu_dai_name = "ADDA",
		.codec_dai_name = "mt6359-snd-codec-aif1",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		.init = mt6785_mt6359_init,
	},
	{
		.name = "Primary Codec CH34",
		.cpu_dai_name = "ADDA_CH34",
		.codec_dai_name = "mt6359-snd-codec-aif2",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "AP_DMIC",
		.cpu_dai_name = "AP_DMIC",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "AP_DMIC_CH34",
		.cpu_dai_name = "AP_DMIC_CH34",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "I2S3",
		.cpu_dai_name = "I2S3",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6785_i2s_hw_params_fixup,
	},
	{
		.name = "I2S0",
		.cpu_dai_name = "I2S0",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6785_i2s_hw_params_fixup,
	},
#if defined(CONFIG_SND_SOC_CS35L41)
	{
		.name = "I2S1",
		.cpu_dai_name = "I2S1",
		.codecs = cirrus_prince,
		.num_codecs = ARRAY_SIZE(cirrus_prince),
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ops = &cs35l41_be_ops,
		.init = cs35l41_mi2s_snd_init,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = mt6785_i2s_hw_params_fixup,
	},
#else
	{
		.name = "I2S1",
		.cpu_dai_name = "I2S1",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6785_i2s_hw_params_fixup,
	},
#endif
	{
		.name = "I2S2",
		.cpu_dai_name = "I2S2",
#ifdef CONFIG_SND_SOC_AS33970
		.codec_dai_name = "cx33970-aif",
		.codec_name = "cx33970.9-0041",
		.ops = &mt6785_mt6359_i2s_ops,
#else
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
#endif
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6785_i2s_hw_params_fixup,
	},
	{
		.name = "I2S5",
		.cpu_dai_name = "I2S5",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6785_i2s_hw_params_fixup,
	},
	{
		.name = "HW Gain 1",
		.cpu_dai_name = "HW Gain 1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "HW Gain 2",
		.cpu_dai_name = "HW Gain 2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "HW_SRC_1",
		.cpu_dai_name = "HW_SRC_1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "HW_SRC_2",
		.cpu_dai_name = "HW_SRC_2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "CONNSYS_I2S",
		.cpu_dai_name = "CONNSYS_I2S",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "PCM 1",
		.cpu_dai_name = "PCM 1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "PCM 2",
		.cpu_dai_name = "PCM 2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "TDM",
		.cpu_dai_name = "TDM",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
	},
	/* dummy BE for ul memif to record from dl memif */
	{
		.name = "Hostless_UL1",
		.cpu_dai_name = "Hostless_UL1 DAI",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_UL2",
		.cpu_dai_name = "Hostless_UL2 DAI",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_UL3",
		.cpu_dai_name = "Hostless_UL3 DAI",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_UL6",
		.cpu_dai_name = "Hostless_UL6 DAI",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_DSP_DL",
		.cpu_dai_name = "Hostless_DSP_DL DAI",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_HW_Gain_AAudio",
		.stream_name = "Hostless_HW_Gain_AAudio",
		.cpu_dai_name = "Hostless HW Gain AAudio DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	{
		.name = "Hostless_SRC_AAudio",
		.stream_name = "Hostless_SRC_AAudio",
		.cpu_dai_name = "Hostless SRC AAudio DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
	},
	/* BTCVSD */
#ifdef CONFIG_SND_SOC_MTK_BTCVSD
	{
		.name = "BTCVSD",
		.stream_name = "BTCVSD",
		.cpu_dai_name   = "snd-soc-dummy-dai",
		.platform_name  = "18050000.mtk-btcvsd-snd",
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
#endif
#if defined(CONFIG_SND_SOC_MTK_AUDIO_DSP)
#if defined(CONFIG_MTK_AUDIO_TUNNELING_SUPPORT)
	{
		.name = "Offload_Playback",
		.stream_name = "Offload_Playback",
		.cpu_dai_name = "audio_task_offload_dai",
		.platform_name = "mt_soc_offload_common",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
#endif
	{
		.name = "DSP_Playback_Voip",
		.stream_name = "DSP_Playback_Voip",
		.cpu_dai_name = "audio_task_voip_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Playback_Primary",
		.stream_name = "DSP_Playback_Primary",
		.cpu_dai_name = "audio_task_primary_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Playback_DeepBuf",
		.stream_name = "DSP_Playback_DeepBuf",
		.cpu_dai_name = "audio_task_deepbuf_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Playback_Playback",
		.stream_name = "DSP_Playback_Playback",
		.cpu_dai_name = "audio_task_Playback_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Capture_Ul1",
		.stream_name = "DSP_Capture_Ul1",
		.cpu_dai_name = "audio_task_capture_ul1_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
/*
	{
		.name = "DSP_Playback_A2DP",
		.stream_name = "DSP_Playback_A2DP",
		.cpu_dai_name = "audio_task_A2DP_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Playback_DataProvider",
		.stream_name = "DSP_Playback_DataProvider",
		.cpu_dai_name = "audio_task_dataprovider_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
*/
	{
		.name = "DSP_Call_Final",
		.stream_name = "DSP_Call_Final",
		.cpu_dai_name = "audio_task_call_final_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Playback_Ktv",
		.stream_name = "DSP_Playback_Ktv",
		.cpu_dai_name = "audio_task_ktv_dai",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
	{
		.name = "DSP_Capture_Raw",
		.stream_name = "DSP_Capture_Raw",
		.cpu_dai_name = "audio_task_capture_raw_dai",
		.platform_name = "snd_audio_dsp",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
#endif
#ifdef CONFIG_MTK_VOW_SUPPORT
	{
		.name = "VOW_Capture",
		.stream_name = "VOW_Capture",
		.cpu_dai_name = "snd-soc-dummy-dai",
		.codec_dai_name = "mt6359-snd-codec-vow",
		.ignore_suspend = 1,
		.ops = &mt6785_mt6359_vow_ops,
	},
#endif  // #ifdef CONFIG_MTK_VOW_SUPPORT
#if defined(CONFIG_SND_SOC_MTK_SCP_SMARTPA)
	{
		.name = "SCP_SPK_Playback",
		.stream_name = "SCP_SPK_Playback",
		.cpu_dai_name = "snd-soc-dummy-dai",
		.platform_name = "snd_scp_spk",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
#endif
#if defined(CONFIG_MTK_ULTRASND_PROXIMITY)
	{
		.name = "SCP_ULTRA_Playback",
		.stream_name = "SCP_ULTRA_Playback",
		.cpu_dai_name = "snd-soc-dummy-dai",
		.platform_name = "snd_scp_ultra",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
	},
#endif

};

static struct snd_soc_card mt6785_mt6359_soc_card = {
	.name = "mt6785-mt6359",
	.owner = THIS_MODULE,
	.dai_link = mt6785_mt6359_dai_links,
	.num_links = ARRAY_SIZE(mt6785_mt6359_dai_links),

	.controls = mt6785_mt6359_controls,
	.num_controls = ARRAY_SIZE(mt6785_mt6359_controls),
	.dapm_widgets = mt6785_mt6359_widgets,
	.num_dapm_widgets = ARRAY_SIZE(mt6785_mt6359_widgets),
	.dapm_routes = mt6785_mt6359_routes,
	.num_dapm_routes = ARRAY_SIZE(mt6785_mt6359_routes),
};

static int mt6785_mt6359_dev_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &mt6785_mt6359_soc_card;
	struct device_node *platform_node, *codec_node, *spk_node, *dsp_node;
	struct snd_soc_dai_link *spk_out_dai_link, *spk_iv_dai_link;
	int ret, i;
	int spk_out_dai_link_idx, spk_iv_dai_link_idx;
	const char *name;
#ifdef CONFIG_SND_SOC_CS35L41
	struct device_node *prince_codec_of_node;
	const char *prince_name_prefix[1];
#endif

#ifdef OPLUS_BUG_COMPATIBILITY//OPLUS_BUG_COMPATIBILITY
	read_audio_extern_config_dts(pdev);
#endif  /*OPLUS_BUG_COMPATIBILITY*/

	ret = mtk_spk_update_info(card, pdev,
				  &spk_out_dai_link_idx, &spk_iv_dai_link_idx,
				  &mt6785_mt6359_i2s_ops);
	if (ret) {
		dev_err(&pdev->dev, "%s(), mtk_spk_update_info error\n",
			__func__);
		return -EINVAL;
	}

	spk_out_dai_link = &mt6785_mt6359_dai_links[spk_out_dai_link_idx];
	spk_iv_dai_link = &mt6785_mt6359_dai_links[spk_iv_dai_link_idx];
	if (!spk_out_dai_link->codec_dai_name &&
	    !spk_iv_dai_link->codec_dai_name) {
		spk_node = of_get_child_by_name(pdev->dev.of_node,
					"mediatek,speaker-codec");
		if (!spk_node) {
			dev_err(&pdev->dev,
				"spk_codec of_get_child_by_name fail\n");
			return -EINVAL;
		}
		ret = snd_soc_of_get_dai_link_codecs(
				&pdev->dev, spk_node, spk_out_dai_link);
		if (ret < 0) {
			dev_err(&pdev->dev,
				"i2s out get_dai_link_codecs fail\n");
			return -EINVAL;
		}
		ret = snd_soc_of_get_dai_link_codecs(
				&pdev->dev, spk_node, spk_iv_dai_link);
		if (ret < 0) {
			dev_err(&pdev->dev,
				"i2s in get_dai_link_codecs fail\n");
			return -EINVAL;
		}
	}

	platform_node = of_parse_phandle(pdev->dev.of_node,
					 "mediatek,platform", 0);
	if (!platform_node) {
		dev_err(&pdev->dev, "Property 'platform' missing or invalid\n");
		return -EINVAL;
	}

	dsp_node = of_parse_phandle(pdev->dev.of_node,
				    "mediatek,snd_audio_dsp", 0);
	if (!dsp_node)
		dev_info(&pdev->dev, "Property 'snd_audio_dsp' missing or invalid\n");

#ifdef OPLUS_ARCH_EXTENDS
    extend_codec_i2s_be_dailinks(mt6785_mt6359_dai_links, ARRAY_SIZE(mt6785_mt6359_dai_links));
#endif /* OPLUS_ARCH_EXTENDS */

	for (i = 0; i < card->num_links; i++) {
		if (mt6785_mt6359_dai_links[i].platform_name)
			continue;
		/* no platform assign and with dsp playback node. */
		name = mt6785_mt6359_dai_links[i].name;
		if (!strncmp(name, "DSP", strlen("DSP")) &&
		    mt6785_mt6359_dai_links[i].platform_name == NULL) {
			mt6785_mt6359_dai_links[i].platform_of_node = dsp_node;
			continue;
		}
		mt6785_mt6359_dai_links[i].platform_of_node = platform_node;
	}

	codec_node = of_parse_phandle(pdev->dev.of_node,
				      "mediatek,audio-codec", 0);
	if (!codec_node) {
		dev_err(&pdev->dev,
			"Property 'audio-codec' missing or invalid\n");
		return -EINVAL;
	}
	for (i = 0; i < card->num_links; i++) {
		if (mt6785_mt6359_dai_links[i].codec_name ||
		    i == spk_out_dai_link_idx ||
		    i == spk_iv_dai_link_idx)
			continue;
#ifdef OPLUS_ARCH_EXTENDS
		if (extend_codec_i2s_compare(mt6785_mt6359_dai_links, i))
			continue;
#endif /* OPLUS_ARCH_EXTENDS */
		mt6785_mt6359_dai_links[i].codec_of_node = codec_node;
	}
#ifdef CONFIG_SND_SOC_MT8185_EVB
		audio_exthpamp_setup_gpio(pdev);
#endif

	card->dev = &pdev->dev;
#ifdef CONFIG_SND_SOC_CS35L41
	/* Alloc prince array of codec conf struct */
	dev_info(&pdev->dev,
				"%s: Default card num_configs = %d\n", __func__, card->num_configs);
	mt_prince_codec_conf = devm_kcalloc(&pdev->dev,
		card->num_configs + cirrus_prince_devs,
		sizeof(struct snd_soc_codec_conf), GFP_KERNEL);
	if (!mt_prince_codec_conf) {
		ret = -ENOMEM;
		return ret;
	}

	for (i = 0; i < cirrus_prince_devs; i++) {
		prince_codec_of_node = of_parse_phandle(pdev->dev.of_node,
			"cirrus,prince-devs", i);
		ret = of_property_read_string_index(pdev->dev.of_node,
			"cirrus,prince-dev-prefix", i, prince_name_prefix);
		if (ret) {
			dev_info(&pdev->dev,
				"%s: failed to read prince dev prefix, ret = %d\n", __func__, ret);
				ret = -EINVAL;
				return ret;
		}
		dev_info(&pdev->dev,
			"%s: prince_dev prefix[%d] = %s\n", __func__, i, prince_name_prefix);

		mt_prince_codec_conf[card->num_configs + i].dev_name = NULL;
		mt_prince_codec_conf[card->num_configs + i].of_node = prince_codec_of_node;
		mt_prince_codec_conf[card->num_configs + i].name_prefix = prince_name_prefix[0];
	}

	card->num_configs += cirrus_prince_devs;
	card->codec_conf = mt_prince_codec_conf;

#endif
	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret)
		dev_err(&pdev->dev, "%s snd_soc_register_card fail %d\n",
			__func__, ret);
	return ret;
}

#ifdef CONFIG_OF
static const struct of_device_id mt6785_mt6359_dt_match[] = {
	{.compatible = "mediatek,mt6785-mt6359-sound",},
	{}
};
#endif

static const struct dev_pm_ops mt6785_mt6359_pm_ops = {
	.poweroff = snd_soc_poweroff,
	.restore = snd_soc_resume,
};

static struct platform_driver mt6785_mt6359_driver = {
	.driver = {
		.name = "mt6785-mt6359",
#ifdef CONFIG_OF
		.of_match_table = mt6785_mt6359_dt_match,
#endif
		.pm = &mt6785_mt6359_pm_ops,
	},
	.probe = mt6785_mt6359_dev_probe,
};

module_platform_driver(mt6785_mt6359_driver);

/* Module information */
MODULE_DESCRIPTION("MT6785 MT6359 ALSA SoC machine driver");
MODULE_AUTHOR("KaiChieh Chuang <kaichieh.chuang@mediatek.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("mt6785 mt6359 soc card");
