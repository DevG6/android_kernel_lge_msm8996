#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#define NEBULA_DEFINE_HELPER_FUNC(__param, __func) \
	static unsigned int __func##__value; \
	static int __init nebula_##__func##_init(char *str) \
	{ \
		int ret = kstrtouint(str, 0, &__func##__value); \
		pr_info("NEBULA_debug_flags " __param " %d: %08x from %26s\r\n", \
				ret, __func##__value, str); \
		return ret; \
	} \
	early_param(__param, nebula_##__func##_init); \
	unsigned int __func(void) \
	{ \
		/* FIXME: timing issue */ \
		return __func##__value; \
	} \
	EXPORT_SYMBOL(__func); \

NEBULA_DEFINE_HELPER_FUNC("td.sf",        get_tamper_sf);
NEBULA_DEFINE_HELPER_FUNC("debugflag",    get_debug_flag);
NEBULA_DEFINE_HELPER_FUNC("kernelflag",   get_kernel_flag);
NEBULA_DEFINE_HELPER_FUNC("radioflag",    get_radio_flag);
NEBULA_DEFINE_HELPER_FUNC("radioflagex1", get_radio_ex1_flag);
NEBULA_DEFINE_HELPER_FUNC("radioflagex2", get_radio_ex2_flag);
NEBULA_DEFINE_HELPER_FUNC("cpumask", get_cpumask_flag);

static char bootmode[64] = "";
static int __init nebula_parse_android_bootmode(char *str)
{
	snprintf(bootmode, sizeof(bootmode), str);
	return 0;
}early_param("androidboot.mode", nebula_parse_android_bootmode);

char *nebula_get_bootmode(void)
{
	return bootmode;
}
EXPORT_SYMBOL(nebula_get_bootmode);
