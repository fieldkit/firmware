################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), DARWIN)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/tc \
hpl/systick \
hpl/dmac \
hpl/evsys \
hpl/pac \
hal/src \
hal/utils/src \
gcc \
hpl/pm \
hpl/core \
hpl/sysctrl \
hpl/eic \
hpl/sercom \
examples \
hpl/gclk \
hpl/wdt \
gcc/gcc \
hpl/rtc \
hpl/nvmctrl \
hpl/dsu

# List the object files
OBJS +=  \
hal/src/hal_io.o \
hpl/systick/hpl_systick.o \
hal/src/hal_calendar.o \
hpl/wdt/hpl_wdt.o \
hpl/eic/hpl_eic.o \
hpl/evsys/hpl_evsys.o \
hal/utils/src/utils_syscalls.o \
hpl/nvmctrl/hpl_nvmctrl.o \
hal/src/hal_timer.o \
hal/src/hal_i2c_m_sync.o \
hal/src/hal_delay.o \
hpl/sysctrl/hpl_sysctrl.o \
hpl/core/hpl_init.o \
hal/src/hal_wdt.o \
hpl/core/hpl_core_m0plus_base.o \
hal/utils/src/utils_assert.o \
hpl/pac/hpl_pac.o \
hpl/pm/hpl_pm.o \
hpl/sercom/hpl_sercom.o \
hpl/gclk/hpl_gclk.o \
hal/src/hal_flash.o \
hal/src/hal_init.o \
hal/src/hal_crc_sync.o \
main.o \
hal/utils/src/utils_list.o \
hpl/rtc/hpl_rtc.o \
gcc/system_samd09.o \
examples/driver_examples.o \
driver_init.o \
hal/src/hal_ext_irq.o \
hal/src/hal_gpio.o \
hal/src/hal_evsys.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
hpl/dmac/hpl_dmac.o \
gcc/gcc/startup_samd09.o \
atmel_start.o \
hpl/tc/hpl_tc.o \
hal/src/hal_atomic.o \
hpl/dsu/hpl_dsu.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"hpl/systick/hpl_systick.o" \
"hal/src/hal_calendar.o" \
"hpl/wdt/hpl_wdt.o" \
"hpl/eic/hpl_eic.o" \
"hpl/evsys/hpl_evsys.o" \
"hal/utils/src/utils_syscalls.o" \
"hpl/nvmctrl/hpl_nvmctrl.o" \
"hal/src/hal_timer.o" \
"hal/src/hal_i2c_m_sync.o" \
"hal/src/hal_delay.o" \
"hpl/sysctrl/hpl_sysctrl.o" \
"hpl/core/hpl_init.o" \
"hal/src/hal_wdt.o" \
"hpl/core/hpl_core_m0plus_base.o" \
"hal/utils/src/utils_assert.o" \
"hpl/pac/hpl_pac.o" \
"hpl/pm/hpl_pm.o" \
"hpl/sercom/hpl_sercom.o" \
"hpl/gclk/hpl_gclk.o" \
"hal/src/hal_flash.o" \
"hal/src/hal_init.o" \
"hal/src/hal_crc_sync.o" \
"main.o" \
"hal/utils/src/utils_list.o" \
"hpl/rtc/hpl_rtc.o" \
"gcc/system_samd09.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"hal/src/hal_ext_irq.o" \
"hal/src/hal_gpio.o" \
"hal/src/hal_evsys.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"hpl/dmac/hpl_dmac.o" \
"gcc/gcc/startup_samd09.o" \
"atmel_start.o" \
"hpl/tc/hpl_tc.o" \
"hal/src/hal_atomic.o" \
"hpl/dsu/hpl_dsu.o"

# List the directories containing header files
DIR_INCLUDES +=  \
-I"../" \
-I"../config" \
-I"../examples" \
-I"../hal/include" \
-I"../hal/utils/include" \
-I"../hpl/core" \
-I"../hpl/dmac" \
-I"../hpl/dsu" \
-I"../hpl/eic" \
-I"../hpl/evsys" \
-I"../hpl/gclk" \
-I"../hpl/nvmctrl" \
-I"../hpl/pac" \
-I"../hpl/pm" \
-I"../hpl/port" \
-I"../hpl/rtc" \
-I"../hpl/sercom" \
-I"../hpl/sysctrl" \
-I"../hpl/systick" \
-I"../hpl/tc" \
-I"../hpl/wdt" \
-I"../hri" \
-I"../" \
-I"../CMSIS/Core/Include" \
-I"../include"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"hpl/wdt/hpl_wdt.d" \
"hal/src/hal_calendar.d" \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"hpl/systick/hpl_systick.d" \
"hpl/eic/hpl_eic.d" \
"hal/utils/src/utils_syscalls.d" \
"hal/src/hal_crc_sync.d" \
"hal/src/hal_i2c_m_sync.d" \
"hpl/nvmctrl/hpl_nvmctrl.d" \
"hal/src/hal_timer.d" \
"hpl/core/hpl_core_m0plus_base.d" \
"hal/src/hal_wdt.d" \
"hpl/pac/hpl_pac.d" \
"hal/utils/src/utils_assert.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/sysctrl/hpl_sysctrl.d" \
"hal/src/hal_flash.d" \
"hpl/gclk/hpl_gclk.d" \
"hpl/dmac/hpl_dmac.d" \
"hal/src/hal_init.d" \
"driver_init.d" \
"main.d" \
"examples/driver_examples.d" \
"gcc/system_samd09.d" \
"hpl/rtc/hpl_rtc.d" \
"hal/utils/src/utils_list.d" \
"hal/src/hal_sleep.d" \
"hpl/sercom/hpl_sercom.d" \
"hal/src/hal_evsys.d" \
"hal/src/hal_gpio.d" \
"hpl/evsys/hpl_evsys.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"hpl/tc/hpl_tc.d" \
"hpl/pm/hpl_pm.d" \
"hpl/dsu/hpl_dsu.d" \
"gcc/gcc/startup_samd09.d" \
"hal/src/hal_ext_irq.d"

OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS) -Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m0plus \
 \
-T"../gcc/gcc/samd09d14a_flash.ld" \
-L"../gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME).elf"

	

# Compiler targets




%.o: %.c
	@echo Building file: $<
	@echo ARM/GNU C Compiler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMD09D14A__ -mcpu=cortex-m0plus  \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARM/GNU Assembler
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMD09D14A__ -mcpu=cortex-m0plus  \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMD09D14A__ -mcpu=cortex-m0plus  \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec