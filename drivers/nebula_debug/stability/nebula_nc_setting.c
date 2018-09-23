#include <linux/delay.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#undef pr_fmt
#define pr_fmt(fmt) "[NC] " fmt

static int nebula_nc_probe(struct platform_device *pdev)
{
	int ret;
	struct pinctrl *nc_pinctrl;
	struct pinctrl_state *set_state;

	pr_info("== NC_probe ==\n");

	// Init NC wake_host_pin
	nc_pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(nc_pinctrl)) {
		if (PTR_ERR(nc_pinctrl) == -EPROBE_DEFER)
			pr_err("NC_pinctrl EPROBE_DEFER !!\n");

		return PTR_ERR(nc_pinctrl);
	}

	pr_info("Init GPIO pins\n");

	set_state = pinctrl_lookup_state(nc_pinctrl, PINCTRL_STATE_DEFAULT);
	if (IS_ERR(set_state)) {
		pr_err("Lookup NC pinctrl gpio state error\n");
		return -ENODEV;
	}
	ret = pinctrl_select_state(nc_pinctrl, set_state);
	if (ret) {
		pr_err("Set NC pinctrl gpio state error :%d\n", ret);
		return -EINVAL;
	}
	return 0;
}

static const struct of_device_id nebula_nc_match_table[] = {
	{ .compatible = "nebula,nc_pin", },
	{},
};

static struct platform_driver nebula_nc_driver = {
	.probe = nebula_nc_probe,
	.driver = {
		.name = "nc_pin",
		.owner = THIS_MODULE,
		.of_match_table = nebula_nc_match_table,
	},
};

static int __init nebula_nc_init(void)
{
	return platform_driver_register(&nebula_nc_driver);
}

static void __exit nebula_nc_exit(void)
{
	platform_driver_unregister(&nebula_nc_driver);
}

module_init(nebula_nc_init);
module_exit(nebula_nc_exit);
MODULE_LICENSE("GPL");
