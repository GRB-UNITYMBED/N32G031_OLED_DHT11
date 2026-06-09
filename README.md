# N32G031 — OLED + DHT11 Temperature & Humidity Monitor

Real-time temperature & humidity monitor on a 0.96" I²C OLED, driven by a
**Nations N32G031** (Cortex-M0) reading a **DHT11** sensor.

This repo is the **macOS-ready** build of the project: it builds and flashes
end-to-end on a Mac with the standard ARM toolchain + OpenOCD (no Windows-only
tooling, no vendor IDE). Bare-metal, direct register access — no Nations HAL.

![Normal Operation](docs/images/normal.png)

---

## Hardware

| Device | Pin | N32G031 | Notes |
| :--- | :--- | :--- | :--- |
| **DHT11** | DATA | **PA5** | Pull-up to VCC if the module lacks one |
| | VCC | 3.3V / 5V | check sensor rating |
| | GND | GND | |
| **OLED (I²C)** | SCL | **PB6** | I²C clock |
| | SDA | **PB7** | I²C data |
| | VCC | 3.3V | |
| | GND | GND | |

Debug probe: any **CMSIS-DAP** adapter over **SWD**.

---

## Build on macOS

**Prerequisites**

```sh
brew install --cask gcc-arm-embedded     # arm-none-eabi-gcc 10.3+
brew install openocd                     # 0.12+
brew install node                        # used by the Makefile to read unitymbed.json
```

**Build**

```sh
make            # -> build/oled_dht11.elf / .bin / .hex
make clean
```

Typical size: **~7.7 KB flash, ~1.6 KB RAM** (64 KB / 8 KB part — plenty of
headroom). `--specs=nano.specs` keeps `printf`/`sprintf` small (integer only).

---

## Flash

Through **UnityMbed** (Flash button) or directly with OpenOCD:

```sh
openocd -f openocd.cfg -c "init" -c "n32_program build/oled_dht11.bin" -c "shutdown"
```

Expected output:

```
N32: flash mass-erased
N32: programming 7664 bytes @ 0x08000000 (1916 words)
N32: verify OK
```

### Why a custom flash routine?

OpenOCD's stock `stm32f1x` driver probes `DBGMCU_IDCODE` at `0x40015800` — an
address the N32G031 doesn't implement — so its auto-probe fails. The N32G031
FPEC is otherwise register-identical to the STM32F1 (controller at
`0x40022000`), so `openocd.cfg` drives FPEC directly.

To stay fast it streams each erase block as **one batched `write_memory`**
instead of one USB round-trip per word. That takes flashing from ~22 s down to
**~1.5 s** for this image, and every flash ends with a hard `verify_image` so a
dropped word can never report success silently.

---

## Behaviour

| State | Screen |
| :--- | :--- |
| **Boot** (1.5 s) | `System Ready` |
| **Normal** | `Temp: XX °C` / `Humidity: XX %` — redraws only on change to avoid flicker |
| **Error** | `Sensor Error!` / `Check Wiring.` when the DHT11 doesn't respond |

| Boot | Normal | Error |
| :---: | :---: | :---: |
| ![boot](docs/images/boot.png) | ![normal](docs/images/normal.png) | ![error](docs/images/error.png) |

---

## Layout

```
src/         application + drivers used (main, oled, dht11, utils, system, it)
inc/         project headers (oled.h, dht11.h, fonts.h, …)
drivers/     Nations N32G031 peripheral library (src/ + inc/)
startup/     startup_n32g031_gcc.s  (vector table, Reset_Handler)
filemain/    alternate main() revisions kept for reference
n32g031_flash.ld   linker script (64K flash @ 0x08000000, 8K RAM)
openocd.cfg        CMSIS-DAP/SWD flash routine (n32_program)
Makefile           cross-platform build
unitymbed.json     project manifest
```

The Makefile compiles every driver and lets `--gc-sections` strip whatever the
firmware doesn't call, so you never chase missing symbols by hand.

---

Part of the [UnityMbed](https://github.com/GRB-UNITYMBED) N32G031 example set.
