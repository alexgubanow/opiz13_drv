#ifndef _buzzer_h
#define _buzzer_h

int softToneCreate(int pin);
void softToneStop(int pin);
void softToneWrite(int pin, int freq);

#endif
