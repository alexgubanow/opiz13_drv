#include "gpio.h"
#include "opiz13_private.h"

#include <linux/list.h>
#include <linux/slab.h>

/*Linked List Node*/
struct pin_list_entry
{
  struct list_head list; // linux kernel list implementation
  int pin;
};
LIST_HEAD(pin_list);

void pinMode(int pin, int mode)
{
  struct pin_list_entry *requested_pin = NULL;
  struct pin_list_entry *temp;
  list_for_each_entry(temp, &pin_list, list)
  {
    if (temp->pin == pin)
    {
      requested_pin = temp;
      break;
    }
  }
  if (requested_pin == NULL)
  {
    if (gpio_is_valid(pin) == false)
    {
      pr_err("gpio_is_valid fail\n");
      return;
    }
    if (gpio_request(pin, NULL))
    {
      pr_err("gpio_request fail\n");
      return;
    }
    requested_pin = kmalloc(sizeof(struct pin_list_entry), GFP_KERNEL);
    /*Assgin the data that is received*/
    requested_pin->pin = pin;
    /*Init the list within the struct*/
    INIT_LIST_HEAD(&requested_pin->list);
    /*Add Node to Linked List*/
    list_add_tail(&requested_pin->list, &pin_list);
  }
  if (mode == INPUT)
  {
    gpio_direction_input(pin);
  }
  else if (mode == OUTPUT)
  {
    gpio_direction_output(pin, 0);
  }
  // else if (mode == PWM_OUTPUT)
  // {
  //   if (pin != 5)
  //   {
  //     printf("the pin you choose doesn't support hardware PWM\n");
  //     printf("you can select wiringPi pin %d for PWM pin\n", 42);
  //     printf("or you can use it in softPwm mode\n");
  //     return;
  //   }
  //   OrangePi_set_gpio_mode(pin, PWM_OUTPUT);
  //   wiringPinMode = PWM_OUTPUT;
  //   return;
  // }
}

void releasePins(void)
{
  struct pin_list_entry *cursor, *temp;
  list_for_each_entry(temp, &pin_list, list) { gpio_free(temp->pin); }
  temp = NULL;
  list_for_each_entry_safe(cursor, temp, &pin_list, list)
  {
    list_del(&cursor->list);
    kfree(cursor);
  }
}