/*
 * Copyright (c) 2015 Arduino LLC.  All right reserved.
 * Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <hal_gpio.h>
#include <sam.h>
#include <variant.h>
#include "bl.h"

#define GENERIC_CLOCK_GENERATOR_MAIN       (0u)

#define CPU_FREQUENCY                      F_CPU
#define VARIANT_MAINOSC                    (32768ul)

#if defined(__SAMD51__)
#define GENERIC_CLOCK_GENERATOR_XOSC32K    (3u)
#define GENERIC_CLOCK_GENERATOR_48M		     (1u)
#define GENERIC_CLOCK_GENERATOR_48M_SYNC   GCLK_SYNCBUSY_GENCTRL1
#define GENERIC_CLOCK_GENERATOR_100M	     (2u)
#define GENERIC_CLOCK_GENERATOR_100M_SYNC	 GCLK_SYNCBUSY_GENCTRL2
#define GENERIC_CLOCK_GENERATOR_12M        (4u)
#define GENERIC_CLOCK_GENERATOR_12M_SYNC   GCLK_SYNCBUSY_GENCTRL4
#define MAIN_CLOCK_SOURCE				           GCLK_GENCTRL_SRC_DPLL0
#define GENERIC_CLOCK_GENERATOR_1M		     (7u)
#define CRYSTALLESS
#else
#define GENERIC_CLOCK_GENERATOR_XOSC32K    (1u)
#endif

#define GENERIC_CLOCK_GENERATOR_OSC32K     (1u)
#define GENERIC_CLOCK_GENERATOR_OSCULP32K  (2u) /* Initialized at reset for WDT */
#define GENERIC_CLOCK_GENERATOR_OSC8M      (3u)
#define GENERIC_CLOCK_MULTIPLEXER_DFLL48M  (0u)

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

void memory_initialize(void) {
    uint32_t *source, *destiny;

    source = &__etext;
    destiny = &__data_start__;

    if ((&__data_start__ != &__data_end__) && (source != destiny)) {
        for ( ; destiny < &__data_end__; destiny++, source++) {
            *destiny = *source;
        }
    }

    if (&__bss_start__ != &__bss_end__) {
        for (destiny = &__bss_start__; destiny < &__bss_end__; destiny++) {
            *destiny = 0ul;
        }
    }
}

void initialize_dpll() {
    OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_RESETVALUE;
    OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_RESETVALUE;
    while (OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.ENABLE != 0);

    OSCCTRL->Dpll[1].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_RESETVALUE;
    OSCCTRL->Dpll[1].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_RESETVALUE;
    while (OSCCTRL->Dpll[1].DPLLSYNCBUSY.bit.ENABLE != 0);

    // PLL0 is 120MHz
    GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL0].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK7_Val);
    OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0x00) | OSCCTRL_DPLLRATIO_LDR(59); // 120 Mhz
    while (OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.DPLLRATIO);

    // MUST USE LBYPASS DUE TO BUG IN REV A OF SAMD51
    OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
    OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;
    while (OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK == 0);


    // PLL1 is 100MHz
    GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL1].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK7_Val);
    OSCCTRL->Dpll[1].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0x00) | OSCCTRL_DPLLRATIO_LDR(49); // 100 Mhz
    while (OSCCTRL->Dpll[1].DPLLSYNCBUSY.bit.DPLLRATIO);

    // MUST USE LBYPASS DUE TO BUG IN REV A OF SAMD51
    OSCCTRL->Dpll[1].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
    OSCCTRL->Dpll[1].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;
    while (OSCCTRL->Dpll[1].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[1].DPLLSTATUS.bit.LOCK == 0);
}

#define FK_PIN_EEPROM_LOCK GPIO(GPIO_PORTB, 1)

