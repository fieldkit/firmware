/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <y> EIC Clock Source
// <id> eic_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for EIC.
#ifndef CONF_GCLK_EIC_SRC
#define CONF_GCLK_EIC_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EIC_FREQUENCY
 * \brief EIC's Clock frequency
 */
#ifndef CONF_GCLK_EIC_FREQUENCY
#define CONF_GCLK_EIC_FREQUENCY 8000000
#endif

/**
 * \def CONF_CPU_FREQUENCY
 * \brief CPU's Clock frequency
 */
#ifndef CONF_CPU_FREQUENCY
#define CONF_CPU_FREQUENCY 8000000
#endif

// <y> Core Clock Source
// <id> core_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for CORE.
#ifndef CONF_GCLK_SERCOM0_CORE_SRC
#define CONF_GCLK_SERCOM0_CORE_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

// <y> Slow Clock Source
// <id> slow_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the slow clock source.
#ifndef CONF_GCLK_SERCOM0_SLOW_SRC
#define CONF_GCLK_SERCOM0_SLOW_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_SERCOM0_CORE_FREQUENCY
 * \brief SERCOM0's Core Clock frequency
 */
#ifndef CONF_GCLK_SERCOM0_CORE_FREQUENCY
#define CONF_GCLK_SERCOM0_CORE_FREQUENCY 8000000
#endif

/**
 * \def CONF_GCLK_SERCOM0_SLOW_FREQUENCY
 * \brief SERCOM0's Slow Clock frequency
 */
#ifndef CONF_GCLK_SERCOM0_SLOW_FREQUENCY
#define CONF_GCLK_SERCOM0_SLOW_FREQUENCY 8000000
#endif

// <y> Core Clock Source
// <id> core_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for CORE.
#ifndef CONF_GCLK_SERCOM1_CORE_SRC
#define CONF_GCLK_SERCOM1_CORE_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

// <y> Slow Clock Source
// <id> slow_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the slow clock source.
#ifndef CONF_GCLK_SERCOM1_SLOW_SRC
#define CONF_GCLK_SERCOM1_SLOW_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_SERCOM1_CORE_FREQUENCY
 * \brief SERCOM1's Core Clock frequency
 */
#ifndef CONF_GCLK_SERCOM1_CORE_FREQUENCY
#define CONF_GCLK_SERCOM1_CORE_FREQUENCY 8000000
#endif

/**
 * \def CONF_GCLK_SERCOM1_SLOW_FREQUENCY
 * \brief SERCOM1's Slow Clock frequency
 */
#ifndef CONF_GCLK_SERCOM1_SLOW_FREQUENCY
#define CONF_GCLK_SERCOM1_SLOW_FREQUENCY 8000000
#endif

// <y> RTC Clock Source
// <id> rtc_clk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for RTC.
#ifndef CONF_GCLK_RTC_SRC
#define CONF_GCLK_RTC_SRC GCLK_CLKCTRL_GEN_GCLK5_Val
#endif

/**
 * \def CONF_GCLK_RTC_FREQUENCY
 * \brief RTC's Clock frequency
 */
#ifndef CONF_GCLK_RTC_FREQUENCY
#define CONF_GCLK_RTC_FREQUENCY 1024
#endif

// <y> TC Clock Source
// <id> tc_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for TC.
#ifndef CONF_GCLK_TC1_SRC
#define CONF_GCLK_TC1_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_TC1_FREQUENCY
 * \brief TC1's Clock frequency
 */
#ifndef CONF_GCLK_TC1_FREQUENCY
#define CONF_GCLK_TC1_FREQUENCY 8000000
#endif

// <y> EVSYS Channel 0 Clock Source
// <id> evsys_clk_selection_0

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for channel 0.
#ifndef CONF_GCLK_EVSYS_CHANNEL_0_SRC
#define CONF_GCLK_EVSYS_CHANNEL_0_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EVSYS_CHANNEL_0_FREQUENCY
 * \brief EVSYS's Clock frequency
 */

