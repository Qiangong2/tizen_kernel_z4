#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <soc/sprd/hardware.h>
#include <soc/sprd/board.h>
#include <soc/sprd/adi.h>
#include <linux/highmem.h>
#include <linux/freezer.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <video/sprd_img.h>

#include "flash.h"
#include "flash_ktd2668.h"
#include "flash_ktd2692.h"
#include "flash_rt5033.h"
#include "flash_rt8547.h"

unsigned int flash_hw_rev;

uint32_t sprd_multi_flash_on(uint32_t flash_index)
{
	printk("%s: In.  flash_index %d\n",__func__,flash_index);
	if (0 == flash_index)
	{
		#ifdef CONFIG_REAR_FLASH_KTD2668
		ktd2668_flash_on(flash_index);
		#endif
		if (flash_hw_rev == 2) {
			#ifdef CONFIG_REAR_FLASH_KTD2692
			ktd2692_flash_on(flash_index);
			#endif
		} else {
			#ifdef CONFIG_REAR_FLASH_RT8547
			rt8547_flash_on(flash_index);
			#endif
		}
	} else {
		#ifdef CONFIG_FRONT_FLASH_SM5701
		sprd_flash_on();
		#endif

		#ifdef CONFIG_FRONT_FLASH_RT5033
		rt5033_flash_on(flash_index);
		#endif
	}
	printk("%s: Out\n",__func__);
	return 0;
}

uint32_t sprd_multi_flash_high_light(uint32_t flash_index)
{
	printk("%s: In.  flash_index %d\n",__func__,flash_index);
	if (0 == flash_index)
	{
		#ifdef CONFIG_REAR_FLASH_KTD2668
		ktd2668_flash_high_light(flash_index); //will tune as per main flash
		#endif

		if (flash_hw_rev == 2) {
			#ifdef CONFIG_REAR_FLASH_KTD2692
			ktd2692_flash_high_light(flash_index);
			#endif
		} else {
			#ifdef CONFIG_REAR_FLASH_RT8547
			rt8547_flash_high_light(flash_index);
			#endif
		}
	} else {
		#ifdef CONFIG_FRONT_FLASH_SM5701
		sprd_flash_high_light();
		#endif

		#ifdef CONFIG_FRONT_FLASH_RT5033
			rt5033_flash_high_light(flash_index);
		#endif
	}
	printk("%s: Out\n",__func__);
	return 0;
}

uint32_t sprd_multi_flash_close(uint32_t flash_index)
{
	printk("%s: In.  flash_index %d\n",__func__,flash_index);
	if (0 == flash_index)
	{
		#ifdef CONFIG_REAR_FLASH_KTD2668
		ktd2668_flash_close(flash_index);
		#endif

		if (flash_hw_rev == 2) {
			#ifdef CONFIG_REAR_FLASH_KTD2692
			ktd2692_flash_close(flash_index);
			#endif
		} else {
			#ifdef CONFIG_REAR_FLASH_RT8547
			rt8547_flash_close(flash_index);
			#endif
		}
	} else {
		#ifdef CONFIG_FRONT_FLASH_SM5701
		sprd_flash_close();
		#endif

		#ifdef CONFIG_FRONT_FLASH_RT5033
		rt5033_flash_close(flash_index);
		#endif
	}
	printk("%s: Out\n",__func__);
	return 0;
}
