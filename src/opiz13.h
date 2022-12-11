#ifndef _opiz13_drv
#define _opiz13_drv
#include <linux/ioctl.h>

/* The major device number. We can't rely on dynamic registration any more, because ioctls need to know it. */
#define MAJOR_NUM 100

#define SWITCH_3v3_PWR _IOW(MAJOR_NUM, 1, unsigned char)

#endif
