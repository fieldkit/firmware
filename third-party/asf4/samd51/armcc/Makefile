
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
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/ramecc \
hpl/evsys \
hpl/qspi \
examples \
hpl/systick \
hpl/oscctrl \
hpl/tc \
hpl/osc32kctrl \
samd51a/armcc/Device/SAMD51/Source \
hpl/dmac \
hal/src \
hpl/eic \
hpl/wdt \
hpl/rtc \
hpl/trng \
hpl/pm \
hpl/aes \
hpl/cmcc \
hpl/gclk \
hal/utils/src \
hpl/mclk \
samd51a/armcc/Device/SAMD51/Source/ARM \
hpl/pac \
hpl/icm \
hpl/sercom \
hpl/core \
hpl/nvmctrl \
hpl/dsu

# List the object files
OBJS +=  \
hal/src/hal_io.o \
hpl/qspi/hpl_qspi.o \
hal/src/hal_cache.o \
hal/src/hal_calendar.o \
hpl/wdt/hpl_wdt.o \
hpl/eic/hpl_eic.o \
hpl/evsys/hpl_evsys.o \
hpl/core/hpl_core_m4.o \
hal/src/hal_aes_sync.o \
hpl/systick/hpl_systick.o \
hpl/nvmctrl/hpl_nvmctrl.o \
hal/src/hal_spi_m_sync.o \
samd51a/armcc/Device/SAMD51/Source/ARM/startup_samd51.o \
hal/src/hal_timer.o \
hpl/aes/hpl_aes.o \
hal/src/hal_i2c_m_sync.o \
samd51a/armcc/Device/SAMD51/Source/system_samd51.o \
hal/src/hal_delay.o \
hal/src/hal_sha_sync.o \
hpl/pm/hpl_pm.o \
hpl/core/hpl_init.o \
hpl/ramecc/hpl_ramecc.o \
hal/src/hal_wdt.o \
hal/utils/src/utils_assert.o \
hpl/pac/hpl_pac.o \
hpl/oscctrl/hpl_oscctrl.o \
hal/src/hal_rand_sync.o \
hpl/trng/hpl_trng.o \
hpl/mclk/hpl_mclk.o \
hpl/sercom/hpl_sercom.o \
hpl/gclk/hpl_gclk.o \
hal/src/hal_flash.o \
hal/src/hal_init.o \
hpl/icm/hpl_icm.o \
hal/src/hal_crc_sync.o \
main.o \
hal/utils/src/utils_list.o \
hpl/osc32kctrl/hpl_osc32kctrl.o \
hpl/rtc/hpl_rtc.o \
examples/driver_examples.o \
driver_init.o \
hal/src/hal_ext_irq.o \
hal/src/hal_atomic.o \
hal/src/hal_gpio.o \
hal/src/hal_qspi_sync.o \
hal/src/hal_evsys.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
hpl/dmac/hpl_dmac.o \
atmel_start.o \
hpl/tc/hpl_tc.o \
hpl/cmcc/hpl_cmcc.o \
hpl/dsu/hpl_dsu.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"hpl/qspi/hpl_qspi.o" \
"hal/src/hal_cache.o" \
"hal/src/hal_calendar.o" \
"hpl/wdt/hpl_wdt.o" \
"hpl/eic/hpl_eic.o" \
"hpl/evsys/hpl_evsys.o" \
"hpl/core/hpl_core_m4.o" \
"hal/src/hal_aes_sync.o" \
"hpl/systick/hpl_systick.o" \
"hpl/nvmctrl/hpl_nvmctrl.o" \
"hal/src/hal_spi_m_sync.o" \
"samd51a/armcc/Device/SAMD51/Source/ARM/startup_samd51.o" \
"hal/src/hal_timer.o" \
"hpl/aes/hpl_aes.o" \
"hal/src/hal_i2c_m_sync.o" \
"samd51a/armcc/Device/SAMD51/Source/system_samd51.o" \
"hal/src/hal_delay.o" \
"hal/src/hal_sha_sync.o" \
"hpl/pm/hpl_pm.o" \
"hpl/core/hpl_init.o" \
"hpl/ramecc/hpl_ramecc.o" \
"hal/src/hal_wdt.o" \
"hal/utils/src/utils_assert.o" \
"hpl/pac/hpl_pac.o" \
"hpl/oscctrl/hpl_oscctrl.o" \
"hal/src/hal_rand_sync.o" \
"hpl/trng/hpl_trng.o" \
"hpl/mclk/hpl_mclk.o" \
"hpl/sercom/hpl_sercom.o" \
"hpl/gclk/hpl_gclk.o" \
"hal/src/hal_flash.o" \
"hal/src/hal_init.o" \
"hpl/icm/hpl_icm.o" \
"hal/src/hal_crc_sync.o" \
"main.o" \
"hal/utils/src/utils_list.o" \
"hpl/osc32kctrl/hpl_osc32kctrl.o" \
"hpl/rtc/hpl_rtc.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"hal/src/hal_ext_irq.o" \
"hal/src/hal_atomic.o" \
"hal/src/hal_gpio.o" \
"hal/src/hal_qspi_sync.o" \
"hal/src/hal_evsys.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"hpl/dmac/hpl_dmac.o" \
"atmel_start.o" \
"hpl/tc/hpl_tc.o" \
"hpl/cmcc/hpl_cmcc.o" \
"hpl/dsu/hpl_dsu.o"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"hpl/wdt/hpl_wdt.d" \
"hal/src/hal_calendar.d" \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"hpl/ramecc/hpl_ramecc.d" \
"hpl/qspi/hpl_qspi.d" \
"hpl/core/hpl_core_m4.d" \
"hpl/eic/hpl_eic.d" \
"hal/src/hal_crc_sync.d" \
"samd51a/armcc/Device/SAMD51/Source/system_samd51.d" \
"hal/src/hal_i2c_m_sync.d" \
"hpl/aes/hpl_aes.d" \
"hpl/nvmctrl/hpl_nvmctrl.d" \
"hal/src/hal_timer.d" \
"hal/src/hal_spi_m_sync.d" \
"hal/src/hal_wdt.d" \
"hpl/cmcc/hpl_cmcc.d" \
"hpl/pac/hpl_pac.d" \
"hal/utils/src/utils_assert.d" \
"hal/src/hal_sha_sync.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/pm/hpl_pm.d" \
"hal/src/hal_flash.d" \
"hpl/gclk/hpl_gclk.d" \
"hpl/dmac/hpl_dmac.d" \
"hpl/icm/hpl_icm.d" \
"hal/src/hal_init.d" \
"hpl/trng/hpl_trng.d" \
"hal/src/hal_rand_sync.d" \
"hpl/mclk/hpl_mclk.d" \
"driver_init.d" \
"hpl/systick/hpl_systick.d" \
"hpl/osc32kctrl/hpl_osc32kctrl.d" \
"hal/src/hal_cache.d" \
"main.d" \
"examples/driver_examples.d" \
"hpl/rtc/hpl_rtc.d" \
"samd51a/armcc/Device/SAMD51/Source/ARM/startup_samd51.d" \
"hal/src/hal_qspi_sync.d" \
"hal/src/hal_aes_sync.d" \
"hal/utils/src/utils_list.d" \
"hal/src/hal_sleep.d" \
"hal/src/hal_ext_irq.d" \
"hal/src/hal_evsys.d" \
"hal/src/hal_gpio.d" \
"hpl/evsys/hpl_evsys.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"hpl/tc/hpl_tc.d" \
"hpl/oscctrl/hpl_oscctrl.d" \
"hpl/dsu/hpl_dsu.d" \
"hpl/sercom/hpl_sercom.d"

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
	@echo Invoking: ARMCC Linker
	$(QUOTE)armlink$(QUOTE) --ro-base 0x00000000 --entry 0x00000000 --rw-base 0x20000000 --entry Reset_Handler --first __Vectors \
