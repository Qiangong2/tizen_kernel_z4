
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/regmap.h>
#include <linux/workqueue.h>
#include <linux/platform_data/leds-ktd2668.h>
#include <linux/of_gpio.h>

/* registers definitions */
#define REG_ENABLE		0x01
#define REG_FLASH_LED0_BR	0x03
#define REG_FLASH_LED1_BR	0x04
#define REG_TORCH_LED0_BR	0x05
#define REG_TORCH_LED1_BR	0x06
#define REG_FLASH_TOUT		0x08
#define REG_FLAG0		0x0a
#define REG_FLAG1		0x0b

enum ktd2668_devid {
	ID_FLASH0 = 0x0,
	ID_FLASH1,
	ID_TORCH0,
	ID_TORCH1,
	ID_MAX
};

enum ktd2668_mode {
	MODE_STDBY = 0x0,
	MODE_IR,
	MODE_TORCH,
	MODE_FLASH,
	MODE_MAX
};

enum ktd2668_devfile {
	DFILE_FLASH0_ENABLE = 0,
	DFILE_FLASH0_ONOFF,
	DFILE_FLASH0_SOURCE,
	DFILE_FLASH0_TIMEOUT,
	DFILE_FLASH1_ENABLE,
	DFILE_FLASH1_ONOFF,
	DFILE_TORCH0_ENABLE,
	DFILE_TORCH0_ONOFF,
	DFILE_TORCH0_SOURCE,
	DFILE_TORCH1_ENABLE,
	DFILE_TORCH1_ONOFF,
	DFILE_MAX
};

#define to_ktd2668(_ctrl, _no) container_of(_ctrl, struct ktd2668, cdev[_no])

struct ktd2668 {
	struct device *dev;

	u8 brightness[ID_MAX];
	struct work_struct work[ID_MAX];
	struct led_classdev cdev[ID_MAX];

	int flash_gpio_en;
	struct regmap *regmap;
	struct mutex lock;
};

struct ktd2668 *pchip_dev = NULL;

static void ktd2668_read_flag(struct ktd2668 *pchip)
{

	int rval;
	unsigned int flag0, flag1;

	rval = regmap_read(pchip->regmap, REG_FLAG0, &flag0);
	rval |= regmap_read(pchip->regmap, REG_FLAG1, &flag1);

	if (rval < 0)
		dev_err(pchip->dev, "i2c access fail.\n");

	dev_info(pchip->dev, "[flag1] 0x%x, [flag0] 0x%x\n",
		 flag1 & 0x1f, flag0);
}
/* torch0 brightness control */
static void ktd2668_deferred_torch0_brightness_set(struct work_struct *work)
{
	struct ktd2668 *pchip = container_of(work,
					    struct ktd2668, work[ID_TORCH0]);

	if (regmap_update_bits(pchip->regmap,
			       REG_TORCH_LED0_BR, 0x7f,
			       pchip->brightness[ID_TORCH0]))
		dev_err(pchip->dev, "i2c access fail.\n");
	ktd2668_read_flag(pchip);
}

static void ktd2668_torch0_brightness_set(struct led_classdev *cdev,
					 enum led_brightness brightness)
{
	struct ktd2668 *pchip =
	    container_of(cdev, struct ktd2668, cdev[ID_TORCH0]);

	pchip->brightness[ID_TORCH0] = brightness;
	schedule_work(&pchip->work[ID_TORCH0]);
}

/* torch1 brightness control */
static void ktd2668_deferred_torch1_brightness_set(struct work_struct *work)
{
	struct ktd2668 *pchip = container_of(work,
					    struct ktd2668, work[ID_TORCH1]);

	if (regmap_update_bits(pchip->regmap,
			       REG_TORCH_LED1_BR, 0x7f,
			       pchip->brightness[ID_TORCH1]))
		dev_err(pchip->dev, "i2c access fail.\n");
	ktd2668_read_flag(pchip);
}

static void ktd2668_torch1_brightness_set(struct led_classdev *cdev,
					 enum led_brightness brightness)
{
	struct ktd2668 *pchip =
	    container_of(cdev, struct ktd2668, cdev[ID_TORCH1]);

	pchip->brightness[ID_TORCH1] = brightness;
	schedule_work(&pchip->work[ID_TORCH1]);
}

