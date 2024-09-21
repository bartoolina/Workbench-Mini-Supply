SHELL := /bin/bash
# Created by bartool
# 15.03.2020
# TODO
# Create pattern for upload
# Create pattern for init (create main.c, src dir, setting.json, c_cpp_properties.json)
PROCESSOR := ATtiny84
F_CPU := 8000000UL

DUDE_PROGRAMMER := usbasp
DUDE_PORT := usb

SRC_DIR := src
BUILD_DIR := build/$(PROCESSOR)

OPTIMIZE = Os
AVRDUDE_PATH := C:/avr/avrdude
CC := avr-gcc
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
OBJSIZE := avr-size

PROJECT_NAME := $(notdir $(CURDIR))
ELF := $(BUILD_DIR)/$(PROJECT_NAME).elf
LSS := $(BUILD_DIR)/$(PROJECT_NAME).lss
MAP := $(BUILD_DIR)/$(PROJECT_NAME).map
FLASH_IMAGE := $(BUILD_DIR)/$(PROJECT_NAME).hex
EEPROM_IMAGE := $(BUILD_DIR)/$(PROJECT_NAME).eep

# C_FILES := $(wildcard $(SRC_DIR)/*/*.c) $(wildcard $(SRC_DIR)/*.c)
# OBJ_FILES := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_FILES))
# DIRS := $(sort $(dir $(OBJ_FILES)))

C_SOURCES += src/main.c
C_SOURCES += src/usi_twi/USI_TWI_Master.c
C_SOURCES += src/oled/oled.c
C_SOURCES += src/bitmap/bitmap.c
C_SOURCES += src/ads1115/ads1115.c
C_SOURCES += src/mcp4651/mcp4651.c
C_SOURCES += src/ina219/ina219.c
# C_SOURCES += src/i2c_soft/i2c_soft.c

C_INCLUDES += -Isrc/main
C_INCLUDES += -Isrc/usi_twi
C_INCLUDES += -Isrc/oled
C_INCLUDES += -Isrc/bitmap
C_INCLUDES += -Isrc/ads1115
C_INCLUDES += -Isrc/mcp4651
C_INCLUDES += -Isrc/ina219
# C_INCLUDES += -Isrc/i2c_soft

# lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))
MCU :=  $(shell echo $(PROCESSOR) | tr '[:upper:]' '[:lower:]')
# MCU = $(call lc,$(PROCESSOR))

# kratka sa oznaczene opcje wykorzystywane przez eclipse
CFLAGS  = -mmcu=$(MCU)#
CFLAGS += -$(OPTIMIZE)#
# CFLAGS += -g
CFLAGS += -std=gnu99#
CFLAGS += -funsigned-char#
CFLAGS += -funsigned-bitfields#
CFLAGS += -fpack-struct#
#CFLAGS += -ffreestanding
CFLAGS += -ffunction-sections#
CFLAGS += -fdata-sections#
CFLAGS += -fshort-enums#
CFLAGS += -fverbose-asm
CFLAGS += -Wall#
#CFLAGS += -Ic:/avr/avr_toolchain/avr/include
CFLAGS += -Wstrict-prototypes
CFLAGS += -DF_CPU=$(F_CPU)#

ESC := \033[
STOP := $(ESC)0m
RESET := $(ESC)$(STOP)
DEBUG := $(RESET)$(ESC)36;49;2m
FOCUS := $(RESET)$(ESC)35;49;10m
PROGRAM := $(RESET)$(ESC)33;49;1m

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
DIRS := $(sort $(dir $(OBJECTS)))
CFLAGS += $(C_INCLUDES)

all: $(ELF) $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(DIRS) sizedummy

build: $(ELF)

$(BUILD_DIR)/%.o: %.c Makefile $(DIRS)
	@echo -e "$(DEBUG)>>> Building file: $(FOCUS)$<$(DEBUG) --> $(FOCUS)$@$(DEBUG) <<<$(STOP)"
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIRS): 
	@echo -e "$(DEBUG)>>> Making a dir: $(FOCUS)$@$(DEBUG) <<<$(STOP)"
	@mkdir -p $@

