#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <soc/sprd/hardware.h>
#include <soc/sprd/board.h>
#include <soc/sprd/adi.h>
#include <linux/leds.h>
#include <linux/highmem.h>
#include <linux/freezer.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <video/sprd_img.h>

#include "flash_ktd2692.h"
#include <linux/leds-ktd2692.h>

uint32_t ktd2692_flash_on(uint32_t flash_index) {
	printk("%s: In. flash_index %d\n",__func__,flash_index);
	if(0 == flash_index) {
		ktd2692_set_torch_current(104); /*104mA for pride device*/
		ktd2692_led_mode_ctrl(3); //Torch or pre flash
	} else {
	}
	printk("%s: Out\n",__func__);
	return 0;
}

uint32_t ktd2692_flash_high_light(uint32_t flash_index) {
	printk("%s: In. flash_index %d\n",__func__,flash_index);
	if(0 == flash_index) {
		ktd2692_set_flash_current(875); /*875mA for pride device*/
		ktd2692_led_mode_ctrl(2); //high flash
	} else {
	}
	return 0;
}

uint32_t ktd2692_flash_close(uint32_t flash_index) {
	printk("%s: In. flash_index %d\n",__func__,flash_index);
	if(0 == flash_index) {
		ktd2692_led_mode_ctrl(1); //close
	} else {
	}
	return 0;
}
