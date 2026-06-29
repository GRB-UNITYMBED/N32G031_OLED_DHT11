#include "n32g031.h"
#include "n32g031_gpio.h"
#include "n32g031_rcc.h"
#include "core_cm0.h"
#include "dht11.h"
#include "oled.h"
#include "utils.h"
#include <stdio.h> 

int main(void)
{
    uint8_t current_temp = 0;
    uint8_t current_humi = 0;
    
    // ตัวแปรเก็บค่าเดิม เพื่อใช้อัปเดตจอเฉพาะตอนที่ค่าเปลี่ยน (กันจอกะพริบ)
    uint8_t last_temp = 255;
    uint8_t last_humi = 255;
    uint8_t was_error = 1; // ตัวเช็กสถานะ Error

    char buffer[20];

    SystemInit();
    SystemCoreClockUpdate();
    
    delay_ms(500); 
    OLED_Init();
    OLED_Clear();
    DHT11_Init(); 
    
    // หน้าจอต้อนรับ
    OLED_ShowString(10, 24, "System Ready", 16);
    delay_ms(1500); 
    
    OLED_Clear();

    while (1)
    {
        if (DHT11_Read_Data(&current_temp, &current_humi) == 1)
        {
            // ถ้ารอบที่แล้วเป็น Error ให้ล้างหน้าจอแล้วพิมพ์หัวข้อใหม่
            if (was_error) {
                OLED_Clear();
                OLED_ShowString(0, 0, "Temp:      ", 8);
                OLED_ShowString(0, 32, "Humidity:         ", 8);
                was_error = 0;
                last_temp = 255; // บังคับให้วาดเลขใหม่
                last_humi = 255;
            }

            // 1. อัปเดตอุณหภูมิ (เฉพาะตอนค่าเปลี่ยนเท่านั้น จอจะได้ไม่กะพริบ)
            if (current_temp != last_temp) {
                // ใช้ %02d เผื่อเลขหลักเดียว ให้มี 2 หลักเสมอ (เช่น 08, 25)
                // เว้นวรรคหน้า C ไว้ 2 ช่อง เพื่อเอาตัว o ไปแทรก
                sprintf(buffer, "%02d  C        ", current_temp);
                OLED_ShowString(0, 12, buffer, 16); 
                
                // พิมพ์ตัว o ที่พิกัด x=24 (หน้าตัว C พอดี)
                OLED_ShowString(24, 12, "o", 8); 
                
                last_temp = current_temp; // จำค่าล่าสุดไว้
            }

            // 2. อัปเดตความชื้น (เฉพาะตอนค่าเปลี่ยนเท่านั้น)
            if (current_humi != last_humi) {
                sprintf(buffer, "%02d %%        ", current_humi);
                OLED_ShowString(0, 44, buffer, 16); 
                
                last_humi = current_humi; // จำค่าล่าสุดไว้
            }
        }
        else
        {
            // ถ้าหลุด ให้ล้างจอพิมพ์ Error ครั้งเดียว (ไม่ให้กะพริบ)
            if (!was_error) {
                OLED_Clear();
                was_error = 1;
            }
            OLED_ShowString(0, 0,  "Sensor Error!     ", 16);
            OLED_ShowString(0, 24, "Check Wiring.     ", 16);
        }

        delay_ms(10); 
    }
}