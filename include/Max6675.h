#ifndef __MAX6675_H
#define __MAX6675_H
#include <Arduino.h>
#include <SPI.h>
void Max6675Init();
uint16_t GetTempValue();
#endif