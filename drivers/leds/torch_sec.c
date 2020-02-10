#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include <linux/file.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/leds.h>
#include <linux/mfd/sm5701_core.h>
#include <linux/leds/torch_sec.h>

/*
 * This flag is used by camera
 * driver to check the state of flash led extern definition in
 * Dcam_v4l2.c (drivers\media\sprd_dcam\common
 *
 * Currently all sensors are not enabled so commenting this code. When all
 * sensors will be enabled need to uncomment the below code
 */
extern uint32_t flash_torch_status;
//uint32_t flash_torch_status;

#define to_sprd_led(led_cdev) \
	container_of(led_cdev, struct torch_sec, cdev)

/*Torch Sec */
struct torch_sec {
	struct platform_device *dev;
	struct work_struct work;
	enum led_brightness value;
	struct led_classdev cdev;
	int enabled;
};

static struct torch_sec_data pdata;

void torch_sec_register_flash_led_data(struct torch_sec_data data)
{
	pdata.torch_enable = data.torch_enable;
	pdata.torch_disable = data.torch_disable;
}

static void torch_sec_enable(struct torch_sec *led)
{
	if (pdata.torch_enable) {
		pr_info("%s, brightness lvl = %d\n",
			__func__, led->value);
		pdata.torch_enable(led->value);
		led->enabled = 1;
		/*
		 * This flag indicates that the flash led is on/off so it should be
		 * handled in other drivers accordingly
		 */
		flash_torch_status = 1;

	} else {
		pr_err("%s, Flash LED driver is not ready\n",
			__func__);
	}
}

static void torch_sec_disable(struct torch_sec *led)
{
	if (pdata.torch_disable) {
		pr_info("torch_sec_disable\n");
		pdata.torch_disable(pdata.default_brightness);
		led->enabled = 0;
		flash_torch_status = 0;
	} else {
		pr_err("%s, Flash LED driver is not ready\n",
			__func__);
	}
}

static void torch_sec_work(struct work_struct *work)
{
	struct torch_sec *led = container_of(work, struct torch_sec, work);

	if (led == NULL) {
		pr_err("%s, led contailer is null\n",
			__func__);
		return;
	}

	if (led->value == LED_OFF)
		torch_sec_disable(led);
	else
		torch_sec_enable(led);
}

static void torch_sec_set(struct led_classdev *led_cdev,
			   enum led_brightness value)
{
	struct torch_sec *led = to_sprd_led(led_cdev);
	led->value = value;
	schedule_work(&led->work);
}

static void torch_sec_shutdown(struct platform_device *dev)
{
	struct torch_sec *led = platform_get_drvdata(dev);
	torch_sec_disable(led);
}

#ifdef CONFIG_OF
static int torch_sec_parse_dt(struct device *dev,
		struct torch_sec_data *pdata)
{
	struct device_node *np = dev->of_node;
	int ret;
	unsigned int i;
	u32 temp;


	if (!np) {
		pr_info("%s: np NULL\n", __func__);
		return 1;
	}

	ret = of_property_read_u32(np, "brightness_level",
		&pdata->brightness_level);
	if (ret) {
		pr_err("%s, torch brightness level is empty = %d\n",
			__func__, pdata->brightness_level);
		pdata->brightness_level = 1;
	}

	ret = of_property_read_u32(np, "max_brightness",
			&pdata->max_brightness);

	if (ret) {
		pr_err("%s, torch max brightness is empty = %d\n",
			__func__, pdata->max_brightness);
		pdata->max_brightness = 0;
	}

	ret = of_property_read_u32(np, "default_brightness",
			&pdata->default_brightness);

	if (ret) {
		pr_err("%s, torch max brightness is empty = %d\n",
			__func__, pdata->default_brightness);
		pdata->default_brightness = 0;
	}

	for (i = 0; i < pdata->brightness_level; i++) {
		ret = of_property_read_u32_index(np,
				"brightness_current", i, &temp);
		if (ret) {
			pr_err("%s : brightness current table is Empty\n",
				__func__);
			temp = 0;
		}
		pdata->brightness_current[i] = (int)temp;
	}

	return 0;
}
#endif

static int torch_sec_probe(struct platform_device *dev)
{
	struct torch_sec *led;
	int ret;

	led = kzalloc(sizeof(struct torch_sec), GFP_KERNEL);
	if (led == NULL) {
		dev_err(&dev->dev, "No memory for device\n");
		return -ENOMEM;
	}

	if (dev->dev.of_node) {
#ifdef CONFIG_OF
		if (torch_sec_parse_dt(&dev->dev, &pdata))
			dev_err(&dev->dev,
				"%s: Failed to get torch data\n", __func__);
#endif
	} else {
	//	torch_data = dev_get_platdata(&dev->dev);
	}

	led->cdev.brightness_set = torch_sec_set;
	led->cdev.default_trigger = "none";
	led->cdev.name = "torch-sec1";
	led->cdev.max_brightness = pdata.max_brightness;
	led->cdev.brightness_get = NULL;
	led->enabled = 0;

	INIT_WORK(&led->work, torch_sec_work);
	led->value = LED_OFF;
	platform_set_drvdata(dev, led);

	/* register our new led device */

	ret = led_classdev_register(&dev->dev, &led->cdev);
	if (ret < 0) {
		dev_err(&dev->dev, "led_classdev_register failed\n");
		kfree(led);
		return ret;
	}

	torch_sec_disable(led);/*disabled by default*/
	pr_info("%s Probe Done!!!", __func__);
	return 0;
}

static int torch_sec_remove(struct platform_device *dev)
{
	struct torch_sec *led = platform_get_drvdata(dev);

	led_classdev_unregister(&led->cdev);
	flush_scheduled_work();
	led->value = LED_OFF;
	led->enabled = 1;
	torch_sec_disable(led);
	kfree(led);
	return 0;
}

static struct of_device_id torch_sec_match_table[] = {
	{ .compatible = "sm,torch-sec1",},
	{},
};

static const struct platform_device_id torch_sec_id[] = {
	{"torch-sec1", 0},
	{}
};

static struct platform_driver torch_sec_driver = {
	.driver = {
		.name  = "torch-sec1",
		.owner = THIS_MODULE,
		.of_match_table = torch_sec_match_table,
	},
	.probe    = torch_sec_probe,
	.remove   = torch_sec_remove,
	.shutdown = torch_sec_shutdown,
	.id_table = torch_sec_id,
};

static int __init torch_sec_init(void)
{
	return platform_driver_register(&torch_sec_driver);
}

static void __exit torch_sec_exit(void)
{
	platform_driver_unregister(&torch_sec_driver);
}

module_init(torch_sec_init);
module_exit(torch_sec_exit);

MODULE_AUTHOR("Diwas Kumar <diwas.kumar@samsung.com>");
MODULE_DESCRIPTION("Torch Sec Driver");
MODULE_LICENSE("GPL");

