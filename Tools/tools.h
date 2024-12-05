#ifndef __MOTOR_H
#define __MOTOR_H

#include "math.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdint.h"
//#include "toolfuncs.h"
//#include "tim.h"
#include "usart.h"
#include "gpio.h"

//轮子ID	F:front R:rear
#define LF_MOTOR_ID 1
#define LR_MOTOR_ID 2
#define RF_MOTOR_ID 3
#define RR_MOTOR_ID 4

//OPS数据包
#define A 0			//航向角
#define ANGLE_X 1	//不用
#define ANGLE_Y 2	//不用
#define X 3			//X坐标
#define Y 4			//Y坐标
#define V_FRONT 5	//航向分速度，不用
//OPS暂未使用

void Motor_SetSpeed(uint8_t address, uint8_t direction, uint16_t speed, uint8_t acceleration);
void SetCarSpeed(float vx, float vy, float vSpin);
void Motor_SetPosition(uint8_t address,uint8_t direction,uint16_t speed,uint32_t pluse);
void Car_move(float vx, float vy, float vSpin,uint32_t pluse);
void Car_turn_right(void);


#endif
