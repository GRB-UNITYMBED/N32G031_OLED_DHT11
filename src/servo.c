#include "servo.h"

void Servo_Init(void)
{
    /* 1. เปิด Clock สำหรับ GPIOA และ TIM3 */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);

    /* 2+3. ตั้งค่า GPIO PA1 เป็น Alternate Function ของ TIM3 CH2
       (SDK นี้ไม่มี GPIO_ConfigPinAf — ตั้งผ่าน field GPIO_Alternate แทน) */
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);                   /* เดิม: GPIO_StructInit */
    GPIO_InitStructure.Pin            = SERVO_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = SERVO_AF_AFx;       /* แทน GPIO_ConfigPinAf */
    GPIO_InitPeripheral(SERVO_PORT, &GPIO_InitStructure);   /* เดิม: GPIO_Init */

/* 4. ตั้งค่า TIM3 สำหรับความถี่ 50Hz (Standard Servo) */
    extern uint32_t SystemCoreClock; // ดึงค่าความเร็วบอร์ดจริงๆ ณ เวลานั้นมาใช้

    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);          
    
    // เปลี่ยนบรรทัดนี้บรรทัดเดียวครับ ให้มันคำนวณอัตโนมัติ
    TIM_TimeBaseStructure.Prescaler = (SystemCoreClock / 1000000) - 1; 
    
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = 20000 - 1;           
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;
    TIM_InitTimeBase(TIM3, &TIM_TimeBaseStructure);

    /* 5. ตั้งค่า PWM Channel 2 (PA1) */
    OCInitType TIM_OCInitStructure;
    TIM_InitOcStruct(&TIM_OCInitStructure);                 /* เดิม: TIM_OCStructInit */
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 1500;                /* เริ่มต้นที่ 90 องศา */
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
    TIM_InitOc2(TIM3, &TIM_OCInitStructure);               /* เดิม: TIM_OC2Init */

    /* 6. เปิด Preload register (จำเป็นสำหรับให้ PWM ออก) */
    TIM_ConfigOc2Preload(TIM3, TIM_OC_PRE_LOAD_ENABLE);
    TIM_ConfigArPreload(TIM3, ENABLE);

    /* 7. เปิดการทำงานของ Timer */
    TIM_Enable(TIM3, ENABLE);
}

void Servo_SetAngle(uint8_t angle)
{
    uint32_t pulse_width;
    if (angle > 180) angle = 180;

    /* แปลงองศา 0-180 เป็นช่วงสัญญาณ 1000-2000 microsecond */
    pulse_width = 1000 + ((angle * 1000) / 180);

    /* ส่งค่าไปยัง Register ของ TIM3 Channel 2 */
    TIM3->CCDAT2 = (uint16_t)pulse_width;
}