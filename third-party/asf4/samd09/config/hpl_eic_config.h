/* Auto-generated config file hpl_eic_config.h */
#ifndef HPL_EIC_CONFIG_H
#define HPL_EIC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Non-Maskable Interrupt Control
// <q> Non-Maskable Interrupt Filter Enable
// <i> Indicates whether the mon-maskable interrupt filter is enabled or not
// <id> eic_arch_nmifilten
#ifndef CONF_EIC_NMIFILTEN
#define CONF_EIC_NMIFILTEN 0
#endif

// <y> Non-Maskable Interrupt Sense
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines non-maskable interrupt sense
// <id> eic_arch_nmisense
#ifndef CONF_EIC_NMISENSE
#define CONF_EIC_NMISENSE EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </h>

// <e> Interrupt 0 Settings
// <id> eic_arch_enable_irq_setting0
#ifndef CONF_EIC_ENABLE_IRQ_SETTING0
#define CONF_EIC_ENABLE_IRQ_SETTING0 0
#endif

// <q> External Interrupt 0 Event Output Enable
// <i> Indicates whether the external interrupt 0 event output is enabled or not
// <id> eic_arch_extinteo0
#ifndef CONF_EIC_EXTINTEO0
#define CONF_EIC_EXTINTEO0 0
#endif

// <q> External Interrupt 0 Wake-up Enable
// <i> Indicates whether the external interrupt 0 wake-up is enabled or not
// <id> eic_arch_wakeupen0
#ifndef CONF_EIC_WAKEUPEN0
#define CONF_EIC_WAKEUPEN0 0
#endif

// <q> External Interrupt 0 Filter Enable
// <i> Indicates whether the external interrupt 0 filter is enabled or not
// <id> eic_arch_filten0
#ifndef CONF_EIC_FILTEN0
#define CONF_EIC_FILTEN0 0
#endif

// <y> Input 0 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense0
#ifndef CONF_EIC_SENSE0
#define CONF_EIC_SENSE0 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

// <e> Interrupt 1 Settings
// <id> eic_arch_enable_irq_setting1
#ifndef CONF_EIC_ENABLE_IRQ_SETTING1
#define CONF_EIC_ENABLE_IRQ_SETTING1 0
#endif

// <q> External Interrupt 1 Event Output Enable
// <i> Indicates whether the external interrupt 1 event output is enabled or not
// <id> eic_arch_extinteo1
#ifndef CONF_EIC_EXTINTEO1
#define CONF_EIC_EXTINTEO1 0
#endif

// <q> External Interrupt 1 Wake-up Enable
// <i> Indicates whether the external interrupt 1 wake-up is enabled or not
// <id> eic_arch_wakeupen1
#ifndef CONF_EIC_WAKEUPEN1
#define CONF_EIC_WAKEUPEN1 0
#endif

// <q> External Interrupt 1 Filter Enable
// <i> Indicates whether the external interrupt 1 filter is enabled or not
// <id> eic_arch_filten1
#ifndef CONF_EIC_FILTEN1
#define CONF_EIC_FILTEN1 0
#endif

// <y> Input 1 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense1
#ifndef CONF_EIC_SENSE1
#define CONF_EIC_SENSE1 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

// <e> Interrupt 2 Settings
// <id> eic_arch_enable_irq_setting2
#ifndef CONF_EIC_ENABLE_IRQ_SETTING2
#define CONF_EIC_ENABLE_IRQ_SETTING2 0
#endif

// <q> External Interrupt 2 Event Output Enable
// <i> Indicates whether the external interrupt 2 event output is enabled or not
// <id> eic_arch_extinteo2
#ifndef CONF_EIC_EXTINTEO2
#define CONF_EIC_EXTINTEO2 0
#endif

// <q> External Interrupt 2 Wake-up Enable
// <i> Indicates whether the external interrupt 2 wake-up is enabled or not
// <id> eic_arch_wakeupen2
#ifndef CONF_EIC_WAKEUPEN2
#define CONF_EIC_WAKEUPEN2 0
#endif

// <q> External Interrupt 2 Filter Enable
// <i> Indicates whether the external interrupt 2 filter is enabled or not
// <id> eic_arch_filten2
#ifndef CONF_EIC_FILTEN2
#define CONF_EIC_FILTEN2 0
#endif

// <y> Input 2 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense2
#ifndef CONF_EIC_SENSE2
#define CONF_EIC_SENSE2 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

// <e> Interrupt 3 Settings
// <id> eic_arch_enable_irq_setting3
#ifndef CONF_EIC_ENABLE_IRQ_SETTING3
#define CONF_EIC_ENABLE_IRQ_SETTING3 0
#endif

// <q> External Interrupt 3 Event Output Enable
// <i> Indicates whether the external interrupt 3 event output is enabled or not
// <id> eic_arch_extinteo3
#ifndef CONF_EIC_EXTINTEO3
#define CONF_EIC_EXTINTEO3 0
#endif

// <q> External Interrupt 3 Wake-up Enable
// <i> Indicates whether the external interrupt 3 wake-up is enabled or not
// <id> eic_arch_wakeupen3
#ifndef CONF_EIC_WAKEUPEN3
#define CONF_EIC_WAKEUPEN3 0
#endif

// <q> External Interrupt 3 Filter Enable
// <i> Indicates whether the external interrupt 3 filter is enabled or not
// <id> eic_arch_filten3
#ifndef CONF_EIC_FILTEN3
#define CONF_EIC_FILTEN3 0
#endif

// <y> Input 3 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense3
#ifndef CONF_EIC_SENSE3
#define CONF_EIC_SENSE3 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

// <e> Interrupt 4 Settings
// <id> eic_arch_enable_irq_setting4
#ifndef CONF_EIC_ENABLE_IRQ_SETTING4
#define CONF_EIC_ENABLE_IRQ_SETTING4 0
#endif

