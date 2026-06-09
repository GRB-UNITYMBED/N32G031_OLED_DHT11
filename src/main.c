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
    char buffer[20];

    SystemInit();
    SystemCoreClockUpdate();
    
    delay_ms(500); 
    OLED_Init();
    OLED_Clear();
    DHT11_Init(); 

    OLED_Clear();

    
    // หน้าจอต้อนรับ (Splash Screen)
    // ใช้ภาษาอังกฤษเพื่อป้องกันฟอนต์ไม่รองรับภาษาไทย
    OLED_ShowString(28, 16, "Unitymbed", 16);      // จัดให้อยู่กึ่งกลาง
    OLED_ShowString(8, 40, "Created by Biw", 16); // จัดให้อยู่กึ่งกลาง
    delay_ms(2000); // โชว์ค้างไว้ 2 วินาที
    
    OLED_Clear();

    // พิมพ์หัวข้อทิ้งไว้เลย (ไม่ต้องลบอีก)
    OLED_ShowString(0, 0, "Temperature:", 16);
    OLED_ShowString(0, 32, "Humidity:", 16);

    while (1)
    {
        // อ่านค่าจาก DHT11
        if (DHT11_Read_Data(&current_temp, &current_humi) == 1)
        {
            // พิมพ์ตัวเลขทับลงไป (ใส่ Spacebar เผื่อไว้ลบตัวอักษรเก่า)
            sprintf(buffer, "%02d C      ", current_temp);
            OLED_ShowString(0, 16, buffer, 16); 
            
            sprintf(buffer, "%02d %%      ", current_humi);
            OLED_ShowString(0, 48, buffer, 16); 
        }
        else
        {
            // ถ้าอ่านค่าไม่ได้ ให้พิมพ์ Error ทับบรรทัดตัวเลข
            OLED_ShowString(0, 16, "Error     ", 16);
            OLED_ShowString(0, 48, "Error     ", 16);
        }

        // หน่วงเวลา 2 วินาที (DHT11 ไม่ควรอ่านถี่กว่านี้)
        delay_ms(2000); 
    }
}