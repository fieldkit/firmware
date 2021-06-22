#!/usr/bin/python3

import subprocess

regular_irqs = [
    # "Reset_Handler",
    "HardFault_Handler",
    # "SVC_Handler",
    # "PendSV_Handler",
    # "SysTick_Handler",
    "WDT_Handler",
    "RTC_Handler",
    "EIC_0_Handler",
    "EIC_1_Handler",
    "EIC_2_Handler",
    "EIC_3_Handler",
    "EIC_4_Handler",
    "EIC_5_Handler",
    "EIC_6_Handler",
    "EIC_7_Handler",
    "EIC_8_Handler",
    "EIC_9_Handler",
    "EIC_10_Handler",
    "EIC_11_Handler",
    "EIC_12_Handler",
    "EIC_13_Handler",
    "EIC_14_Handler",
    "EIC_15_Handler",
    # "NVMCTRL_0_Handler",
    # "NVMCTRL_1_Handler",
    "USB_0_Handler",
    "USB_1_Handler",
    "USB_2_Handler",
    "USB_3_Handler",
    # "SERCOM0_0_Handler",
    # "SERCOM0_1_Handler",
    # "SERCOM0_2_Handler",
    # "SERCOM0_3_Handler",
]

dummy_irqs = [
    "NMI_Handler",
    "MemManage_Handler",
    "BusFault_Handler",
    "UsageFault_Handler",
    "DebugMon_Handler",
    "PM_Handler",
    "MCLK_Handler",
    "OSCCTRL_0_Handler",
    "OSCCTRL_1_Handler",
    "OSCCTRL_2_Handler",
    "OSCCTRL_3_Handler",
    "OSCCTRL_4_Handler",
    "OSC32KCTRL_Handler",
    "SUPC_0_Handler",
    "SUPC_1_Handler",
    "FREQM_Handler",
    "DMAC_0_Handler",
    "DMAC_1_Handler",
    "DMAC_2_Handler",
    "DMAC_3_Handler",
    "DMAC_4_Handler",
    "EVSYS_0_Handler",
    "EVSYS_1_Handler",
    "EVSYS_2_Handler",
    "EVSYS_3_Handler",
    "EVSYS_4_Handler",
    "PAC_Handler",
    "TAL_0_Handler",
    "TAL_1_Handler",
    "RAMECC_Handler",
    "SERCOM1_0_Handler",
    "SERCOM1_1_Handler",
    "SERCOM1_2_Handler",
    "SERCOM1_3_Handler",
    "SERCOM2_0_Handler",
    "SERCOM2_1_Handler",
    "SERCOM2_2_Handler",
    "SERCOM2_3_Handler",
    "SERCOM3_0_Handler",
    "SERCOM3_1_Handler",
    "SERCOM3_2_Handler",
    "SERCOM3_3_Handler",
    "SERCOM4_0_Handler",
    "SERCOM4_1_Handler",
    "SERCOM4_2_Handler",
    "SERCOM4_3_Handler",
    "SERCOM5_0_Handler",
    "SERCOM5_1_Handler",
    "SERCOM5_2_Handler",
    "SERCOM5_3_Handler",
    "SERCOM6_0_Handler",
    "SERCOM6_1_Handler",
    "SERCOM6_2_Handler",
    "SERCOM6_3_Handler",
    "SERCOM7_0_Handler",
    "SERCOM7_1_Handler",
    "SERCOM7_2_Handler",
    "SERCOM7_3_Handler",
    "TCC0_0_Handler",
    "TCC0_1_Handler",
    "TCC0_2_Handler",
    "TCC0_3_Handler",
    "TCC0_4_Handler",
    "TCC0_5_Handler",
    "TCC0_6_Handler",
    "TCC1_0_Handler",
    "TCC1_1_Handler",
    "TCC1_2_Handler",
    "TCC1_3_Handler",
    "TCC1_4_Handler",
    "TCC2_0_Handler",
    "TCC2_1_Handler",
    "TCC2_2_Handler",
    "TCC2_3_Handler",
    "TCC3_0_Handler",
    "TCC3_1_Handler",
    "TCC3_2_Handler",
    "TCC4_0_Handler",
    "TCC4_1_Handler",
    "TCC4_2_Handler",
    "TC0_Handler",
    "TC1_Handler",
    "TC2_Handler",
    "TC3_Handler",
    "TC4_Handler",
    "TC5_Handler",
    "TC6_Handler",
    "TC7_Handler",
    "PDEC_0_Handler",
    "PDEC_1_Handler",
    "PDEC_2_Handler",
    "ADC0_0_Handler",
    "ADC0_1_Handler",
    "ADC1_0_Handler",
    "ADC1_1_Handler",
    "AC_Handler",
    "DAC_0_Handler",
    "DAC_1_Handler",
    "DAC_2_Handler",
    "DAC_3_Handler",
    "DAC_4_Handler",
    "I2S_Handler",
    "PCC_Handler",
    "AES_Handler",
    "TRNG_Handler",
    "ICM_Handler",
    "PUKCC_Handler",
    "QSPI_Handler",
    "SDHC0_Handler",
    "SDHC1_Handler",
]


