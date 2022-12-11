#ifndef _opiz13_private
#define _opiz13_private

#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

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

/*Global variable of device*/
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
void __iomem *gpio_mapped_addr;

/*declaring default handles of driver*/
static struct file_operations _drv_fops = {.owner = THIS_MODULE,
                                           .open = _drv_open,
                                           .release = _drv_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
                                           .ioctl = _drv_ioctl
#else
                                           .unlocked_ioctl = _drv_ioctl
#endif
};

#endif
