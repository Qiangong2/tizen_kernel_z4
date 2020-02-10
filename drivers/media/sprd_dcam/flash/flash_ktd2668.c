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

#include "flash_ktd2668.h"
#include <linux/platform_data/leds-ktd2668.h>

uint32_t ktd2668_flash_on(uint32_t flash_index) {
    printk("%s: In. flash_index %d\n",__func__,flash_index);
    if(0 == flash_index) {
        ktd2688_torch0_set_brightness(100); //Will change base on the preflash tuning
	ktd2688_torch0_enable();
	ktd2688_torch0_on();
    } else {
	ktd2688_torch1_set_brightness(100); //Will change base on the preflash tuning
	ktd2688_torch1_enable();
	ktd2688_torch1_on();
    }
    printk("%s: Out\n",__func__);
    return 0;
}

uint32_t ktd2668_flash_high_light(uint32_t flash_index) {
    printk("%s: In. flash_index %d\n",__func__,flash_index);
    if(0 == flash_index) {
        ktd2688_torch0_set_brightness(0x7F); //Will change base on the mainflash tuning
	ktd2688_torch0_enable();
	ktd2688_torch0_on();
    } else {
        ktd2688_torch1_set_brightness(0x7F); //Will change base on the mainflash tuning
	ktd2688_torch1_enable();
	ktd2688_torch1_on();
    }
    return 0;
}

uint32_t ktd2668_flash_close(uint32_t flash_index) {
    printk("%s: In. flash_index %d\n",__func__,flash_index);
    if(0 == flash_index) {
	ktd2688_torch0_off();
	ktd2688_torch0_disable();
    } else {
	ktd2688_torch1_off();
	ktd2688_torch1_disable();
    }
    return 0;
}


