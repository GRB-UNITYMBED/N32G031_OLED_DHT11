# Smart Temperature and Humidity Monitor

![Normal Operation](images/normal.png)

A real-time temperature and humidity monitoring system displaying data on an OLED screen, using the N32G031 microcontroller and a DHT11 sensor.

## Hardware Requirements
* Microcontroller: **N32G031**
* Temperature & Humidity Sensor: **DHT11**
* Display: **OLED Display (I2C)** (0.96-inch or any standard display driver)

## Wiring Diagram

| Device | Pin | Connect to N32G031 (Pin) | Notes |
| :--- | :--- | :--- | :--- |
| **DHT11** | DATA / OUT | **PA5** | Requires a pull-up resistor (if not built into the module) |
| | VCC | 3.3V / 5V | Check sensor voltage rating |
| | GND | GND | |
| **OLED (I2C)**| SCL | **PB6** | I2C Clock |
| | SDA | **PB7** | I2C Data |
| | VCC | 3.3V | |
| | GND | GND | |

## Screen Prompts & Output

1. **Boot Screen:**
   Upon startup, the screen displays a welcome message for 1.5 seconds.
   > `System Ready`
![Boot Screen](images/boot.png)

2. **Normal Operation:**
   The system updates the numbers only when the values change to prevent screen flickering.
   > `Temp:      XX °C`
   > `Humidity:  XX %`
![Normal Operation](images/normal.png)
k
3. **Error State:**
   If the DHT11 is disconnected or damaged, the system clears the screen and immediately displays a warning message.
   > `Sensor Error!`
   > `Check Wiring.`
![Error State](images/error.png)

## Troubleshooting
* **Screen constantly shows "Sensor Error! Check Wiring.":** Ensure the PA5 wire is not loose and verify the DHT11 VCC/GND connections are correct.
* **OLED screen is completely dark:** Check if the PB6 (SCL) and PB7 (SDA) wires are swapped, and verify the 3.3V power supply.
