# N32G031_SERVO_POTENTIOMETER 🤖🎛️

An interactive embedded project demonstrating how to control a servo motor's position in real-time using a potentiometer (analog knob) via the **ADC (Analog-to-Digital Converter)** on the **N32G031** microcontroller. 

This project serves as an excellent educational resource for understanding analog sensor reading, data mapping formulas, and manual robotic hardware control.

---

## 🎯 Learning Points
* **ADC Configuration:** Learn how to enable the peripheral clock on the **AHB bus** specifically for the ADC, and configure a GPIO pin (`PA0`) to run in analog mode.
* **Mathematical Mapping:** Understand how to translate raw 12-bit sensor data ($0$ to $4095$) into precise servo control pulse widths ($2250$ to $5500$) using standard conversion formulas.
* **Real-time Interaction:** Observe how hardware registers continuously capture human input to dynamically drive an actuator with minimal latency.

---

## 🔌 Wiring Diagram

Connect your N32G031 board to the potentiometer and the servo motor as follows:

### 1. Potentiometer (Knob) Connection
| Potentiometer Pin | N32G031 Pin | Description |
| :--- | :---: | :--- |
| **VCC / Outer Pin 1** | **3.3V / 5V** | Power Supply |
| **GND / Outer Pin 3** | **GND** | Ground Reference |
| **Signal / Middle Pin 2** | **PA0** | Analog Input (ADC Channel 0) |

### 2. Servo Motor Connection (e.g., SG90)
| Servo Wire Color | N32G031 Pin | Description |
| :--- | :---: | :--- |
| 🟤 **Brown / Black** | **GND** | Common System Ground |
| 🔴 **Red** | **5V** | Motor Power Supply |
| 🟠 **Orange / Yellow** | **PA1** | Control Signal Pin (PWM Bit-Banging) |

---

## 🛠️ Hardware Constants & Calibration
* **Min Pulse (`PULSE_MIN`):** `2250` (Servo at 0° - Far Left)
* **Max Pulse (`PULSE_MAX`):** `5500` (Servo at 180° - Far Right)
* **Max ADC Value (`KNOB_MAX`):** `4095` (12-bit Resolution)

> 💡 **Mapping Formula Used in Code:**
> `target_pulse = PULSE_MIN + ((knob_value * (PULSE_MAX - PULSE_MIN)) / KNOB_MAX);`

---

## 🚀 Code Execution Loop
Once powered on, the microcontroller performs the following operations inside an infinite loop:
1. **Read:** The ADC samples the analog voltage on `PA0` and converts it to a digital value between `0` and `4095`.
2. **Map:** The digital value is processed through the mapping formula to calculate the exact corresponding pulse width.
3. **Write:** The `Servo_Step` function triggers `PA1` high, holds it for the calculated duration, and pulls it low, forcing the servo to replicate your hand movement instantaneously.

---
Developed by **GRB-UNITYMBED** — Building interactive, high-quality coding and robotics materials for future engineers. 🚀