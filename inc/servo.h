#ifndef __SERVO_H
#define __SERVO_H

/* ดึงไฟล์หลักของบอร์ด */
#include "n32g031.h"
/* ดึงคำสั่งควบคุมพินและไทม์เมอร์โดยตรง */
#include "n32g031_gpio.h"
#include "n32g031_tim.h"
#include "n32g031_rcc.h"

/* ใช้ PA1 สำหรับ TIM3 Channel 2 */
#define SERVO_PIN        GPIO_PIN_1
#define SERVO_PORT       GPIOA
#define SERVO_AF_AFx     GPIO_AF1_TIM3 

void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);

#endif