/* flash0 brightness control */
static void ktd2668_deferred_flash0_brightness_set(struct work_struct *work)
{
	struct ktd2668 *pchip = container_of(work,
					    struct ktd2668, work[ID_FLASH0]);

	if (regmap_update_bits(pchip->regmap,
			       REG_FLASH_LED0_BR, 0x7f,
			       pchip->brightness[ID_FLASH0]))
		dev_err(pchip->dev, "i2c access fail.\n");
	ktd2668_read_flag(pchip);
}

static void ktd2668_flash0_brightness_set(struct led_classdev *cdev,
					 enum led_brightness brightness)
{
	struct ktd2668 *pchip =
	    container_of(cdev, struct ktd2668, cdev[ID_FLASH0]);

	pchip->brightness[ID_FLASH0] = brightness;
	schedule_work(&pchip->work[ID_FLASH0]);
}

/* flash1 brightness control */
static void ktd2668_deferred_flash1_brightness_set(struct work_struct *work)
{
	struct ktd2668 *pchip = container_of(work,
					    struct ktd2668, work[ID_FLASH1]);

	if (regmap_update_bits(pchip->regmap,
			       REG_FLASH_LED1_BR, 0x7f,
			       pchip->brightness[ID_FLASH1]))
		dev_err(pchip->dev, "i2c access fail.\n");
	ktd2668_read_flag(pchip);
}

static void ktd2668_flash1_brightness_set(struct led_classdev *cdev,
					 enum led_brightness brightness)
{
	struct ktd2668 *pchip =
	    container_of(cdev, struct ktd2668, cdev[ID_FLASH1]);

	pchip->brightness[ID_FLASH1] = brightness;
	schedule_work(&pchip->work[ID_FLASH1]);
}

struct ktd2668_devices {
	struct led_classdev cdev;
	work_func_t func;
};

static struct ktd2668_devices ktd2668_leds[ID_MAX] = {
	[ID_FLASH0] = {
		       .cdev.name = "flash0",
		       .cdev.brightness = 0,
		       .cdev.max_brightness = 0x7f,
		       .cdev.brightness_set = ktd2668_flash0_brightness_set,
		       .cdev.default_trigger = "flash0",
		       .func = ktd2668_deferred_flash0_brightness_set},
	[ID_FLASH1] = {
		       .cdev.name = "flash1",
		       .cdev.brightness = 0,
		       .cdev.max_brightness = 0x7f,
		       .cdev.brightness_set = ktd2668_flash1_brightness_set,
		       .cdev.default_trigger = "flash1",
		       .func = ktd2668_deferred_flash1_brightness_set},
	[ID_TORCH0] = {
		       .cdev.name = "torch0",
		       .cdev.brightness = 0,
		       .cdev.max_brightness = 0x7f,
		       .cdev.brightness_set = ktd2668_torch0_brightness_set,
		       .cdev.default_trigger = "torch0",
		       .func = ktd2668_deferred_torch0_brightness_set},
	[ID_TORCH1] = {
		       .cdev.name = "torch1",
		       .cdev.brightness = 0,
		       .cdev.max_brightness = 0x7f,
		       .cdev.brightness_set = ktd2668_torch1_brightness_set,
		       .cdev.default_trigger = "torch1",
		       .func = ktd2668_deferred_torch1_brightness_set},
};

static void ktd2668_led_unregister(struct ktd2668 *pchip, enum ktd2668_devid id)
{
	int icnt;

	for (icnt = id; icnt > 0; icnt--)
		led_classdev_unregister(&pchip->cdev[icnt - 1]);
}

static int ktd2668_led_register(struct ktd2668 *pchip)
{
	int icnt, rval;

	for (icnt = 0; icnt < ID_MAX; icnt++) {
		INIT_WORK(&pchip->work[icnt], ktd2668_leds[icnt].func);
		pchip->cdev[icnt].name = ktd2668_leds[icnt].cdev.name;
		pchip->cdev[icnt].max_brightness =
		    ktd2668_leds[icnt].cdev.max_brightness;
		pchip->cdev[icnt].brightness =
		    ktd2668_leds[icnt].cdev.brightness;
		pchip->cdev[icnt].brightness_set =
		    ktd2668_leds[icnt].cdev.brightness_set;
		pchip->cdev[icnt].default_trigger =
		    ktd2668_leds[icnt].cdev.default_trigger;
		rval = led_classdev_register((struct device *)
					     pchip->dev, &pchip->cdev[icnt]);
		if (rval < 0) {
			ktd2668_led_unregister(pchip, icnt);
			return rval;
		}
	}
	return 0;
}

