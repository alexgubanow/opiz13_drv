#include "pwr.h"
#include "gpio.h"

static char _3V3_is_inited = 0;

void switch_3V3(int state)
{
  if (_3V3_is_inited == 0)
  {
    pinMode(_3V3EN, OUTPUT);
  }
  gpio_set_value(_3V3EN, state);
}
