#ifndef _gpio_h
#define _gpio_h
#include <linux/gpio.h>

#define INPUT 0
#define OUTPUT 1
#define PWM_OUTPUT 2
#define GPIO_CLOCK 3
#define SOFT_PWM_OUTPUT 4
#define SOFT_TONE_OUTPUT 5
#define PWM_TONE_OUTPUT 6

#define LOW 0
#define HIGH 1

#define OPI_BLOCK_SIZE (4 * 1024)

#define OPI_GPIOA_BASE (0x0300B000)
#define OPI_GPIO_NUM (0x40)
#define OPI_GPIO_BASE_MAP (0x0300B000)
#define OPI_MEM_INFO (1024)
#define OPI_GPIOL_BASE (0x07022000)
#define OPI_GPIOL_BASE_MAP (0x07022000)
#define OPI_MAP_SIZE_L (4096 * 1)
#define OPI_GPIO_PWM_OP (0x0300A000)

#define OPI_MAX_PIN_NUM OPI_GPIO_NUM
#define OPI_MAP_SIZE OPI_MAP_SIZE_L
#define OPI_MAP_MASK (OPI_MAP_SIZE - 1)
#define OPI_PI_GPIO_MASK (~(OPI_GPIO_NUM - 1))
#define OPI_GPIO_BASE OPI_GPIOA_BASE
#define OPI_ORANGEPI_MEM_INFO OPI_MEM_INFO
#define OPI_GPIO_PWM OPI_GPIO_PWM_OP

#define _3V3EN PC9
#define WS_DI PC6
#define FAN1 PC5
#define FAN2 PC11
#define _595_CS PC15
#define BUZZ1 PC10
#define BUZZ2 PC7
#define SPI1_CS CE_1
#define OLED_RST PC14
#define OLED_DC PC8

#define CE_1 233 // 15- from doc
#define PC5 69   // 7
#define PC6 70   // 5
#define PC7 71   // 13
#define PC8 72   // 8
#define PC9 73   // 2
#define PC10 74  // 16
#define PC11 75  // 6
#define PC14 78  // 10
#define PC15 79  // 9

/*int pinToGpioOrangePi[64] = {
    229,
    228,
    73,
    226,
    227,
    70,
    75,
    69,
    72,
    79,
    78,
    231,
    232,
    71,
    230,
    233,
    74,
    65,
    272,
    262,
    234,
    // Padding:
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,         // ... 63
};*/

/* pinMode:Sets the mode of a pin to be input, output or PWM output */
void pinMode(int pin, int mode);
void releasePins(void);

#endif