/* device files to control registers */
struct ktd2668_commands {
	char *str;
	int size;
};

enum ktd2668_cmd_id {
	CMD_ENABLE = 0,
	CMD_DISABLE,
	CMD_ON,
	CMD_OFF,
	CMD_IRMODE,
	CMD_OVERRIDE,
	CMD_MAX
};

struct ktd2668_commands cmds[CMD_MAX] = {
	[CMD_ENABLE] = {"enable", 6},
	[CMD_DISABLE] = {"disable", 7},
	[CMD_ON] = {"on", 2},
	[CMD_OFF] = {"off", 3},
	[CMD_IRMODE] = {"irmode", 6},
	[CMD_OVERRIDE] = {"override", 8},
};

struct ktd2668_files {
	enum ktd2668_devid id;
	struct device_attribute attr;
};

static size_t ktd2668_ctrl(struct device *dev,
			  const char *buf, enum ktd2668_devid id,
			  enum ktd2668_devfile dfid, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct ktd2668 *pchip = to_ktd2668(led_cdev, id);
	enum ktd2668_cmd_id icnt;
	int tout, rval;

	mutex_lock(&pchip->lock);
	for (icnt = 0; icnt < CMD_MAX; icnt++) {
		if (strncmp(buf, cmds[icnt].str, cmds[icnt].size) == 0)
			break;
	}

	switch (dfid) {
		/* led 0 enable */
	case DFILE_FLASH0_ENABLE:
	case DFILE_TORCH0_ENABLE:
		if (icnt == CMD_ENABLE)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x1,
					       0x1);
		else if (icnt == CMD_DISABLE)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x1,
					       0x0);
		break;
		/* led 1 enable, flash override */
	case DFILE_FLASH1_ENABLE:
		if (icnt == CMD_ENABLE) {
			rval = regmap_update_bits(pchip->regmap,
						  REG_FLASH_LED0_BR, 0x80, 0x0);
			rval |=
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x2,
					       0x2);
		} else if (icnt == CMD_DISABLE) {
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x2,
					       0x0);
		} else if (icnt == CMD_OVERRIDE) {
			rval = regmap_update_bits(pchip->regmap,
						  REG_FLASH_LED0_BR, 0x80,
						  0x80);
			rval |=
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x2,
					       0x2);
		}
		break;
		/* led 1 enable, torch override */
	case DFILE_TORCH1_ENABLE:
		if (icnt == CMD_ENABLE) {
			rval = regmap_update_bits(pchip->regmap,
						  REG_TORCH_LED0_BR, 0x80, 0x0);
			rval |=
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x2,
					       0x2);
		} else if (icnt == CMD_DISABLE) {
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x2,
					       0x0);
		} else if (icnt == CMD_OVERRIDE) {
			rval = regmap_update_bits(pchip->regmap,
						  REG_TORCH_LED0_BR, 0x80,
						  0x80);
			rval |=
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x2,
					       0x2);
		}
		break;
		/* mode control flash/ir */
	case DFILE_FLASH0_ONOFF:
	case DFILE_FLASH1_ONOFF:
		if (icnt == CMD_ON)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0xc,
					       0xc);
		else if (icnt == CMD_OFF)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0xc,
					       0x0);
		else if (icnt == CMD_IRMODE)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0xc,
					       0x4);
		break;
		/* mode control torch */
	case DFILE_TORCH0_ONOFF:
	case DFILE_TORCH1_ONOFF:
		if (icnt == CMD_ON)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0xc,
					       0x8);
		else if (icnt == CMD_OFF)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0xc,
					       0x0);
		break;
		/* strobe pin control */
	case DFILE_FLASH0_SOURCE:
		if (icnt == CMD_ON)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x20,
					       0x20);
		else if (icnt == CMD_OFF)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x20,
					       0x0);
		break;
	case DFILE_TORCH0_SOURCE:
		if (icnt == CMD_ON)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x10,
					       0x10);
		else if (icnt == CMD_OFF)
			rval =
			    regmap_update_bits(pchip->regmap, REG_ENABLE, 0x10,
					       0x0);
		break;
		/* flash time out */
	case DFILE_FLASH0_TIMEOUT:
		rval = kstrtouint((const char *)buf, 10, &tout);
		if (rval < 0)
			break;
		rval = regmap_update_bits(pchip->regmap,
					  REG_FLASH_TOUT, 0x0f, tout);
		break;
	default:
		dev_err(pchip->dev, "error : undefined dev file\n");
		break;
	}
	ktd2668_read_flag(pchip);
	mutex_unlock(&pchip->lock);
	return size;
}