class FkSegger(gdb.Command):
    "Segger mode."

    def __init__(self):
        super(FkSegger, self).__init__(
            "js", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        if arg is None or len(arg) == 0:
            print("Pass JLink port: js 2331")
            return
        if True:
            gdb.execute("add-symbol-file build/samd51/bootloader/fkbl.elf 0x0000")
        gdb.execute("target extended-remote :" + arg)
        gdb.execute("monitor exec SetRTTSearchRanges 0x20000000 64")
        if False:
            for fn in regular_irqs:
                print("bp %s" % (fn,))
                gdb.execute("b %s" % (fn,))
        if True:
            gdb.execute("b osi_hard_fault_report")
            gdb.execute("b fk_debugger_break")
            gdb.execute("b fk_assert")
        gdb.execute("monitor reset")
        # NOTE GDB is crashing with this?!
        if False:
            gdb.execute("continue")


class FkFixLogs(gdb.Command):
    "Fix logs."

    def __init__(self):
        super(FkFixLogs, self).__init__(
            "jfl", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        gdb.execute("monitor exec SetRTTSearchRanges 0x20000000 64")


class FkRunHosted(gdb.Command):
    "Run hosted tests."

    def __init__(self):
        super(FkRunHosted, self).__init__(
            "jrh", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        made = subprocess.run(["make", "amd64", "-j4"])
        if made.returncode != 0:
            return False
        gdb.execute("run")


class FkRestart(gdb.Command):
    "Restart."

    def __init__(self):
        super(FkRestart, self).__init__(
            "rs", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        gdb.execute("monitor reset")
        # NOTE GDB is crashing with this?!
        if False:
            gdb.execute("continue")


class FkReloadAllAndRun(gdb.Command):
    "Reload all."

    def __init__(self):
        super(FkReloadAllAndRun, self).__init__(
            "jrar", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        made = subprocess.run(["make", "fw", "-j4"])
        if made.returncode != 0:
            # gdb.execute("load build/samd51-pic/fk/fk-bundled-fkb.elf")
            return False
        gdb.execute("load build/samd51/bootloader/fkbl-fkb.elf")
        gdb.execute("load build/samd51/fk/fk-bundled-fkb.elf")
        gdb.execute("monitor reset")
        # NOTE GDB is crashing with this?!
        if False:
            gdb.execute("continue")


class FkReloadAll(gdb.Command):
    "Reload all."

    def __init__(self):
        super(FkReloadAll, self).__init__(
            "jra", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        made = subprocess.run(["make", "fw", "-j4"])
        if made.returncode != 0:
            # gdb.execute("load build/samd51-pic/fk/fk-bundled-fkb.elf")
            return False
        gdb.execute("load build/samd51/bootloader/fkbl-fkb.elf")
        gdb.execute("load build/samd51/fk/fk-bundled-fkb.elf")
        gdb.execute("monitor reset")


# FkReloadAll()
FkReloadAllAndRun()
FkSegger()
# FkRunHosted()
# FkRestart()
# FkFixLogs()
