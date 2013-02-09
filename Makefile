##########################################################################
#                    STM32F4 Project Template Makefile                   #
##########################################################################

TARGET:=main
TOOLCHAIN_PATH:=~/Development/sat/bin
TOOLCHAIN_PREFIX:=arm-none-eabi
OPTLVL:=3 # Optimization level, can be [0, 1, 2, 3, s].

PROJECT_NAME:=$(notdir $(lastword $(CURDIR)))



LINKER_SCRIPT=stm32_flash.ld


INCLUDE=-I$(CURDIR)/inc
INCLUDE+=-I$(CURDIR)/lib/inc
INCLUDE+=-I$(CURDIR)/lib/cmsis
INCLUDE+=-I$(CURDIR)/lib/STM32_EVAL/Common
INCLUDE+=-I$(CURDIR)/lib/STM32_EVAL/STM3240_41_G_EVAL
INCLUDE+=-I$(CURDIR)/lib/lwip_v1.3.2/src/include
INCLUDE+=-I$(CURDIR)/lib/lwip_v1.3.2/port/STM32F4x7
INCLUDE+=-I$(CURDIR)/lib/lwip_v1.3.2/src/include/ipv4
INCLUDE+=-I$(CURDIR)/lib/lwip_v1.3.2/port/STM32F4x7/Standalone




# vpath is used so object files are written to the current directory instead
# of the same directory as their source files
vpath %.c $(CURDIR)/lib/src
vpath %.s $(STARTUP)

ASRC=startup_stm32f4xx.s

# Project Source Files




# Standard Peripheral Source Files
SRC+=$(wildcard $(CURDIR)/*.c)
SRC+=$(wildcard $(CURDIR)/lib/src/*.c)
SRC+=$(wildcard $(CURDIR)/lib/lwip_v1.3.2/port/STM32F4x7/Standalone/*.c)
SRC+=$(wildcard $(CURDIR)/lib/lwip_v1.3.2/src/api/*.c)
SRC+=$(wildcard $(CURDIR)/lib/lwip_v1.3.2/src/core/*.c)
SRC+=$(wildcard $(CURDIR)/lib/lwip_v1.3.2/src/core/ipv4/*.c)
SRC+=$(wildcard $(CURDIR)/lib/lwip_v1.3.2/src/netif/*.c)


CDEFS=-DSTM32F4XX
CDEFS+=-DUSE_STDPERIPH_DRIVER

MCUFLAGS=-mcpu=cortex-m4 -mthumb -std=c99
#MCUFLAGS=-mcpu=cortex-m4 -mthumb -mlittle-endian -mfpu=fpa -mfloat-abi=hard -mthumb-interwork
#MCUFLAGS=-mcpu=cortex-m4 -mfpu=vfpv4-sp-d16 -mfloat-abi=hard
COMMONFLAGS=-O$(OPTLVL) -g -Wall
CFLAGS=$(COMMONFLAGS) $(MCUFLAGS) $(INCLUDE) $(CDEFS)

LDLIBS=
LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections \
        -nostartfiles -Wl,--gc-sections,-T$(LINKER_SCRIPT)

#####
#####

OBJ = $(SRC:%.c=%.o) $(ASRC:%.s=%.o)

CC=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gcc
LD=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gcc
OBJCOPY=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-objcopy
AS=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-as
AR=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-ar
GDB=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gdb

all: bin

bin: $(OBJ)
	$(CC) -o $(TARGET).elf $(LDFLAGS) $(OBJ)	$(LDLIBS)
	$(OBJCOPY) -O ihex   $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin


flash: bin
	st-flash write $(TARGET).bin 0x08000000

.PHONY: clean

clean:
	rm -f $(OBJ)
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex
	rm -f $(TARGET).bin