/* flash enable control */
static ssize_t ktd2668_flash0_enable_store(struct device *dev,
					  struct device_attribute *devAttr,
					  const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_FLASH0, DFILE_FLASH0_ENABLE, size);
}

static ssize_t ktd2668_flash1_enable_store(struct device *dev,
					  struct device_attribute *devAttr,
					  const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_FLASH1, DFILE_FLASH1_ENABLE, size);
}

/* flash onoff control */
static ssize_t ktd2668_flash0_onoff_store(struct device *dev,
					 struct device_attribute *devAttr,
					 const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_FLASH0, DFILE_FLASH0_ONOFF, size);
}

static ssize_t ktd2668_flash1_onoff_store(struct device *dev,
					 struct device_attribute *devAttr,
					 const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_FLASH1, DFILE_FLASH1_ONOFF, size);
}

/* flash timeout control */
static ssize_t ktd2668_flash0_timeout_store(struct device *dev,
					   struct device_attribute *devAttr,
					   const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_FLASH0, DFILE_FLASH0_TIMEOUT, size);
}

/* flash source control */
static ssize_t ktd2668_flash0_source_store(struct device *dev,
					  struct device_attribute *devAttr,
					  const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_FLASH0, DFILE_FLASH0_SOURCE, size);
}

/* torch enable control */
static ssize_t ktd2668_torch0_enable_store(struct device *dev,
					  struct device_attribute *devAttr,
					  const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_TORCH0, DFILE_TORCH0_ENABLE, size);
}

static ssize_t ktd2668_torch1_enable_store(struct device *dev,
					  struct device_attribute *devAttr,
					  const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_TORCH1, DFILE_TORCH1_ENABLE, size);
}

/* torch onoff control */
static ssize_t ktd2668_torch0_onoff_store(struct device *dev,
					 struct device_attribute *devAttr,
					 const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_TORCH0, DFILE_TORCH0_ONOFF, size);
}

static ssize_t ktd2668_torch1_onoff_store(struct device *dev,
					 struct device_attribute *devAttr,
					 const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_TORCH1, DFILE_TORCH1_ONOFF, size);
}

/* torch source control */
static ssize_t ktd2668_torch0_source_store(struct device *dev,
					  struct device_attribute *devAttr,
					  const char *buf, size_t size)
{
	return ktd2668_ctrl(dev, buf, ID_TORCH0, DFILE_TORCH0_SOURCE, size);
}

#define ktd2668_attr(_name, _show, _store)\
{\
	.attr = {\
		.name = _name,\
		.mode = 0644,\
	},\
	.show = _show,\
	.store = _store,\
}

