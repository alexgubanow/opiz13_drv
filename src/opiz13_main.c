#include "opiz13_private.h"
#include "pwr.h"
#include "gpio.h"

/*first of the requested range of minor numbers*/
#define FIRST_MINOR 0
/*the number of minor numbers required*/
#define MINOR_CNT 1
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Oleksandr Hubanov <alex@0x0h.com>");
MODULE_DESCRIPTION("OpiZ13 faceboard Driver");

/*Global variable of device*/
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

/*default methods to handle opening and closing device*/
static int _drv_open(struct inode *i, struct file *f);
static int _drv_close(struct inode *i, struct file *f);
/*handle of query to driver*/
static long _drv_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

/*declaring default handles of driver*/
static struct file_operations _drv_fops = {
    .owner = THIS_MODULE, .open = _drv_open, .release = _drv_close, .unlocked_ioctl = _drv_ioctl};

static int _drv_open(struct inode *i, struct file *f) { return 0; }
static int _drv_close(struct inode *i, struct file *f) { return 0; }

/*Method to cast incoming arg to unsigned char struct*/
int copyFrom(unsigned char *buff, unsigned long addr)
{
  if (copy_from_user(buff, (unsigned char *)addr, sizeof(unsigned char)))
  {
    printk(KERN_ERR "opiz13: Error while getting data from user\n");
    return -EACCES;
  }
  return 0;
}
/*Nethod pushing unsigned char answer to user space*/
int copyTo(unsigned char *answer, unsigned long addr)
{
  if (copy_to_user((unsigned char *)addr, answer, sizeof(unsigned char)))
  {
    printk(KERN_ERR "opiz13: Error while transmitting data to user about Light\n");
    return -EACCES;
  }
  return 0;
}

/*handle of query to driver*/
static long _drv_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  // variables to store possible variant of request
  //unsigned char rq;
  //int rq_idx;
  int err = 0;
  printk(KERN_INFO "opiz13: _drv_ioctl");

  return err;
}

/*init method of driver*/
static int __init _drv_ioctl_init(void)
{
  int ret;
  struct device *dev_ret;

  if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "_drv_ioctl")) < 0)
  {
    printk(KERN_DEBUG "opiz13: Fail while alloc region\n");
    return ret;
  }
  printk(KERN_DEBUG "opiz13: Allocated region\n");

  cdev_init(&c_dev, &_drv_fops);

  if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
  {
    printk(KERN_ERR "opiz13: Fail while adding device\n");
    return ret;
  }
  printk(KERN_DEBUG "opiz13: Added this to device pool\n");

  if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
  {
    printk(KERN_ERR "opiz13: Fail while create class\n");
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    return PTR_ERR(cl);
  }
  printk(KERN_DEBUG "opiz13: Created class\n");
  if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "opiz13")))
  {
    printk(KERN_ERR "opiz13: Failed to create device\n");
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    return PTR_ERR(dev_ret);
  }
  switch_3V3(1);
  printk(KERN_INFO "opiz13: Initialization finished\n");

  return 0;
}
/*exit method of driver*/
static void __exit _drv_ioctl_exit(void)
{
  switch_3V3(0);
	releasePins();
  device_destroy(cl, dev);
  class_destroy(cl);
  cdev_del(&c_dev);
  unregister_chrdev_region(dev, MINOR_CNT);
  printk(KERN_INFO "opiz13: Unloading finished, bye bye\n");
}
/*declaring init and exit*/
module_init(_drv_ioctl_init);
module_exit(_drv_ioctl_exit);
