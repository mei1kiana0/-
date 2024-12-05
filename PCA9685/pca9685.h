#ifndef __PCA9685_H
#define __PCA9685_H

#define PCA9685_ADDRESS 0x80 // 默认地址（A0-A5接地）
#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE
#define LED0_ON_L 0x06
#define LED0_OFF_L 0x08

#include "main.h"
#include "i2c.h"

void PCA9685_Init(void);
void PCA9685_SetServoAngle(uint8_t channel, float angle);
#endif