static struct ktd2668_files ktd2668_devfiles[DFILE_MAX] = {
	[DFILE_FLASH0_ENABLE] = {
				 .id = ID_FLASH0,
				 .attr =
				 ktd2668_attr("enable", NULL,
					     ktd2668_flash0_enable_store),
				 },
	[DFILE_FLASH0_ONOFF] = {
				.id = ID_FLASH0,
				.attr =
				ktd2668_attr("onoff", NULL,
					    ktd2668_flash0_onoff_store),
				},
	[DFILE_FLASH0_SOURCE] = {
				 .id = ID_FLASH0,
				 .attr =
				 ktd2668_attr("source", NULL,
					     ktd2668_flash0_source_store),
				 },
	[DFILE_FLASH0_TIMEOUT] = {
				  .id = ID_FLASH0,
				  .attr =
				  ktd2668_attr("timeout", NULL,
					      ktd2668_flash0_timeout_store),
				  },
	[DFILE_FLASH1_ENABLE] = {
				 .id = ID_FLASH1,
				 .attr =
				 ktd2668_attr("enable", NULL,
					     ktd2668_flash1_enable_store),
				 },
	[DFILE_FLASH1_ONOFF] = {
				.id = ID_FLASH1,
				.attr =
				ktd2668_attr("onoff", NULL,
					    ktd2668_flash1_onoff_store),
				},
	[DFILE_TORCH0_ENABLE] = {
				 .id = ID_TORCH0,
				 .attr =
				 ktd2668_attr("enable", NULL,
					     ktd2668_torch0_enable_store),
				 },
	[DFILE_TORCH0_ONOFF] = {
				.id = ID_TORCH0,
				.attr =
				ktd2668_attr("onoff", NULL,
					    ktd2668_torch0_onoff_store),
				},
	[DFILE_TORCH0_SOURCE] = {
				 .id = ID_TORCH0,
				 .attr =
				 ktd2668_attr("source", NULL,
					     ktd2668_torch0_source_store),
				 },
	[DFILE_TORCH1_ENABLE] = {
				 .id = ID_TORCH1,
				 .attr =
				 ktd2668_attr("enable", NULL,
					     ktd2668_torch1_enable_store),
				 },
	[DFILE_TORCH1_ONOFF] = {
				.id = ID_TORCH1,
				.attr =
				ktd2668_attr("onoff", NULL,
					    ktd2668_torch1_onoff_store),
				}
};

static void ktd2668_df_remove(struct ktd2668 *pchip, enum ktd2668_devfile dfid)
{
	enum ktd2668_devfile icnt;

	for (icnt = dfid; icnt > 0; icnt--)
		device_remove_file(pchip->cdev[ktd2668_devfiles[icnt - 1].id].
				   dev, &ktd2668_devfiles[icnt - 1].attr);
}

static int ktd2668_df_create(struct ktd2668 *pchip)
{
	enum ktd2668_devfile icnt;
	int rval;

	for (icnt = 0; icnt < DFILE_MAX; icnt++) {
		rval =
		    device_create_file(pchip->cdev[ktd2668_devfiles[icnt].id].
				       dev, &ktd2668_devfiles[icnt].attr);
		if (rval < 0) {
			ktd2668_df_remove(pchip, icnt);
			return rval;
		}
	}
	return 0;
}

void ktd2688_torch0_set_brightness(int brightness)
{
	if (pchip_dev)
		ktd2668_torch0_brightness_set(&(pchip_dev->cdev[ID_TORCH0]),
				(enum led_brightness)brightness);
}

void ktd2688_torch1_set_brightness(int brightness)
{
	if (pchip_dev)
		ktd2668_torch1_brightness_set(&(pchip_dev->cdev[ID_TORCH1]),
				(enum led_brightness)brightness);
}

void ktd2688_torch0_enable(void)
{
	if (pchip_dev)
		regmap_update_bits(pchip_dev->regmap, REG_ENABLE, 0x1, 0x1);
}

void ktd2688_torch0_disable(void)
{
	if (pchip_dev)
		regmap_update_bits(pchip_dev->regmap, REG_ENABLE, 0x1, 0x0);
}

void ktd2688_torch1_enable(void)
{
	if (pchip_dev) {
		regmap_update_bits(pchip_dev->regmap, REG_TORCH_LED0_BR,
				0x80, 0x0);
		regmap_update_bits(pchip_dev->regmap, REG_ENABLE, 0x2, 0x2);
	}
}

void ktd2688_torch1_disable(void)
{
	if (pchip_dev)
		regmap_update_bits(pchip_dev->regmap, REG_ENABLE, 0x2, 0x0);
}

void ktd2688_torch0_on(void)
{
	if (pchip_dev)
		regmap_update_bits(pchip_dev->regmap, REG_ENABLE, 0xc, 0x8);
}

void ktd2688_torch0_off(void)
{
	if (pchip_dev)
		regmap_update_bits(pchip_dev->regmap, REG_ENABLE, 0xc, 0x0);
}

