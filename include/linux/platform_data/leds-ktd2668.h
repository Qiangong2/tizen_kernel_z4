#ifndef __KTD2668_H
#define __KTD2668_H

#define KTD2668_NAME "leds-ktd2668"
#define KTD2668_ADDR 0x63

void ktd2688_torch0_set_brightness(int);
void ktd2688_torch1_set_brightness(int);
void ktd2688_torch0_enable(void);
void ktd2688_torch0_disable(void);
void ktd2688_torch1_enable(void);
void ktd2688_torch1_disable(void);
void ktd2688_torch0_on(void);
void ktd2688_torch0_off(void);
void ktd2688_torch1_on(void);
void ktd2688_torch1_off(void);
#endif /* __KTD2668_H */

