# UnityMbed Makefile — N32G031 (OLED + DHT11, full Nations SDK)
# Cross-platform (macOS / Linux). Flash is handled by UnityMbed via openocd.cfg.
PROJECT   := $(shell node -e "console.log(require('./unitymbed.json').name)" 2>/dev/null || echo app)
MCU_FLAGS := -mcpu=cortex-m0 -mthumb

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

INCLUDES := -Iinc -Idrivers/inc

CFLAGS  := $(MCU_FLAGS) -Os -g3 -Wall $(INCLUDES) \
           -ffunction-sections -fdata-sections $(CFLAGS_EXTRA)

# nano newlib gives integer printf/sprintf in ~10KB instead of ~45KB.
# NO -nostartfiles: Nations startup calls SystemInit + __libc_init_array,
# which need newlib's crt init (crti _init / __libc_init_array).
LDFLAGS := $(MCU_FLAGS) -Tn32g031_flash.ld \
           --specs=nano.specs --specs=nosys.specs \
           -Wl,--gc-sections -Wl,-Map=build/$(PROJECT).map

# Compile every app source + every Nations driver; --gc-sections strips
# whatever the firmware doesn't actually call, so we never chase missing
# symbols by hand.
SRC_C := $(wildcard src/*.c) $(wildcard drivers/src/*.c)
SRC_S := startup/startup_n32g031_gcc.s
OBJ   := $(patsubst %.c,build/%.o,$(SRC_C)) $(patsubst %.s,build/%.o,$(SRC_S))

all: build/$(PROJECT).elf build/$(PROJECT).bin build/$(PROJECT).hex
	@$(SIZE) build/$(PROJECT).elf

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/$(PROJECT).elf: $(OBJ)
	@mkdir -p build
	$(CC) $(OBJ) $(LDFLAGS) -o $@

build/%.bin: build/%.elf
	$(OBJCOPY) -O binary $< $@

build/%.hex: build/%.elf
	$(OBJCOPY) -O ihex $< $@

clean:
	rm -rf build

.PHONY: all clean