void ktd2688_torch1_on(void)
{
	        if (pchip_dev)
			regmap_update_bits(pchip_dev->regmap, REG_ENABLE,
					0xc, 0x8);
}

void ktd2688_torch1_off(void)
{
	        if (pchip_dev)
			regmap_update_bits(pchip_dev->regmap, REG_ENABLE,
					0xc, 0x0);
}



static const struct regmap_config ktd2668_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = 0xff,
};

static int ktd2668_parse_dt(struct ktd2668 *pchip, struct device *dev)
{
	struct device_node *np = dev->of_node;
	int ret = 0;
	int flash_gpio_en = 0;

	if (!np)
		return -EINVAL;

	flash_gpio_en = of_get_named_gpio(np, "flashen_gpio", 0);
	if (flash_gpio_en < 0) {
		pr_err("%s: of_get_named_gpio failed: %d\n", __func__,
							flash_gpio_en);
		return flash_gpio_en;
	}

	pchip->flash_gpio_en = flash_gpio_en;

	return 0;
}


static int ktd2668_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct ktd2668 *pchip;
	int rval;

	/* i2c check */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "i2c functionality check fail.\n");
		return -EOPNOTSUPP;
	}

	pchip = devm_kzalloc(&client->dev, sizeof(struct ktd2668), GFP_KERNEL);
	if (!pchip)
		return -ENOMEM;

	pchip->dev = &client->dev;
	pchip->regmap = devm_regmap_init_i2c(client, &ktd2668_regmap);
	if (IS_ERR(pchip->regmap)) {
		rval = PTR_ERR(pchip->regmap);
		dev_err(&client->dev, "Failed to allocate register map: %d\n",
			rval);
		return rval;
	}
	mutex_init(&pchip->lock);
	i2c_set_clientdata(client, pchip);
	pchip_dev = pchip;

	rval = ktd2668_parse_dt(pchip, &client->dev);
	if (rval < 0) {
		dev_err(&client->dev,"node error");
		return rval;
	}

	rval = gpio_request(pchip->flash_gpio_en, "flashen_gpio");
	if (rval) {
		pr_err("%s failed: %d\n", __func__, pchip->flash_gpio_en);
		return rval;
	}

	rval = gpio_direction_output(pchip->flash_gpio_en, 1);
	if (rval) {
		pr_err("%s failed!!\n", __func__);
		return rval;
	}

	/* led class register */
	rval = ktd2668_led_register(pchip);
	if (rval < 0)
		return rval;

	/* create dev files */
	rval = ktd2668_df_create(pchip);
	if (rval < 0) {
		ktd2668_led_unregister(pchip, ID_MAX);
		return rval;
	}

	dev_info(pchip->dev, "ktd2668 leds initialized\n");
	return 0;
}

static int ktd2668_remove(struct i2c_client *client)
{
	struct ktd2668 *pchip = i2c_get_clientdata(client);

	ktd2668_df_remove(pchip, DFILE_MAX);
	ktd2668_led_unregister(pchip, ID_MAX);

	return 0;
}

static const struct i2c_device_id ktd2668_id[] = {
	{KTD2668_NAME, 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, ktd2668_id);

#ifdef CONFIG_OF
static struct of_device_id leds_ktd2668_match_table[] = {
	        { .compatible = "kinetic,leds_ktd2668",},
		        {},
};
#else
#define leds_ktd2668_match_table NULL
#endif

static struct i2c_driver ktd2668_i2c_driver = {
	.driver = {
		   .name = KTD2668_NAME,
		   .owner = THIS_MODULE,
		   .pm = NULL,
		   .of_match_table = leds_ktd2668_match_table,
		   },
	.probe = ktd2668_probe,
	.remove = ktd2668_remove,
	.id_table = ktd2668_id,
};

module_i2c_driver(ktd2668_i2c_driver);
MODULE_DESCRIPTION("Flash Lighting driver for KTD2668");
MODULE_AUTHOR("Harsh Jain <harsh.jain@samsung.com>");
MODULE_AUTHOR("Daniel Jeong <daniel.jeong@ti.com>");
MODULE_AUTHOR("G.Shark Jeong <gshark.jeong@gmail.com>");
MODULE_LICENSE("GPL v2");

