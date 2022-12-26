#include "buzzer.h"
#include "gpio.h"

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>

// static char _BUZZ1_is_inited = 0;
// static char _BUZZ2_is_inited = 0;

// void beep2(void)
// {
//   if (_BUZZ1_is_inited == 0)
//   {
//     pinMode(BUZZ1, OUTPUT);
//   }
//   if (_BUZZ2_is_inited == 0)
//   {
//     pinMode(BUZZ2, OUTPUT);
//   }
//   gpio_set_value(BUZZ1, 1);
//   msleep(200);
//   gpio_set_value(BUZZ1, 0);
//   msleep(200);
//   gpio_set_value(BUZZ2, 1);
//   msleep(200);
//   gpio_set_value(BUZZ2, 0);
//   msleep(200);
// }

static int freqs[MAX_PINS];
static struct task_struct *threads[MAX_PINS];
// static struct task_struct *BUZZ1_thread;
// static struct task_struct *BUZZ2_thread;

/* softToneWrite:	Write a frequency value to the given pin*/
void softToneWrite(int pin, int freq)
{
  pin &= 63;

  /**/ if (freq < 0)
    freq = 0;
  else if (freq > 5000) // Max 5KHz
    freq = 5000;

  freqs[pin] = freq;
}
int softToneThread(void *arg)
{
  int pin = *(int *)arg;
  int freq, halfPeriod;

  // struct sched_param param;
  // param.sched_priority = sched_get_priority_max(SCHED_RR);
  // pthread_setschedparam(pthread_self(), SCHED_RR, &param);
  // piHiPri(50);
  // kthread_should_stop call is important.
  while (!kthread_should_stop())
  {
    freq = freqs[pin];
    if (freq == 0)
      msleep(500);
    else
    {
      halfPeriod = 500000 / freq;
      gpio_set_value(pin, HIGH);
      udelay(halfPeriod);
      gpio_set_value(pin, LOW);
      udelay(halfPeriod);
    }
  }
  printk(KERN_INFO "softToneThread#%d stopped\n", pin);
  return 0;
}
/* softToneCreate:	Create a new tone thread.*/
int softToneCreate(int pin)
{
  char th_name[20] = {"\0"};
  if (threads[pin] != NULL)
  {
    pr_err("thread for pin#%d is already running\n", pin);
    return -1;
  }
  freqs[pin] = 0;
  sprintf(th_name, "softToneThread_%d", pin);
  threads[pin] = kthread_create(softToneThread, (void *)&pin, (const char *)th_name);
  if (threads[pin] != NULL)
  {
    wake_up_process(threads[pin]);
    printk(KERN_INFO "%s is running\n", th_name);
  }
  else
  {
    printk(KERN_INFO "kthread %s could not be created\n", th_name);
    return -1;
  }
  return 0;
}

/* softToneStop:	Stop an existing softTone thread*/
void softToneStop(int pin)
{
  if (threads[pin] != NULL)
  {
    if (kthread_stop(threads[pin]) != 0)
    {
      pr_err("can't stop thread %d", pin);
    }
    threads[pin] = NULL;
    gpio_set_value(BUZZ1, 0);
    gpio_set_value(BUZZ2, 0);
  }
}
