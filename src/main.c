#include "n32g031.h"
#include "n32g031_gpio.h"
#include "n32g031_rcc.h"
#include "n32g031_adc.h" 

/* ====================================================================
 * ⚙️ [โซนตั้งค่าฮาร์ดแวร์] 
 * ==================================================================== */
#define PULSE_MIN     2250   // เซอร์โว 0 องศา (ซ้ายสุด)
#define PULSE_MAX     5500   // เซอร์โว 180 องศา (ขวาสุด)
#define KNOB_MAX      4095   // ค่าสูงสุดของตัวหมุนบนบอร์ด N32 (12-bit)

/* ====================================================================
 * [โซนหลังบ้าน] ฟังก์ชันผู้ช่วยของพี่ AI
 * ==================================================================== */
void Delay_Loop(volatile uint32_t count) {
    while(count--) { __NOP(); }
}

/* ฟังก์ชันตั้งค่าตัวหมุน (ADC) */
void Setup_Knob(void) {
    // ✅ แก้ไขจุดที่ 1: แยกเปิด Clock ให้พอร์ต A (APB2) และ ADC (AHB) ตามสถาปัตยกรรมของชิป
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);
    
    // ตั้งค่าขา PA0 ให้เป็นขารับสัญญาณ Analog
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    // เปิดใช้งานระบบอ่านค่า ADC
    ADC_InitType ADC_InitStructure;
    ADC_InitStruct(&ADC_InitStructure);
    ADC_InitStructure.MultiChEn      = DISABLE;
    ADC_InitStructure.ContinueConvEn = DISABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = 1;
    ADC_Init(ADC, &ADC_InitStructure);
    ADC_Enable(ADC, ENABLE);
}

/* ฟังก์ชันอ่านค่าจากตัวหมุน (คืนค่าเป็นตัวเลข 0 ถึง 4095) */
uint32_t Read_Knob(void) {
    // ✅ แก้ไขจุดที่ 2: เปลี่ยนชื่อตัวแปรจาก 55CYCLES5 เป็น 56CYCLES5 ตามที่ GCC แนะนำ
    ADC_ConfigRegularChannel(ADC, ADC_CH_0, 1, ADC_SAMP_TIME_56CYCLES5);
    ADC_EnableSoftwareStartConv(ADC, ENABLE);
    while(ADC_GetFlagStatus(ADC, ADC_FLAG_ENDC) == RESET);
    ADC_ClearFlag(ADC, ADC_FLAG_ENDC);
    return ADC_GetDat(ADC);
}

/* ฟังก์ชันส่งสัญญาณให้เซอร์โว */
void Servo_Step(uint32_t pulse_width) {
    GPIO_SetBits(GPIOA, GPIO_PIN_1);     
    Delay_Loop(pulse_width);             
    GPIO_ResetBits(GPIOA, GPIO_PIN_1);   
    Delay_Loop(100000);                  
}

/* ====================================================================
 * 🚀 [โซนฝึกเขียนโค้ด] ภารกิจ: "บิดวอลลุ่ม ควบคุมหุ่นยนต์!"
 * ==================================================================== */
int main(void) {
    /* 1. เปิดสวิตช์เตรียมความพร้อมให้อุปกรณ์ */
    Setup_Knob(); // เตรียมตัวหมุน (PA0)
    
    // เตรียมแขนเซอร์โว (PA1)
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP; 
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    uint32_t knob_value = 0;
    uint32_t target_pulse = PULSE_MIN;

    /* 2. ลูปการทำงานหลัก (หุ่นยนต์จะทำตามเราตลอดเวลา) */
    while(1) {
        
        // ก. อ่านค่าจากมือเราที่บิดตัวหมุน (ได้เลข 0 ถึง 4095)
        knob_value = Read_Knob(); 
        
        // ข. ใช้คณิตศาสตร์เวทมนตร์ แปลงค่าตัวหมุน ให้กลายเป็นองศาเซอร์โว
        target_pulse = PULSE_MIN + ((knob_value * (PULSE_MAX - PULSE_MIN)) / KNOB_MAX);
        
        // ค. ส่งคำสั่งไปบอกเซอร์โวให้ขยับไปที่ตำแหน่งนั้นทันที!
        Servo_Step(target_pulse);
        
    }
}