void bod33_wait_for(uint8_t level) {
    SEGGER_RTT_printf(0, "bl: bod33-waiting: %d\n", level);

    gpio_set_pin_direction(FK_PIN_EEPROM_LOCK, GPIO_DIRECTION_OUT);
    gpio_set_pin_level(FK_PIN_EEPROM_LOCK, 0);

    SUPC->BOD33.bit.ENABLE = 0;
    while (!SUPC->STATUS.bit.B33SRDY) {
    }

    SUPC->BOD33.reg = SUPC_BOD33_LEVEL(level) |
        SUPC_BOD33_HYST(0) |
        SUPC_BOD33_ACTION_NONE;

    SUPC->BOD33.bit.ENABLE = 1;
    while (!SUPC->STATUS.bit.B33SRDY) {
    }

    uint32_t waiting = 0u;
    uint32_t clearing = 0;

    while (clearing < 100) {
        while (SUPC->STATUS.bit.BOD33DET) {
            gpio_set_pin_level(FK_PIN_EEPROM_LOCK, 1);
            waiting++;
            clearing = 0;
        }
        gpio_set_pin_level(FK_PIN_EEPROM_LOCK, 0);

        clearing++;
    }

    SUPC->BOD33.bit.ENABLE = 0;
    while (!SUPC->STATUS.bit.B33SRDY) {
    }

    SEGGER_RTT_printf(0, "bl: bod33-waited: %d", waiting);
    SEGGER_RTT_printf(0, "\n\n");
}

void delay(uint32_t ms);

void board_configure_supply_controller() {

    while (true) {
        for (int32_t i = 230; i <= 255; ++i) {
            bod33_wait_for(i);
        }

        SEGGER_RTT_printf(0, "\n");
        for (int32_t i = 0; i < 50; ++i) {
            SEGGER_RTT_printf(0, ".");
        }
        SEGGER_RTT_printf(0, "\n");
    }

    /*
    SUPC->BOD33.reg = SUPC_BOD33_LEVEL(240) |
        SUPC_BOD33_HYST(0) |
        SUPC_BOD33_ACTION_RESET;

    SUPC->BOD33.bit.ENABLE = 1;
    while (!SUPC->STATUS.bit.B33SRDY) {
    }
    */
}

