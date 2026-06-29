# Smart Temperature and Humidity Monitor

![Normal Operation](doc/images/boot.png)

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

## Screen Prompts & Output

| 1. Boot Screen | 2. Normal Operation | 3. Error State |
| :--- | :--- | :--- |
| Upon startup, the screen displays a welcome message for 1.5 seconds.<br><br>**Display:**<br>`System Ready` | The system updates the numbers only when the values change to prevent screen flickering.<br><br>**Display:**<br>`Temp:     XX °C`<br>`Humidity: XX %` | If the DHT11 is disconnected or damaged, the system clears the screen and displays a warning.<br><br>**Display:**<br>`Sensor Error!`<br>`Check Wiring.` |
| ![Boot Screen](doc/images/boot.png) | ![Normal Operation](doc/images/normal.png) | ![Error State](doc/images/error.png) |

## Troubleshooting
* **Screen constantly shows "Sensor Error! Check Wiring.":** Ensure the PA5 wire is not loose and verify the DHT11 VCC/GND connections are correct.
* **OLED screen is completely dark:** Check if the PB6 (SCL) and PB7 (SDA) wires are swapped, and verify the 3.3V power supply.
