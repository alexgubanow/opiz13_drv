#include "opiz13_private.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

/*first of the requested range of minor numbers*/
#define FIRST_MINOR 0
/*the number of minor numbers required*/
#define MINOR_CNT 1
/*Global variable of device*/
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

/*default methods to handle opening and closing device*/
static int _drv_open(struct inode *i, struct file *f) { return 0; }
static int _drv_close(struct inode *i, struct file *f) { return 0; }

/*Method to cast incoming arg to lghtIO_arg_t struct*/
int copyFrom(lghtIO_arg_t *buff, unsigned long addr)
{
  if (copy_from_user(buff, (lghtIO_arg_t *)addr, sizeof(lghtIO_arg_t)))
  {
    printk(KERN_ERR "opiz13: Error while getting data from user\n");
    return -EACCES;
  }
  return 0;
}
/*Nethod pushing lghtIO_arg_t answer to user space*/
int copyTo(lghtIO_arg_t *answer, unsigned long addr)
{
  if (copy_to_user((lghtIO_arg_t *)addr, answer, sizeof(lghtIO_arg_t)))
  {
    printk(KERN_ERR "opiz13: Error while transmitting data to user about Light\n");
    return -EACCES;
  }
  return 0;
}

/*handle of query to driver*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
static int _drv_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long _drv_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
  // variables to store possible variant of request
  lghtIO_arg_t rq;
  int rq_idx;
  int err = 0;
  // recognize requst type
  switch (cmd)
  {
  case getLghtByIdx: // getter
    printk(KERN_INFO "opiz13: Request for Light");
    // casting incoming arg to lghtIO_arg_t
    err = copyFrom(&rq, arg);
    if (err < 0)
    {
      return err;
    }
    printk(KERN_INFO "opiz13: Light#%d\n", rq.idx);
    // geting color
    rq.clrT = getColor(rq.idx);
    // pushing answer back
    copyTo(&rq, arg);
    if (err < 0)
    {
      return err;
    }
    break;
  default:
    printk(KERN_ERR "opiz13: Received wrong control sequence\n");
    return -EINVAL;
  }

  return err;
}
/*getter of color, returns color of light by idx*/
lghtColor getColor(int targetIdx)
{
  return lColor;
}

/*setter of color, push given color to light by idx*/
void setColor(lghtColor targetColor, int targetIdx)
{
  idx = targetIdx;
  lColor = targetColor;
  printk(KERN_INFO "opiz13: Light idx#%d is become to %s\n", targetIdx, enumToColor(targetColor));
}
/*declaring default handles of driver*/
static struct file_operations lghtIO_fops =
    {
        .owner = THIS_MODULE,
        .open = _drv_open,
        .release = _drv_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
        .ioctl = _drv_ioctl
#else
        .unlocked_ioctl = _drv_ioctl
#endif
};
/*init method of driver*/
static int __init _drv_ioctl_init(void)
{
  int ret;
  struct device *dev_ret;

  if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "_drv_ioctl")) < 0)
  {
    printk(KERN_DEBUG "opiz13: Fail while alloc region for\n");
    return ret;
  }
  printk(KERN_DEBUG "opiz13: Allocated region for\n");

  cdev_init(&c_dev, &lghtIO_fops);

  if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
  {
    printk(KERN_DEBUG "opiz13: Fail while adding this\n");
    return ret;
  }
  printk(KERN_DEBUG "opiz13: Added this to device pool\n");

  if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
  {
    printk(KERN_DEBUG "opiz13: Fail while create class for\n");
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    return PTR_ERR(cl);
  }
  printk(KERN_DEBUG "opiz13: Created class for\n");
  if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "opiz13")))
  {
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    return PTR_ERR(dev_ret);
  }
  printk(KERN_INFO "opiz13: Loading finished, enjoy\n");

  return 0;
}
/*exit method of driver*/
static void __exit _drv_ioctl_exit(void)
{
  device_destroy(cl, dev);
  class_destroy(cl);
  cdev_del(&c_dev);
  unregister_chrdev_region(dev, MINOR_CNT);
  printk(KERN_INFO "opiz13: Unloading finished, bye bye\n");
}
/*declaring init and exit*/
module_init(_drv_ioctl_init);
module_exit(_drv_ioctl_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Oleksandr Hubanov <alex@0x0h.com>");
MODULE_DESCRIPTION("OpiZ13 faceboard Driver");