#ifndef CONF_GCLK_EVSYS_CHANNEL_0_FREQUENCY
#define CONF_GCLK_EVSYS_CHANNEL_0_FREQUENCY 8000000.0
#endif

// <y> EVSYS Channel 1 Clock Source
// <id> evsys_clk_selection_1

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for channel 1.
#ifndef CONF_GCLK_EVSYS_CHANNEL_1_SRC
#define CONF_GCLK_EVSYS_CHANNEL_1_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EVSYS_CHANNEL_1_FREQUENCY
 * \brief EVSYS's Clock frequency
 */

#ifndef CONF_GCLK_EVSYS_CHANNEL_1_FREQUENCY
#define CONF_GCLK_EVSYS_CHANNEL_1_FREQUENCY 8000000.0
#endif

// <y> EVSYS Channel 2 Clock Source
// <id> evsys_clk_selection_2

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for channel 2.
#ifndef CONF_GCLK_EVSYS_CHANNEL_2_SRC
#define CONF_GCLK_EVSYS_CHANNEL_2_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EVSYS_CHANNEL_2_FREQUENCY
 * \brief EVSYS's Clock frequency
 */

#ifndef CONF_GCLK_EVSYS_CHANNEL_2_FREQUENCY
#define CONF_GCLK_EVSYS_CHANNEL_2_FREQUENCY 8000000.0
#endif

// <y> EVSYS Channel 3 Clock Source
// <id> evsys_clk_selection_3

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for channel 3.
#ifndef CONF_GCLK_EVSYS_CHANNEL_3_SRC
#define CONF_GCLK_EVSYS_CHANNEL_3_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EVSYS_CHANNEL_3_FREQUENCY
 * \brief EVSYS's Clock frequency
 */

#ifndef CONF_GCLK_EVSYS_CHANNEL_3_FREQUENCY
#define CONF_GCLK_EVSYS_CHANNEL_3_FREQUENCY 8000000.0
#endif

// <y> EVSYS Channel 4 Clock Source
// <id> evsys_clk_selection_4

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for channel 4.
#ifndef CONF_GCLK_EVSYS_CHANNEL_4_SRC
#define CONF_GCLK_EVSYS_CHANNEL_4_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EVSYS_CHANNEL_4_FREQUENCY
 * \brief EVSYS's Clock frequency
 */

#ifndef CONF_GCLK_EVSYS_CHANNEL_4_FREQUENCY
#define CONF_GCLK_EVSYS_CHANNEL_4_FREQUENCY 8000000.0
#endif

// <y> EVSYS Channel 5 Clock Source
// <id> evsys_clk_selection_5

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for channel 5.
#ifndef CONF_GCLK_EVSYS_CHANNEL_5_SRC
#define CONF_GCLK_EVSYS_CHANNEL_5_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif

/**
 * \def CONF_GCLK_EVSYS_CHANNEL_5_FREQUENCY
 * \brief EVSYS's Clock frequency
 */

#ifndef CONF_GCLK_EVSYS_CHANNEL_5_FREQUENCY
#define CONF_GCLK_EVSYS_CHANNEL_5_FREQUENCY 8000000.0
#endif

// <y> WDT Clock Source
// <id> wdt_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for WDT.
#ifndef CONF_GCLK_WDT_SRC
#define CONF_GCLK_WDT_SRC GCLK_CLKCTRL_GEN_GCLK5_Val
#endif
/**
 * \def CONF_GCLK_WDT_FREQUENCY
 * \brief WDT's Clock frequency
 */
// #ifndef CONF_GCLK_WDT_FREQUENCY
// #define CONF_GCLK_WDT_FREQUENCY 8000000
// #endif

// <<< end of configuration section >>>

#endif // PERIPHERAL_CLK_CONFIG_H