void board_minimal_initialize(void) {
    /* Set 1 Flash Wait State for 48MHz */
    NVMCTRL->CTRLA.reg |= NVMCTRL_CTRLA_RWS(0);

    /* Software reset the module to ensure it is re-initialized correctly */
    /* Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
     * CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete
     */
    GCLK->CTRLA.bit.SWRST = 1;
    while (GCLK->SYNCBUSY.bit.SWRST) {
        /* wait for reset to complete */
    }

    // Temporarily switch the CPU to the internal 32k oscillator while we
    // reconfigure the DFLL.
    GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) |
        GCLK_GENCTRL_OE |
        GCLK_GENCTRL_GENEN;

    while (GCLK->SYNCBUSY.bit.GENCTRL0) {
        /* Wait for synchronization */
    }

    // Configure the DFLL for USB clock recovery.
    OSCCTRL->DFLLCTRLA.reg = 0;

    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP(0x1) |
        OSCCTRL_DFLLMUL_FSTEP(0x1) |
        OSCCTRL_DFLLMUL_MUL(0xBB80);

    while (OSCCTRL->DFLLSYNC.bit.DFLLMUL) {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLCTRLB.reg = 0;
    while (OSCCTRL->DFLLSYNC.bit.DFLLCTRLB) {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLCTRLA.bit.ENABLE = 1;
    while (OSCCTRL->DFLLSYNC.bit.ENABLE) {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLVAL.reg = OSCCTRL->DFLLVAL.reg;
    while(OSCCTRL->DFLLSYNC.bit.DFLLVAL ) {}

    OSCCTRL->DFLLCTRLB.reg = OSCCTRL_DFLLCTRLB_WAITLOCK |
        OSCCTRL_DFLLCTRLB_CCDIS | OSCCTRL_DFLLCTRLB_USBCRM ;

    while (!OSCCTRL->STATUS.bit.DFLLRDY) {
        /* Wait for synchronization */
    }

    // 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
    GCLK->GENCTRL[0].reg =
        GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL) |
        GCLK_GENCTRL_IDC |
        GCLK_GENCTRL_OE |
        GCLK_GENCTRL_GENEN;

    while (GCLK->SYNCBUSY.bit.GENCTRL0) {
        /* Wait for synchronization */
    }

    /*
     * Now that all system clocks are configured, we can set CLKDIV .
     * These values are normally the ones present after Reset.
     */
    MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;

    SysTick_Config(1000);
}

void board_initialize(void) {
    NVMCTRL->CTRLA.reg |= NVMCTRL_CTRLA_RWS(0);

    GCLK->CTRLA.bit.SWRST = 1;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST) {
        /* wait for reset to complete */
    }

    /* ----------------------------------------------------------------------------------------------
     * 2) Put OSCULP32K as source of Generic Clock Generator 3
     */
    GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_XOSC32K].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN; //generic clock gen 3


    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL3) {
        /* Wait for synchronization */
    }

    /* ----------------------------------------------------------------------------------------------
     * 3) Put Generic Clock Generator 3 as source for Generic Clock Gen 0 (DFLL48M reference)
     */
    GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN;

    /* ----------------------------------------------------------------------------------------------
     * 4) Enable DFLL48M clock
     */

    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0) {
        /* Wait for synchronization */
    }

    /* DFLL Configuration in Open Loop mode */

    OSCCTRL->DFLLCTRLA.reg = 0;
    //GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK3_Val);

    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP( 0x1 ) |
        OSCCTRL_DFLLMUL_FSTEP( 0x1 ) |
        OSCCTRL_DFLLMUL_MUL( 0 );

    while (OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLMUL) {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLCTRLB.reg = 0;
    while (OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLCTRLB) {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLCTRLA.reg |= OSCCTRL_DFLLCTRLA_ENABLE;
    while (OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_ENABLE) {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLVAL.reg = OSCCTRL->DFLLVAL.reg;
    while (OSCCTRL->DFLLSYNC.bit.DFLLVAL);

    OSCCTRL->DFLLCTRLB.reg = OSCCTRL_DFLLCTRLB_WAITLOCK | OSCCTRL_DFLLCTRLB_CCDIS | OSCCTRL_DFLLCTRLB_USBCRM;

    while (!OSCCTRL->STATUS.bit.DFLLRDY) {
        /* Wait for synchronization */
    }

    GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_1M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(24u);

    while (GCLK->SYNCBUSY.bit.GENCTRL5) {
        /* Wait for synchronization */
    }

    /* ------------------------------------------------------------------------
     * Set up the PLLs
     */

    initialize_dpll();


    /* ------------------------------------------------------------------------
     * Set up the peripheral clocks
     */

    GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_48M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) |
        GCLK_GENCTRL_IDC |
        GCLK_GENCTRL_GENEN;

    while (GCLK->SYNCBUSY.reg & GENERIC_CLOCK_GENERATOR_48M_SYNC) {
        /* Wait for synchronization */
    }

    GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_100M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DPLL1_Val) |
        GCLK_GENCTRL_IDC |
        GCLK_GENCTRL_GENEN;

    while (GCLK->SYNCBUSY.reg & GENERIC_CLOCK_GENERATOR_100M_SYNC) {
    }

    GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_12M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) |
        GCLK_GENCTRL_IDC |
        GCLK_GENCTRL_DIV(4) |
        GCLK_GENCTRL_DIVSEL |
        GCLK_GENCTRL_GENEN;

    while (GCLK->SYNCBUSY.reg & GENERIC_CLOCK_GENERATOR_12M_SYNC) {
        /* Wait for synchronization */
    }

    board_configure_supply_controller();

    /*---------------------------------------------------------------------
     * Set up main clock
     */

    GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_MAIN].reg = GCLK_GENCTRL_SRC(MAIN_CLOCK_SOURCE) |
        GCLK_GENCTRL_IDC |
        GCLK_GENCTRL_GENEN;

    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0) {
        /* Wait for synchronization */
    }

    MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;

    /* If desired, enable cache! */
#if defined(ENABLE_CACHE)
    __disable_irq();
    CMCC->CTRL.reg = 1;
    __enable_irq();
#endif
}