// <q> External Interrupt 4 Event Output Enable
// <i> Indicates whether the external interrupt 4 event output is enabled or not
// <id> eic_arch_extinteo4
#ifndef CONF_EIC_EXTINTEO4
#define CONF_EIC_EXTINTEO4 0
#endif

// <q> External Interrupt 4 Wake-up Enable
// <i> Indicates whether the external interrupt 4 wake-up is enabled or not
// <id> eic_arch_wakeupen4
#ifndef CONF_EIC_WAKEUPEN4
#define CONF_EIC_WAKEUPEN4 0
#endif

// <q> External Interrupt 4 Filter Enable
// <i> Indicates whether the external interrupt 4 filter is enabled or not
// <id> eic_arch_filten4
#ifndef CONF_EIC_FILTEN4
#define CONF_EIC_FILTEN4 0
#endif

// <y> Input 4 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense4
#ifndef CONF_EIC_SENSE4
#define CONF_EIC_SENSE4 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

// <e> Interrupt 5 Settings
// <id> eic_arch_enable_irq_setting5
#ifndef CONF_EIC_ENABLE_IRQ_SETTING5
#define CONF_EIC_ENABLE_IRQ_SETTING5 1
#endif

// <q> External Interrupt 5 Event Output Enable
// <i> Indicates whether the external interrupt 5 event output is enabled or not
// <id> eic_arch_extinteo5
#ifndef CONF_EIC_EXTINTEO5
#define CONF_EIC_EXTINTEO5 0
#endif

// <q> External Interrupt 5 Wake-up Enable
// <i> Indicates whether the external interrupt 5 wake-up is enabled or not
// <id> eic_arch_wakeupen5
#ifndef CONF_EIC_WAKEUPEN5
#define CONF_EIC_WAKEUPEN5 1
#endif

// <q> External Interrupt 5 Filter Enable
// <i> Indicates whether the external interrupt 5 filter is enabled or not
// <id> eic_arch_filten5
#ifndef CONF_EIC_FILTEN5
#define CONF_EIC_FILTEN5 0
#endif

// <y> Input 5 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense5
#ifndef CONF_EIC_SENSE5
#define CONF_EIC_SENSE5 EIC_NMICTRL_NMISENSE_BOTH_Val
#endif
// </e>

// <e> Interrupt 6 Settings
// <id> eic_arch_enable_irq_setting6
#ifndef CONF_EIC_ENABLE_IRQ_SETTING6
#define CONF_EIC_ENABLE_IRQ_SETTING6 0
#endif

// <q> External Interrupt 6 Event Output Enable
// <i> Indicates whether the external interrupt 6 event output is enabled or not
// <id> eic_arch_extinteo6
#ifndef CONF_EIC_EXTINTEO6
#define CONF_EIC_EXTINTEO6 0
#endif

// <q> External Interrupt 6 Wake-up Enable
// <i> Indicates whether the external interrupt 6 wake-up is enabled or not
// <id> eic_arch_wakeupen6
#ifndef CONF_EIC_WAKEUPEN6
#define CONF_EIC_WAKEUPEN6 0
#endif

// <q> External Interrupt 6 Filter Enable
// <i> Indicates whether the external interrupt 6 filter is enabled or not
// <id> eic_arch_filten6
#ifndef CONF_EIC_FILTEN6
#define CONF_EIC_FILTEN6 0
#endif

// <y> Input 6 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense6
#ifndef CONF_EIC_SENSE6
#define CONF_EIC_SENSE6 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

// <e> Interrupt 7 Settings
// <id> eic_arch_enable_irq_setting7
#ifndef CONF_EIC_ENABLE_IRQ_SETTING7
#define CONF_EIC_ENABLE_IRQ_SETTING7 0
#endif

// <q> External Interrupt 7 Event Output Enable
// <i> Indicates whether the external interrupt 7 event output is enabled or not
// <id> eic_arch_extinteo7
#ifndef CONF_EIC_EXTINTEO7
#define CONF_EIC_EXTINTEO7 0
#endif

// <q> External Interrupt 7 Wake-up Enable
// <i> Indicates whether the external interrupt 7 wake-up is enabled or not
// <id> eic_arch_wakeupen7
#ifndef CONF_EIC_WAKEUPEN7
#define CONF_EIC_WAKEUPEN7 0
#endif

// <q> External Interrupt 7 Filter Enable
// <i> Indicates whether the external interrupt 7 filter is enabled or not
// <id> eic_arch_filten7
#ifndef CONF_EIC_FILTEN7
#define CONF_EIC_FILTEN7 0
#endif

// <y> Input 7 Sense Configuration
// <EIC_NMICTRL_NMISENSE_NONE_Val"> No detection
// <EIC_NMICTRL_NMISENSE_RISE_Val"> Rising-edge detection
// <EIC_NMICTRL_NMISENSE_FALL_Val"> Falling-edge detection
// <EIC_NMICTRL_NMISENSE_BOTH_Val"> Both-edges detection
// <EIC_NMICTRL_NMISENSE_HIGH_Val"> High-level detection
// <EIC_NMICTRL_NMISENSE_LOW_Val"> Low-level detection
// <i> This defines input sense trigger
// <id> eic_arch_sense7
#ifndef CONF_EIC_SENSE7
#define CONF_EIC_SENSE7 EIC_NMICTRL_NMISENSE_NONE_Val
#endif
// </e>

#ifndef CONFIG_EIC_EXTINT_MAP
#define CONFIG_EIC_EXTINT_MAP {5, PIN_PA25},
#endif

// <<< end of configuration section >>>

#endif // HPL_EIC_CONFIG_H
