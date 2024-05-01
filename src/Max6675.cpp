#include<Max6675.h>
#include<Arduino.h>
uint16_t GetTempValue()
{
    return SPI.transfer16(0x70);
}
void Max6675Init()
{
    SPI.begin();
}