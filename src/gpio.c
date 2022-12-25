#include "gpio.h"
#include "opiz13_private.h"

void pinMode(int pin, int mode)
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
