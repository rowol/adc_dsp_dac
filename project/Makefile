#
# Build sample ADC -> filter -> DAC path code
#

OOCD = openocd
OOCD_SCRIPT = scripts/openocd/stm32f4discovery.cfg


SRCS=main.c dac.c adc.c gpio_square_wave.c tmr_sample.c 
SRCS += system_stm32f4xx.c
SRCS += stm32f4xx_adc.c stm32f4xx_dac.c stm32f4xx_gpio.c stm32f4xx_rcc.c
SRCS += stm32f4xx_tim.c misc.c


# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=adc_dac

#######################################################################################

STM_COMMON=../STM32F4-Discovery_FW_V1.1.0
CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

OBJDIR=obj/

CFLAGS += -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -funsigned-char

ifndef SOFTFLOAT
 #For hard float
 CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
 CFLAGS += -fsingle-precision-constant -Wdouble-promotion
else
 #For soft float
 CFLAGS += -msoft-float
endif


#Tell linker to garbage collect and discard unused sections
CFLAGS += -Wl,--gc-sections 

#Strip the debug information, so as to make the code as small as possible.  
#(I presume that you'd want to do this in a dead-code removal build.)
CFLAGS += -Wl,-s


INCLUDES  = -I.

# Include files from STM libraries
INCLUDES += -I$(STM_COMMON)/Libraries/CMSIS/Include 
INCLUDES += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
INCLUDES += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc


# add startup file to build
ASM_SRCS = startup_stm32f4xx.s 
ASM_OBJS= $(addprefix $(OBJDIR),$(ASM_SRCS:.s=.o))

OBJS = $(addprefix $(OBJDIR),$(SRCS:.c=.o))




vpath %.c \
$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src 

vpath %.s \
$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO


.PHONY: all proj 

all: $(OBJDIR) proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(ASM_OBJS) $(OBJS)  
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@ 

$(OBJDIR)%.o: %.s 
	$(CC) $(CFLAGS) $< -c -o $@ 



clean:
	rm -f $(OBJDIR)*.o
	rm -f *~
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin


# Flash the STM32F4
burn: all
	@echo DOWNLOAD to TARGET
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
		-c "init" \
		-c "halt 0" \
		-c "flash write_image erase unlock $(PROJ_NAME).elf" \
		-c "reset run" \
		-c "shutdown" 2>&1