--strict --summary_stderr --info summarysizes --map --xref --callgraph --symbols \
--info sizes --info totals --info unused --info veneers --list $(OUTPUT_FILE_NAME).map \
-o $(OUTPUT_FILE_NAME).elf --cpu Cortex-M4 \
$(OBJS_AS_ARGS)

	@echo Finished building target: $@

# Compiler target(s)




%.o: %.c
	@echo Building file: $<
	@echo ARMCC Compiler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M4 -D__SAMD51P20A__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/aes" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/dsu" -I"../hpl/eic" -I"../hpl/evsys" -I"../hpl/gclk" -I"../hpl/icm" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pac" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/qspi" -I"../hpl/ramecc" -I"../hpl/rtc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hpl/tc" -I"../hpl/trng" -I"../hpl/wdt" -I"../hri" -I"../" -I"../CMSIS/Core/Include" -I"../samd51a/include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARMCC Assembler
	$(QUOTE)armasm$(QUOTE) -g --apcs=interwork --cpu Cortex-M4 --pd "D__SAMD51P20A__ SETA 1" \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/aes" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/dsu" -I"../hpl/eic" -I"../hpl/evsys" -I"../hpl/gclk" -I"../hpl/icm" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pac" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/qspi" -I"../hpl/ramecc" -I"../hpl/rtc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hpl/tc" -I"../hpl/trng" -I"../hpl/wdt" -I"../hri" -I"../" -I"../CMSIS/Core/Include" -I"../samd51a/include"  \
--depend "$(@:%.o=%.d)" -o "$@" "$<"

	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARMCC Preprocessing Assembler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M4 -D__SAMD51P20A__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/aes" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/dsu" -I"../hpl/eic" -I"../hpl/evsys" -I"../hpl/gclk" -I"../hpl/icm" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pac" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/qspi" -I"../hpl/ramecc" -I"../hpl/rtc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hpl/tc" -I"../hpl/trng" -I"../hpl/wdt" -I"../hri" -I"../" -I"../CMSIS/Core/Include" -I"../samd51a/include"  \
--depend "$@"  -o "$@" "$<"

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
	rm -f $(OUTPUT_FILE_NAME).map $(OUTPUT_FILE_NAME).elf