$(ELF): $(OBJECTS)
	@echo -e "$(DEBUG)>>> Building target: $(FOCUS)$@$(DEBUG) <<<$(STOP)"
	@echo -e "$(DEBUG)>>> Invoking: AVR C Linker <<<$(STOP)"
	$(CC) -Wl,-Map,$(MAP) -mmcu=$(MCU) -o $(ELF) $(OBJECTS)

$(LSS): $(ELF)
	@echo -e "$(DEBUG)>>> Invoking: AVR Create Extended Listing $(FOCUS)$@$(DEBUG) <<<$(STOP)"
	$(OBJDUMP) -h -S $(ELF) > $(LSS)

$(FLASH_IMAGE): $(ELF)
	@echo -e "$(DEBUG)>>> Create flash image (ihex format) $(FOCUS)$@$(DEBUG) <<<$(STOP)"
	$(OBJCOPY) -j .text -j .data -O ihex $(ELF) $(FLASH_IMAGE)

$(EEPROM_IMAGE): $(ELF)
	@echo -e "$(DEBUG)>>> Create eeprom image (ihex format) $(FOCUS)$@$(DEBUG) <<<$(STOP)"
	$(OBJCOPY) -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex $(ELF) $(EEPROM_IMAGE)

sizedummy: $(ELF)
	@echo -e "$(DEBUG)>>> Invoking: Print size <<<$(STOP)"
	$(OBJSIZE) --format=avr --mcu=$(MCU) $(ELF)

clean: 
	@echo -e "$(DEBUG)>>> Cleaning... <<<$(STOP)"
	rm -rf $(ELF) $(LSS) $(MAP) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(OBJECTS)
	rm -rf $(DIRS)

info: 
	@echo -e "$(DEBUG)MCU: $(MCU) F_CPU: $(F_CPU)$(STOP)"
	@echo -e "$(DEBUG)Programer: $(DUDE_PROGRAMMER) Port: $(DUDE_PORT)$(STOP)"
	@echo -e "$(DEBUG)Source dir: $(SRC_DIR) Build dir: $(BUILD_DIR)$(STOP)"
	@echo -e "$(DEBUG)Project name: $(PROJECT_NAME)$(STOP)"
	@echo -e "$(FOCUS)elf: $(ELF) lss: $(LSS) map: $(MAP)$(STOP)"
	@echo -e "$(DEBUG)hex: $(FLASH_IMAGE) $(ESC)$(STOP)$(FOCUS)eep: $(EEPROM_IMAGE)$(STOP)"
	@echo -e "$(DEBUG)All dirs: $(DIRS)$(STOP)"
	@echo -e "$(DEBUG)All c files: $(C_SOURCES)$(STOP)"
	@echo -e "$(DEBUG)All o files: $(OBJECTS)$(STOP)"
	@echo -e "$(DEBUG)Cflags: $(CFLAGS)$(STOP)"
	@echo $(LOWER_VAR)
	@echo "done"

$(BUILD_DIR):
	mkdir -p $@

# nie testowane! sprawdzic poprawnosc parametrow
flash: $(FLASH_IMAGE)
	@echo -e " $(PROGRAM)>>> Uplod only flash <<<$(STOP)"
	avrdude -p $(MCU) -P $(DUDE_PORT) -c $(DUDE_PROGRAMMER) -U flash:w:$(FLASH_IMAGE):i

flash_eeprom:
	@echo -e " $(PROGRAM)>>> Upload flash and eeprom <<<$(STOP)"
	avrdude -p $(MCU) -P $(DUDE_PORT) -c $(DUDE_PROGRAMMER) -U flash:w:$(FLASH_IMAGE):i -D -U eeprom:w:$(EEPROM_IMAGE):i

# .SECONDEXPANSION:
# $(OBJ_FILES): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $$(wildcard $$(dir $(SRC_DIR)/%.c)*.h) 
# 	@if [ ! -d "$(dir $@)" ]; then echo -e "$(DEBUG) >>> Create dir: $(FOCUS)$(dir $@)$(DEBUG) <<<$(STOP)"; mkdir -p $(dir $@); fi
# 	@echo -e "$(DEBUG)>>> Building file: $(FOCUS)$<$(DEBUG) --> $(FOCUS)$@$(DEBUG) <<<$(STOP)"
# 	$(CC) $(CFLAGS) -c -o $@ $<

